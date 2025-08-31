#ifndef GENERATE_H
#define GENERATE_H

#include "module_loader.h"

#include "cli.h"

void generate_command(ModuleRegistry* registry, const char* shell_id, const char* lhost, int lport, const char* encoding, const char* shell_override, const Command* cmd);

char* encode_url(const char* input);
char* encode_double_url(const char* input);
char* encode_base64(const char* input);
char* encode_base32(const char* input);

#endif /* GENERATE_H */
