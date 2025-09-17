#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE
#define _GNU_SOURCE
#define _BSD_SOURCE
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../include/module_loader.h"
#include "../lib/tomlc17/src/tomlc17.h"

#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

char *get_shells_dir_relative_to_exe(const char *exe_path) {
    char resolved[PATH_MAX];
    char *dir_end;
    if (!exe_path) return NULL;
    
#ifdef __NetBSD__
    // NetBSD fallback - use exe_path directly if realpath unavailable
    strncpy(resolved, exe_path, PATH_MAX - 1);
    resolved[PATH_MAX - 1] = '\0';
#else
    if (realpath(exe_path, resolved) == NULL) return NULL;
#endif
    
    dir_end = strrchr(resolved, '/');
    if (!dir_end) return NULL;
    *dir_end = '\0';

    char *shells_path = (char *)malloc(PATH_MAX);
    if (!shells_path) return NULL;
    snprintf(shells_path, PATH_MAX, "%s/shells", resolved);
    return shells_path;
}

ModuleRegistry* registry_init(const char* base_path) {
    ModuleRegistry* registry = malloc(sizeof(ModuleRegistry));
    if (!registry) return NULL;

    registry->count = 0;
    strncpy(registry->base_path, base_path, MAX_PATH_LEN - 1);
    registry->base_path[MAX_PATH_LEN - 1] = '\0';

    memset(registry->modules, 0, sizeof(registry->modules));

    return registry;
}

void registry_destroy(ModuleRegistry* registry) {
    if (!registry) return;

    for (int i = 0; i < registry->count; i++) {
        cleanup_module(&registry->modules[i]);
    }

    free(registry);
}

int load_all_modules(ModuleRegistry* registry) {
    if (!registry) return -1;

    registry->count = 0;
    return scan_directory_recursive(registry->base_path, registry);
}

int scan_directory_recursive(const char* path, ModuleRegistry* registry) {
    DIR* dir;
    struct dirent* entry;
    struct stat statbuf;
    char full_path[MAX_PATH_LEN];

    dir = opendir(path);
    if (!dir) return -1;

    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        snprintf(full_path, sizeof(full_path), "%s/%s", path, entry->d_name);

        if (stat(full_path, &statbuf) == -1) continue;

        if (S_ISDIR(statbuf.st_mode)) {
            scan_directory_recursive(full_path, registry);
        } else if (S_ISREG(statbuf.st_mode) &&
                   strstr(entry->d_name, ".toml") != NULL) {
            load_module_from_file(registry, full_path);
        }
    }

    closedir(dir);
    return 0;
}

int load_module_from_file(ModuleRegistry* registry, const char* filepath) {
    if (!registry || registry->count >= MAX_MODULES) return -1;

    ShellModule* module = &registry->modules[registry->count];

    if (parse_toml_module(filepath, module) == 0) {
        registry->count++;
        return 0;
    }

    return -1;
}

