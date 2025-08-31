#include "../include/module_loader.h"
#include "../include/colors.h"

void search_command(ModuleRegistry* registry, const char* term) {
    printf("%s '%s':\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "Search results for"), term);
    printf("%s\n\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "============================"));
    for (int i = 0; i < registry->count; i++) {
        ShellModule* mod = &registry->modules[i];
        if ((strstr(mod->id, term) || strstr(mod->name, term) || strstr(mod->description, term))) {
            printf("[%s] %s/%s/%s\n", mod->id, mod->os, mod->type, mod->protocol);
            printf("  %s %s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "Name:"), mod->name);
            printf("  %s %s\n\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "Description:"), mod->description);
        }
    }
}
