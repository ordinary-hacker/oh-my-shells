#include "../lib/unity/src/unity.h"
#include "../include/module_loader.h"
#include <string.h>

static ModuleRegistry *test_registry = NULL;

void setUp(void) {
    test_registry = registry_init("shells");
}

void tearDown(void) {
    if (test_registry) {
        registry_destroy(test_registry);
        test_registry = NULL;
    }
}

void test_registry_init(void) {
    TEST_ASSERT_NOT_NULL(test_registry);
    TEST_ASSERT_EQUAL_STRING("shells", test_registry->base_path);
    TEST_ASSERT_EQUAL_INT(0, test_registry->count);
}

void test_load_all_modules(void) {
    int result = load_all_modules(test_registry);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_GREATER_THAN(0, test_registry->count);
}

void test_find_module_by_id(void) {
    load_all_modules(test_registry);
    ShellModule *module = find_module_by_id(test_registry, "bash_i");
    if (module) {
        TEST_ASSERT_EQUAL_STRING("bash_i", module->id);
        TEST_ASSERT_NOT_NULL(module->payload);
    }
}

void test_find_nonexistent_module(void) {
    load_all_modules(test_registry);
    ShellModule *module = find_module_by_id(test_registry, "nonexistent_shell");
    TEST_ASSERT_NULL(module);
}

void test_module_structure(void) {
    load_all_modules(test_registry);
    if (test_registry->count > 0) {
        ShellModule *module = &test_registry->modules[0];
        TEST_ASSERT_NOT_NULL(module->id);
        TEST_ASSERT_NOT_NULL(module->name);
        TEST_ASSERT_NOT_NULL(module->os);
        TEST_ASSERT_NOT_NULL(module->type);
        TEST_ASSERT_NOT_NULL(module->protocol);
    }
}

int run_module_loader_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_registry_init);
    RUN_TEST(test_load_all_modules);
    RUN_TEST(test_find_module_by_id);
    RUN_TEST(test_find_nonexistent_module);
    RUN_TEST(test_module_structure);
    return UNITY_END();
}

int main(void) {
    return run_module_loader_tests();
}