int parse_toml_module(const char* filepath, ShellModule* module) {
    toml_result_t result = toml_parse_file_ex(filepath);

    if (!result.ok) {
        fprintf(stderr, "TOML parse error in %s: %s\n", filepath, result.errmsg);
        return -1;
    }

    memset(module, 0, sizeof(ShellModule));
    module->is_webshell = 0;
    toml_datum_t webshell_datum = toml_seek(result.toptab, "is_webshell");
    if (webshell_datum.type == TOML_BOOLEAN) {
        module->is_webshell = webshell_datum.u.boolean ? 1 : 0;
    } else {
        module->is_webshell = 0;
    }

    toml_datum_t data;

    data = toml_seek(result.toptab, "id");
    if (data.type == TOML_STRING) {
        strncpy(module->id, data.u.s, sizeof(module->id) - 1);
        module->id[sizeof(module->id) - 1] = '\0';
    }

    data = toml_seek(result.toptab, "name");
    if (data.type == TOML_STRING) {
        strncpy(module->name, data.u.s, sizeof(module->name) - 1);
        module->name[sizeof(module->name) - 1] = '\0';
    }

    data = toml_seek(result.toptab, "os");
    if (data.type == TOML_STRING) {
        strncpy(module->os, data.u.s, sizeof(module->os) - 1);
        module->os[sizeof(module->os) - 1] = '\0';
    }

    data = toml_seek(result.toptab, "type");
    if (data.type == TOML_STRING) {
        strncpy(module->type, data.u.s, sizeof(module->type) - 1);
        module->type[sizeof(module->type) - 1] = '\0';
    }


    data = toml_seek(result.toptab, "proto");
    if (data.type == TOML_STRING) {
        strncpy(module->protocol, data.u.s, sizeof(module->protocol) - 1);
        module->protocol[sizeof(module->protocol) - 1] = '\0';
    } else {
        module->protocol[0] = '\0';
    }

    data = toml_seek(result.toptab, "lang");
    if (data.type == TOML_STRING) {
        strncpy(module->lang, data.u.s, sizeof(module->lang) - 1);
        module->lang[sizeof(module->lang) - 1] = '\0';
    } else {
        module->lang[0] = '\0';
    }

    data = toml_seek(result.toptab, "description");
    if (data.type == TOML_STRING) {
        strncpy(module->description, data.u.s, sizeof(module->description) - 1);
        module->description[sizeof(module->description) - 1] = '\0';
    }

    data = toml_seek(result.toptab, "payload");
    if (data.type == TOML_STRING) {
        strncpy(module->payload, data.u.s, sizeof(module->payload) - 1);
        module->payload[sizeof(module->payload) - 1] = '\0';
    }

    module->listener_count = 0;

    toml_datum_t listeners_datum = toml_seek(result.toptab, "listeners");
    if (listeners_datum.type == TOML_TABLE) {
        int n = listeners_datum.u.tab.size;
        for (int i = 0; i < n && module->listener_count < MAX_LISTENERS; i++) {
            const char* key = listeners_datum.u.tab.key[i];
            toml_datum_t val = listeners_datum.u.tab.value[i];
            if (val.type == TOML_STRING) {
                module->listeners[module->listener_count].name = strdup(key);
                module->listeners[module->listener_count].value = strdup(val.u.s);
                module->listener_count++;
            }
        }
    }

    module->compatible_shells = NULL;
    module->compatible_shell_count = 0;
    module->default_shell[0] = '\0';
    toml_datum_t shells_datum = toml_get(result.toptab, "shells");
    if (shells_datum.type == TOML_TABLE) {
        toml_datum_t compat = toml_get(shells_datum, "compatible");
        if (compat.type == TOML_ARRAY && compat.u.arr.size > 0) {
            int n = compat.u.arr.size;
            module->compatible_shells = malloc(sizeof(char*) * n);
            module->compatible_shell_count = n;
            for (int i = 0; i < n; i++) {
                toml_datum_t elem = compat.u.arr.elem[i];
                if (elem.type == TOML_STRING) {
                    module->compatible_shells[i] = strdup(elem.u.s);
                } else {
                    module->compatible_shells[i] = NULL;
                }
            }
        }
        toml_datum_t def = toml_get(shells_datum, "default");
        if (def.type == TOML_STRING) {
            strncpy(module->default_shell, def.u.s, sizeof(module->default_shell)-1);
            module->default_shell[sizeof(module->default_shell)-1] = '\0';
        }
    }

    toml_free(result);
    return 0;
}

ShellModule* find_module_by_id(ModuleRegistry* registry, const char* id) {
    if (!registry || !id) return NULL;

    for (int i = 0; i < registry->count; i++) {
        if (strcmp(registry->modules[i].id, id) == 0) {
            return &registry->modules[i];
        }
    }

    return NULL;
}

ShellModule* find_module_by_path(ModuleRegistry* registry, const char* os,
                                const char* type, const char* protocol,
                                const char* name) {
    if (!registry) return NULL;

    for (int i = 0; i < registry->count; i++) {
        ShellModule* mod = &registry->modules[i];

        if ((os && strcmp(mod->os, os) != 0) ||
            (type && strcmp(mod->type, type) != 0) ||
            (protocol && strcmp(mod->protocol, protocol) != 0) ||
            (name && strcmp(mod->id, name) != 0)) {
            continue;
        }

        return mod;
    }

    return NULL;
}

void list_modules(ModuleRegistry* registry, const char* filter_os,
                 const char* filter_type) {
    if (!registry) return;

    printf("Available Modules:\n");
    printf("==================\n\n");

    for (int i = 0; i < registry->count; i++) {
        ShellModule* mod = &registry->modules[i];

        if (filter_os && strcmp(mod->os, "all") != 0 && strcmp(mod->os, filter_os) != 0) continue;
        if (filter_type && strcmp(mod->type, filter_type) != 0) continue;

        printf("[%s] %s/%s/%s\n", mod->id, mod->os, mod->type, mod->protocol);
        printf("  Name: %s\n", mod->name);
        printf("  Description: %s\n", mod->description);
        printf("\n");
    }
}

