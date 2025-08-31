#include "../include/module_loader.h"
#include "../include/colors.h"

void show_command(ModuleRegistry* registry, const char* shell_id) {
    ShellModule* mod = find_module_by_id(registry, shell_id);
    if (!mod) {
        printf("%s\n", COLORIZE(COLOR_BOLD COLOR_RED, "Shell not found:"));
        printf("%s\n", shell_id);
        return;
    }
    print_module_info(mod);

    if (mod->compatible_shells && mod->compatible_shell_count > 0) {
        printf("\n%s ", COLORIZE(COLOR_BOLD COLOR_CYAN, "Compatible shells:"));
        for (int i = 0; i < mod->compatible_shell_count; i++) {
            printf("%s%s", mod->compatible_shells[i], (i < mod->compatible_shell_count-1) ? ", " : "\n");
        }
        printf("%s %s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "Default shell:"), mod->default_shell[0] ? mod->default_shell : "(none)");
    }
}
