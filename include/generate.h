#ifndef GENERATE_H
#define GENERATE_H

#include "module_loader.h"

#include "cli.h"

void generate_command(ModuleRegistry* registry, const char* shell_id, const char* lhost, int lport, const char* encoding, const char* shell_override, const Command* cmd);

char* encode_url(const char* input);
char* encode_double_url(const char* input);
char* encode_base64(const char* input);
char* encode_base32(const char* input);

// LHOST helpers
int is_valid_ip(const char* str);
int is_valid_domain(const char* str);
int is_valid_interface(const char* str);
char* get_ip_from_interface(const char* ifname);

#endif /* GENERATE_H */
