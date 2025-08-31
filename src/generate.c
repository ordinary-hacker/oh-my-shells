
#include "../include/module_loader.h"
#include "../include/generate.h"
#include "../include/cli.h"
#include "../include/colors.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

char* encode_url(const char* input) {
    const char *hex = "0123456789ABCDEF";
    size_t len = strlen(input);
    char* out = malloc(len * 3 + 1);
    char* p = out;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)input[i];
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
            *p++ = c;
        } else {
            *p++ = '%';
            *p++ = hex[c >> 4];
            *p++ = hex[c & 15];
        }
    }
    *p = 0;
    return out;
}

char* encode_double_url(const char* input) {
    char* once = encode_url(input);
    char* twice = encode_url(once);
    free(once);
    return twice;
}

char* encode_base64(const char* input) {
    static const char tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t len = strlen(input);
    char* out = malloc((len + 2) / 3 * 4 + 1);
    char* p = out;
    for (size_t i = 0; i < len; i += 3) {
        int v = (input[i] << 16) + ((i+1 < len ? input[i+1] : 0) << 8) + (i+2 < len ? input[i+2] : 0);
        *p++ = tbl[(v >> 18) & 0x3F];
        *p++ = tbl[(v >> 12) & 0x3F];
        *p++ = (i+1 < len) ? tbl[(v >> 6) & 0x3F] : '=';
        *p++ = (i+2 < len) ? tbl[v & 0x3F] : '=';
    }
    *p = 0;
    return out;
}

char* encode_base32(const char* input) {
    static const char tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
    size_t len = strlen(input);
    char* out = malloc((len * 8 + 4) / 5 + 1);
    char* p = out;
    int buffer = 0, bitsLeft = 0;
    for (size_t i = 0; i < len; i++) {
        buffer <<= 8;
        buffer |= (unsigned char)input[i];
        bitsLeft += 8;
        while (bitsLeft >= 5) {
            *p++ = tbl[(buffer >> (bitsLeft - 5)) & 0x1F];
            bitsLeft -= 5;
        }
    }
    if (bitsLeft > 0) {
        buffer <<= (5 - bitsLeft);
        *p++ = tbl[buffer & 0x1F];
    }
    *p = 0;
    return out;
}

void generate_command(ModuleRegistry* registry, const char* shell_id, const char* lhost, int lport, const char* encoding, const char* shell_override, const Command* cmd) {
    extern int no_color;
    if (cmd) no_color = cmd->no_color;
    ShellModule* mod = find_module_by_id(registry, shell_id);
    if (!mod) {
        printf("%s\n", COLORIZE(COLOR_BOLD COLOR_RED, "Shell not found:"));
        printf("%s\n", shell_id);
        return;
    }

    const char* shell_to_use = NULL;
    if (shell_override && shell_override[0]) {
        int found = 0;
        if (mod->compatible_shells && mod->compatible_shell_count > 0) {
            for (int i = 0; i < mod->compatible_shell_count; i++) {
                if (mod->compatible_shells[i] && strcmp(mod->compatible_shells[i], shell_override) == 0) {
                    found = 1;
                    break;
                }
            }
        }
        if (!found) {
            printf("%s ", COLORIZE(COLOR_BOLD COLOR_RED, "Error:"));
            printf("Shell '%s' is not compatible with this payload.\n", shell_override);
            if (mod->compatible_shells && mod->compatible_shell_count > 0) {
                printf("%s ", COLORIZE(COLOR_BOLD COLOR_YELLOW, "Compatible shells:"));
                for (int i = 0; i < mod->compatible_shell_count; i++) {
                    printf("%s%s", mod->compatible_shells[i], (i < mod->compatible_shell_count-1) ? ", " : "\n");
                }
            } else {
                printf("%s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "No compatible shells listed for this payload."));
            }
            return;
        }
        shell_to_use = shell_override;
    } else if (mod->default_shell[0]) {
        shell_to_use = mod->default_shell;
    }
    char* payload = generate_payload(mod, lhost, lport);
    if (!payload) {
        printf("%s\n", COLORIZE(COLOR_BOLD COLOR_RED, "Failed to generate payload."));
        return;
    }

    char* final_payload = NULL;
    if (shell_to_use && strstr(payload, "{{SHELL}}")) {
        size_t plen = strlen(payload);
        size_t slen = strlen(shell_to_use);
        size_t outlen = plen - 9 + slen + 1;
        final_payload = malloc(outlen);
        char* p = strstr(payload, "{{SHELL}}");
        if (p) {
            size_t before = p - payload;
            strncpy(final_payload, payload, before);
            strcpy(final_payload + before, shell_to_use);
            strcpy(final_payload + before + slen, p + 9);
        } else {
            strcpy(final_payload, payload);
        }
    } else {
        final_payload = strdup(payload);
    }
    char* encoded = NULL;
    if (!encoding || strcmp(encoding, "none") == 0) {
        encoded = strdup(final_payload);
    } else if (strcmp(encoding, "url") == 0) {
        encoded = encode_url(final_payload);
    } else if (strcmp(encoding, "double-url") == 0) {
        encoded = encode_double_url(final_payload);
    } else if (strcmp(encoding, "base64") == 0) {
        encoded = encode_base64(final_payload);
    } else if (strcmp(encoding, "base32") == 0) {
        encoded = encode_base32(final_payload);
    } else {
        printf("%s ", COLORIZE(COLOR_BOLD COLOR_RED, "Unknown encoding:"));
        printf("%s\n", encoding);
        free(payload);
        free(final_payload);
        return;
    }

    if (cmd && cmd->only_payload) {
        printf("%s\n", encoded);
    } else {
        printf("%s\n", COLORIZE(COLOR_BOLD COLOR_GREEN, "Generated payload:"));
        printf("%s\n", encoded);
    }

    free(payload);
    free(final_payload);
    free(encoded);

    if (mod->listener_count > 0) {
        if (cmd && cmd->only_listener) {
            char* listener = generate_listener(mod, mod->listeners[0].name, lport);
            if (listener) {
                printf("%s\n", listener);
                free(listener);
            }
        } else if (!(cmd && cmd->only_payload)) {
            printf("\n%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "Available listeners:"));
            for (int i = 0; i < mod->listener_count; i++) {
                char* listener = generate_listener(mod, mod->listeners[i].name, lport);
                if (listener) {
                    printf("  %s: %s\n", mod->listeners[i].name, listener);
                    free(listener);
                }
            }
        }
    }
}
