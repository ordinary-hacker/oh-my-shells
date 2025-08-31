#include <limits.h>

char *get_shells_dir_relative_to_exe(const char *exe_path);
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cli.h"
#include "../include/module_loader.h"
#include "../include/version.h"

void list_command(ModuleRegistry* registry, const char* os, const char* type, const char* proto, const char* lang);
void search_command(ModuleRegistry* registry, const char* term);
void show_command(ModuleRegistry* registry, const char* shell_id);
void generate_command(ModuleRegistry* registry, const char* shell_id, const char* lhost, int lport, const char* encoding, const char* shell_override, const Command* cmd);



int main(int argc, char **argv) {
    Command cmd;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--version") == 0) {
            printf("%s\n", OMS_VERSION);
            return 0;
        }
    }

    int ret = parse_cli(argc, argv, &cmd);
    if (ret != 0) {
        cleanup_command(&cmd);
        return (ret == 1) ? 0 : 1;
    }

    extern int no_color;
    no_color = cmd.no_color;


    char *shells_path = get_shells_dir_relative_to_exe(argv[0]);
    ModuleRegistry *registry = NULL;
    if (shells_path) {
        registry = registry_init(shells_path);
        free(shells_path);
    }
    if (!registry) {
        registry = registry_init("/opt/oh-my-shells/shells");
        if (!registry) {
            fprintf(stderr, "Failed to initialize module registry (tried executable-relative and /opt/oh-my-shells/shells)\n");
            cleanup_command(&cmd);
            return 1;
        }
    }
    if (load_all_modules(registry) != 0) {
        fprintf(stderr, "Failed to load modules\n");
        registry_destroy(registry);
        cleanup_command(&cmd);
        return 1;
    }

    if (strcmp(cmd.command, "list") == 0) {
        list_command(registry, cmd.os, cmd.type, cmd.proto, cmd.lang);
    } else if (strcmp(cmd.command, "search") == 0) {
        search_command(registry, cmd.term);
    } else if (strcmp(cmd.command, "show") == 0) {
        show_command(registry, cmd.shell_id);
    } else if (strcmp(cmd.command, "generate") == 0) {
    generate_command(registry, cmd.shell_id, cmd.lhost, cmd.lport, cmd.encoding, cmd.shell, &cmd);
    }

    registry_destroy(registry);
    cleanup_command(&cmd);
    return 0;
}