void print_module_info(const ShellModule* module) {
    if (!module) return;

    printf("Module Information:\n");
    printf("==================\n");
    printf("ID: %s\n", module->id);
    printf("Name: %s\n", module->name);
    printf("OS: %s\n", module->os);
    printf("Type: %s\n", module->type);
    printf("Protocol: %s\n", module->protocol[0] ? module->protocol : "(none)");
    printf("Language: %s\n", module->lang[0] ? module->lang : "(none)");
    printf("Description: %s\n\n", module->description);

    printf("Payload:\n%s\n\n", module->payload);

    printf("Available Listeners:\n");
    for (int i = 0; i < module->listener_count; i++) {
        printf("  %s: %s\n", module->listeners[i].name, module->listeners[i].value);
    }
}

char* generate_payload(const ShellModule* module, const char* lhost, int lport) {
    if (!module || !lhost) return NULL;

    char* result = malloc(MAX_STRING_LEN);
    if (!result) return NULL;

    strncpy(result, module->payload, MAX_STRING_LEN - 1);
    result[MAX_STRING_LEN - 1] = '\0';

    char lhost_placeholder[] = "{{LHOST}}";
    char lport_placeholder[] = "{{LPORT}}";
    char lport_str[16];
    snprintf(lport_str, sizeof(lport_str), "%d", lport);

    char* pos = strstr(result, lhost_placeholder);
    if (pos) {
        char temp[MAX_STRING_LEN];
        size_t prefix_len = pos - result;
        size_t suffix_len = strlen(pos + strlen(lhost_placeholder));

        strncpy(temp, result, prefix_len);
        temp[prefix_len] = '\0';
        strcat(temp, lhost);
        strcat(temp, pos + strlen(lhost_placeholder));
        strcpy(result, temp);
    }

    pos = strstr(result, lport_placeholder);
    if (pos) {
        char temp[MAX_STRING_LEN];
        size_t prefix_len = pos - result;

        strncpy(temp, result, prefix_len);
        temp[prefix_len] = '\0';
        strcat(temp, lport_str);
        strcat(temp, pos + strlen(lport_placeholder));
        strcpy(result, temp);
    }

    return result;
}

char* generate_listener(const ShellModule* module, const char* listener_type,
                       int lport) {
    if (!module || !listener_type) return NULL;

    for (int i = 0; i < module->listener_count; i++) {
        if (strcmp(module->listeners[i].name, listener_type) == 0) {
            char* result = malloc(MAX_STRING_LEN);
            if (!result) return NULL;

            strncpy(result, module->listeners[i].value, MAX_STRING_LEN - 1);
            result[MAX_STRING_LEN - 1] = '\0';

            char lport_placeholder[] = "{{LPORT}}";
            char lport_str[16];
            snprintf(lport_str, sizeof(lport_str), "%d", lport);

            char* pos = strstr(result, lport_placeholder);
            if (pos) {
                char temp[MAX_STRING_LEN];
                size_t prefix_len = pos - result;

                strncpy(temp, result, prefix_len);
                temp[prefix_len] = '\0';
                strcat(temp, lport_str);
                strcat(temp, pos + strlen(lport_placeholder));
                strcpy(result, temp);
            }

            return result;
        }
    }

    return NULL;
}

void cleanup_module(ShellModule* module) {
    if (!module) return;

    for (int i = 0; i < module->listener_count; i++) {
        free(module->listeners[i].name);
        free(module->listeners[i].value);
    }
    if (module->compatible_shells) {
        for (int i = 0; i < module->compatible_shell_count; i++) {
            if (module->compatible_shells[i]) free(module->compatible_shells[i]);
        }
        free(module->compatible_shells);
        module->compatible_shells = NULL;
        module->compatible_shell_count = 0;
    }
}

char* extract_module_path_info(const char* filepath, char* os, char* type, char* protocol) {

    char* path_copy = strdup(filepath);
    char* token;
    char* parts[10];
    int part_count = 0;

    token = strtok(path_copy, "/");
    while (token && part_count < 10) {
        parts[part_count++] = token;
        token = strtok(NULL, "/");
    }

    if (part_count >= 4) {
        if (os) strcpy(os, parts[1]);
        if (type) strcpy(type, parts[2]);
        if (protocol) strcpy(protocol, parts[3]);
    }

    free(path_copy);
    return NULL;
}
