#include "../lib/unity/src/unity.h"
#include "../include/cli.h"
#include "../include/module_loader.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void setUp(void) {}
void tearDown(void) {}

void test_full_workflow_list(void) {
    // Test the complete list workflow
    ModuleRegistry *registry = registry_init("shells");
    TEST_ASSERT_NOT_NULL(registry);

    int result = load_all_modules(registry);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_GREATER_THAN(0, registry->count);

    registry_destroy(registry);
}

void test_full_workflow_search(void) {
    ModuleRegistry *registry = registry_init("shells");
    TEST_ASSERT_NOT_NULL(registry);

    load_all_modules(registry);

    // Search for bash shells
    int found_count = 0;
    for (int i = 0; i < registry->count; i++) {
        if (strstr(registry->modules[i].id, "bash") ||
            strstr(registry->modules[i].name, "bash") ||
            strstr(registry->modules[i].description, "bash")) {
            found_count++;
        }
    }
    TEST_ASSERT_GREATER_THAN(0, found_count);

    registry_destroy(registry);
}

void test_full_workflow_show(void) {
    ModuleRegistry *registry = registry_init("shells");
    TEST_ASSERT_NOT_NULL(registry);

    load_all_modules(registry);

    if (registry->count > 0) {
        ShellModule *module = &registry->modules[0];
        TEST_ASSERT_NOT_NULL(module->id);
        TEST_ASSERT_NOT_NULL(module->payload);
    }

    registry_destroy(registry);
}

void test_payload_generation(void) {
    ModuleRegistry *registry = registry_init("shells");
    TEST_ASSERT_NOT_NULL(registry);

    load_all_modules(registry);

    ShellModule *module = find_module_by_id(registry, "bash_i");
    if (module) {
        char *payload = generate_payload(module, "192.168.1.100", 4444);
        if (payload) {
            TEST_ASSERT_NOT_NULL(payload);
            TEST_ASSERT_TRUE(strlen(payload) > 0);
            free(payload);
        }
    }

    registry_destroy(registry);
}

void test_command_line_parsing_integration(void) {
    Command cmd;

    // Test generate command parsing
    char *argv[] = {"oh-my-shells", "generate", "bash_i", "-H", "10.10.10.10", "-P", "5555"};
    int result = parse_cli(7, argv, &cmd);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_STRING("generate", cmd.command);
    TEST_ASSERT_EQUAL_STRING("bash_i", cmd.shell_id);
    TEST_ASSERT_EQUAL_STRING("10.10.10.10", cmd.lhost);
    TEST_ASSERT_EQUAL_INT(5555, cmd.lport);

    cleanup_command(&cmd);
}

void test_filter_functionality(void) {
    ModuleRegistry *registry = registry_init("shells");
    TEST_ASSERT_NOT_NULL(registry);

    load_all_modules(registry);

    // Count modules with specific filters
    int unix_count = 0, reverse_count = 0, tcp_count = 0;

    for (int i = 0; i < registry->count; i++) {
        if (strcmp(registry->modules[i].os, "unix") == 0) unix_count++;
        if (strcmp(registry->modules[i].type, "reverse") == 0) reverse_count++;
        if (strcmp(registry->modules[i].protocol, "tcp") == 0) tcp_count++;
    }

    TEST_ASSERT_GREATER_THAN(0, unix_count);
    TEST_ASSERT_GREATER_THAN(0, reverse_count);
    TEST_ASSERT_GREATER_THAN(0, tcp_count);

    registry_destroy(registry);
}

int run_integration_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_full_workflow_list);
    RUN_TEST(test_full_workflow_search);
    RUN_TEST(test_full_workflow_show);
    RUN_TEST(test_payload_generation);
    RUN_TEST(test_command_line_parsing_integration);
    RUN_TEST(test_filter_functionality);
    return UNITY_END();
}

int main(void) {
    return run_integration_tests();
}
