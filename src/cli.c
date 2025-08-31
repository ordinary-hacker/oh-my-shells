#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "../include/cli.h"
#include "../include/colors.h"

int no_color = 0;

void init_command(Command *cmd) {
    cmd->command = NULL;
    cmd->shell_id = NULL;
    cmd->term = NULL;
    cmd->lhost = NULL;
    cmd->lport = 4444;
    cmd->encoding = NULL;
    cmd->shell = NULL;
    cmd->no_color = 0;
    cmd->only_listener = 0;
    cmd->only_payload = 0;
    cmd->os = NULL;
    cmd->type = NULL;
    cmd->proto = NULL;
    cmd->lang = NULL;
}

void print_help(const char *program_name) {
    printf("%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "NAME"));
    printf("    oh-my-shells - The offline and CLI-first swiss army knife for shell payloads\n\n");

    printf("%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "SYNOPSIS"));
    printf("    %s <command> [OPTIONS]\n\n", program_name);

    printf("%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "COMMANDS"));
    printf("  %s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "list"));
    printf("      List payloads with optional filters\n");
    printf("  %s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "search <term>"));
    printf("      Search for payloads\n");
    printf("  %s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "show <shell_id>"));
    printf("      Show details about a specific payload\n");
    printf("  %s\n", COLORIZE(COLOR_BOLD COLOR_YELLOW, "generate <shell_id>"));
    printf("      Generate a payload\n\n");

    printf("%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "OPTIONS"));
    printf("  -i, --id <shell_id>        Shell identifier\n");
    printf("  -s, --search <term>        Search term\n");
    printf("  -H, --lhost <host>         Local host for payload generation\n");
    printf("  -P, --lport <port>         Local port for payload generation (default: 4444)\n");
    printf("  -S, --shell <shell>        Shell to use for payloads (must be compatible)\n");
    printf("  -e, --encoding <type>      Output encoding (url, double-url, base64, base32, none)\n");
    printf("  -l, --listener             Output only the recommended listener command for the payload\n");
    printf("  -p, --payload              Output only the generated payload for scripting\n");
    printf("      --no-color       Disable ANSI color output\n");

    printf("\n%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "FILTERS (for list command)"));
    printf("  --os <os>                  Filter by OS (bsd, linux, osx, unix, windows)\n");
    printf("  --type <type>              Filter by type (reverse, bind)\n");
    printf("  --proto <proto>            Filter by protocol (tcp, udp, http)\n");
    printf("  --lang <lang>              Filter by language (ruby, go, powershell, python, etc.)\n");

    printf("\n%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "GENERAL"));
    printf("  -h, --help                 Show this help message\n");
    printf("      --version              Show version information\n");

    printf("\n%s\n", COLORIZE(COLOR_BOLD COLOR_CYAN, "EXAMPLES"));
    printf("  %s list --os linux --type reverse\n", program_name);
    printf("  %s search \"netcat\"\n", program_name);
    printf("  %s show bash_read_lines\n", program_name);
    printf("  %s generate bash_i -H 192.168.1.100 -P 4444\n", program_name);
}

int is_valid_command(const char *cmd) {
    return (strcmp(cmd, "list") == 0 ||
            strcmp(cmd, "search") == 0 ||
            strcmp(cmd, "show") == 0 ||
            strcmp(cmd, "generate") == 0);
}

int parse_cli(int argc, char **argv, Command *cmd) {
    static struct option long_options[] = {
        {"id",        required_argument, 0, 'i'},
        {"search",    required_argument, 0, 's'},
        {"lhost",     required_argument, 0, 'H'},
        {"lport",     required_argument, 0, 'P'},
        {"shell",     required_argument, 0, 'S'},
        {"os",        required_argument, 0, 'O'},
        {"type",      required_argument, 0, 'T'},
        {"proto",     required_argument, 0, 'p'},
        {"lang",      required_argument, 0, 'L'},
        {"encoding",  required_argument, 0, 'e'},
        {"no-color",  no_argument,       0, 1000},
        {"listener",  no_argument,       0, 'l'},
        {"payload",   no_argument,       0, 'p'},
        {"help",      no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    int c;
    int option_index = 0;

    init_command(cmd);

    if (argc < 2) {
        fprintf(stderr, "Error: No command specified\n");
        print_help(argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
        print_help(argv[0]);
        return 1;
    }

    if (!is_valid_command(argv[1])) {
        fprintf(stderr, "Error: Invalid command '%s'\n", argv[1]);
        print_help(argv[0]);
        return -1;
    }

    cmd->command = strdup(argv[1]);

    if ((strcmp(cmd->command, "search") == 0 ||
         strcmp(cmd->command, "show") == 0 ||
         strcmp(cmd->command, "generate") == 0) && argc >= 3 && argv[2][0] != '-') {

        if (strcmp(cmd->command, "search") == 0) {
            cmd->term = strdup(argv[2]);
        } else {
            cmd->shell_id = strdup(argv[2]);
        }

        optind = 3;
    } else {
        optind = 2;
    }

    while ((c = getopt_long(argc, argv, "i:s:H:P:S:O:T:p:L:e:hl", long_options, &option_index)) != -1) {
        switch (c) {
            case 'S':
                if (cmd->shell) free(cmd->shell);
                cmd->shell = strdup(optarg);
                break;
            case 'i':
                if (cmd->shell_id) free(cmd->shell_id);
                cmd->shell_id = strdup(optarg);
                break;
            case 's':
                if (cmd->term) free(cmd->term);
                cmd->term = strdup(optarg);
                break;
            case 'H':
                if (cmd->lhost) free(cmd->lhost);
                cmd->lhost = strdup(optarg);
                break;
            case 'P':
                cmd->lport = atoi(optarg);
                if (cmd->lport <= 0 || cmd->lport > 65535) {
                    fprintf(stderr, "Error: Invalid port number\n");
                    return -1;
                }
                break;
            case 'O':
                if (cmd->os) free(cmd->os);
                cmd->os = strdup(optarg);
                break;
            case 'T':
                if (cmd->type) free(cmd->type);
                cmd->type = strdup(optarg);
                break;
            case 'p':
                if (optarg) {
                    if (cmd->proto) free(cmd->proto);
                    cmd->proto = strdup(optarg);
                } else {
                    cmd->only_payload = 1;
                }
                break;
            case 'L':
                if (cmd->lang) free(cmd->lang);
                cmd->lang = strdup(optarg);
                break;
            case 'e':
                if (cmd->encoding) free(cmd->encoding);
                cmd->encoding = strdup(optarg);
                break;
            case 'l':
                cmd->only_listener = 1;
                break;
            case 1000:
                cmd->no_color = 1;
                break;
            case 'h':
                print_help(argv[0]);
                return 1;
            case '?':
                return -1;
            default:
                return -1;
        }
    }

    if (strcmp(cmd->command, "generate") == 0) {
        if (!cmd->shell_id) {
            fprintf(stderr, "Error: 'generate' command requires a shell_id\n");
            return -1;
        }
        if (!cmd->lhost) {
            fprintf(stderr, "Error: 'generate' command requires --lhost\n");
            return -1;
        }
    } else if (strcmp(cmd->command, "show") == 0) {
        if (!cmd->shell_id) {
            fprintf(stderr, "Error: 'show' command requires a shell_id\n");
            return -1;
        }
    } else if (strcmp(cmd->command, "search") == 0) {
        if (!cmd->term) {
            fprintf(stderr, "Error: 'search' command requires a search term\n");
            return -1;
        }
    }

    return 0;
}

void cleanup_command(Command *cmd) {
    if (cmd->command) { free(cmd->command); cmd->command = NULL; }
    if (cmd->shell_id) { free(cmd->shell_id); cmd->shell_id = NULL; }
    if (cmd->term) { free(cmd->term); cmd->term = NULL; }
    if (cmd->lhost) { free(cmd->lhost); cmd->lhost = NULL; }
    if (cmd->shell) { free(cmd->shell); cmd->shell = NULL; }
    if (cmd->os) { free(cmd->os); cmd->os = NULL; }
    if (cmd->type) { free(cmd->type); cmd->type = NULL; }
    if (cmd->proto) { free(cmd->proto); cmd->proto = NULL; }
    if (cmd->lang) { free(cmd->lang); cmd->lang = NULL; }
    if (cmd->encoding) { free(cmd->encoding); cmd->encoding = NULL; }
}
