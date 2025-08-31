#include "../include/module_loader.h"
#include "../include/colors.h"

void list_command(ModuleRegistry* registry, const char* os, const char* type, const char* proto, const char* lang) {
    printf("%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "Available Shells:"));
    printf("%s\n\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "=================="));
    for (int i = 0; i < registry->count; i++) {
        ShellModule* mod = &registry->modules[i];
        if (os && strcmp(mod->os, os) != 0) continue;
        if (type && strcmp(mod->type, type) != 0) continue;
        if (proto && (!mod->protocol[0] || strcmp(mod->protocol, proto) != 0)) continue;
        if (lang && (!mod->lang[0] || strcmp(mod->lang, lang) != 0)) continue;
            char id_col[256];
            char proto_col[256];
            if (no_color) {
                snprintf(id_col, sizeof(id_col), "%s", mod->id);
                snprintf(proto_col, sizeof(proto_col), "%s", mod->protocol[0] ? mod->protocol : "(none)");
            } else {
                snprintf(id_col, sizeof(id_col), "%s%s%s", COLOR_BOLD COLOR_GREEN, mod->id, COLOR_RESET);
                if (mod->protocol[0])
                    snprintf(proto_col, sizeof(proto_col), "%s%s%s", COLOR_CYAN, mod->protocol, COLOR_RESET);
                else
                    snprintf(proto_col, sizeof(proto_col), "(none)");
            }
            printf("[%s] %s/%s/%s\n", id_col, mod->os, mod->type, proto_col);

    printf("  %s %s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "Name:"), mod->name);
    printf("  %s %s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "Language:"), mod->lang[0] ? mod->lang : "(none)");
    printf("  %s %s\n\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "Description:"), mod->description);
    }
}
