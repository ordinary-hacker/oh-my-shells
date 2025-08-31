#ifndef CLI_H
#define CLI_H

typedef struct {
    char *command;
    char *shell_id;
    char *term;

    char *lhost;
    int lport;
    char *encoding;
    char *shell;

    int no_color;
    int only_listener;
    int only_payload;

    char *os;
    char *type;
    char *proto;
    char *lang;
} Command;

void init_command(Command *cmd);

int parse_cli(int argc, char **argv, Command *cmd);

void print_help(const char *program_name);

void cleanup_command(Command *cmd);

#endif /* CLI_H */
