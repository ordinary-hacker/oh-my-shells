#ifndef MODULE_LOADER_H
#define MODULE_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define MAX_PATH_LEN 512
#define MAX_STRING_LEN 1024
#define MAX_LISTENERS 32
#define MAX_MODULES 256

typedef struct {
    char *name;
    char *value;
} KeyValue;

typedef struct {
    char id[64];
    char name[256];
    char os[32];
    char type[32];
    char protocol[16];
    char lang[32];
    char description[512];
    char payload[MAX_STRING_LEN];

    KeyValue listeners[MAX_LISTENERS];
    int listener_count;

    char **compatible_shells;
    int compatible_shell_count;
    char default_shell[128];
} ShellModule;

typedef struct {
    ShellModule modules[MAX_MODULES];
    int count;
    char base_path[MAX_PATH_LEN];
} ModuleRegistry;

ModuleRegistry* registry_init(const char* base_path);
void registry_destroy(ModuleRegistry* registry);

int load_all_modules(ModuleRegistry* registry);
int load_module_from_file(ModuleRegistry* registry, const char* filepath);
int parse_toml_module(const char* filepath, ShellModule* module);

ShellModule* find_module_by_id(ModuleRegistry* registry, const char* id);
ShellModule* find_module_by_path(ModuleRegistry* registry, const char* os,
                                const char* type, const char* protocol,
                                const char* name);

void list_modules(ModuleRegistry* registry, const char* filter_os,
                 const char* filter_type);
void print_module_info(const ShellModule* module);

char* generate_payload(const ShellModule* module, const char* lhost,
                      int lport);
char* generate_listener(const ShellModule* module, const char* listener_type,
                       int lport);

int scan_directory_recursive(const char* path, ModuleRegistry* registry);
char* extract_module_path_info(const char* filepath, char* os, char* type,
                              char* protocol);
void cleanup_module(ShellModule* module);

#endif /* MODULE_LOADER_H */
