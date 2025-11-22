#include "../lib/unity/src/unity.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

void setUp(void) {}
void tearDown(void) {}

// Helper function to execute command and capture output
int execute_command(const char *cmd, char *output, size_t output_size) {
    FILE *fp = popen(cmd, "r");
    if (!fp) return -1;

    size_t bytes_read = fread(output, 1, output_size - 1, fp);
    output[bytes_read] = '\0';

    int status = pclose(fp);
    return WEXITSTATUS(status);
}

void test_binary_exists(void) {
    int result = access("./oh-my-shells", X_OK);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_help_command(void) {
    char output[4096];
    int result = execute_command("./oh-my-shells --help", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strstr(output, "oh-my-shells") != NULL);
    TEST_ASSERT_TRUE(strstr(output, "SYNOPSIS") != NULL);
}

void test_version_command(void) {
    char output[256];
    int result = execute_command("./oh-my-shells --version", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
}

void test_list_command(void) {
    char output[8192];
    int result = execute_command("./oh-my-shells list", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
}

void test_list_with_filters(void) {
    char output[4096];
    int result = execute_command("./oh-my-shells list --os unix --type reverse", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
}

void test_search_command(void) {
    char output[4096];
    int result = execute_command("./oh-my-shells search bash", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
}

void test_show_command(void) {
    char output[4096];
    int result = execute_command("./oh-my-shells show bash_i", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
}

void test_generate_command(void) {
    char output[4096];
    int result = execute_command("./oh-my-shells generate bash_i -H 192.168.1.100 -P 4444", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
    TEST_ASSERT_TRUE(strstr(output, "192.168.1.100") != NULL);
    TEST_ASSERT_TRUE(strstr(output, "4444") != NULL);
}

void test_generate_payload_only(void) {
    char output[4096];
    int result = execute_command("./oh-my-shells generate bash_i -H 10.10.10.10 -P 5555 -p", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
    TEST_ASSERT_TRUE(strstr(output, "10.10.10.10") != NULL);
}

void test_generate_listener_only(void) {
    char output[4096];
    int result = execute_command("./oh-my-shells generate bash_i -H 10.10.10.10 -P 5555 -l", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
}

void test_invalid_command(void) {
    char output[1024];
    int result = execute_command("./oh-my-shells invalid_command", output, sizeof(output));
    TEST_ASSERT_NOT_EQUAL(0, result);
}

void test_missing_required_args(void) {
    char output[1024];
    int result = execute_command("./oh-my-shells generate", output, sizeof(output));
    TEST_ASSERT_NOT_EQUAL(0, result);
}

void test_no_color_flag(void) {
    char output[4096];
    int result = execute_command("./oh-my-shells list --no-color", output, sizeof(output));
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_TRUE(strlen(output) > 0);
}

int run_blackbox_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_binary_exists);
    RUN_TEST(test_help_command);
    RUN_TEST(test_version_command);
    RUN_TEST(test_list_command);
    RUN_TEST(test_list_with_filters);
    RUN_TEST(test_search_command);
    RUN_TEST(test_show_command);
    RUN_TEST(test_generate_command);
    RUN_TEST(test_generate_payload_only);
    RUN_TEST(test_generate_listener_only);
    RUN_TEST(test_invalid_command);
    RUN_TEST(test_missing_required_args);
    RUN_TEST(test_no_color_flag);
    return UNITY_END();
}

int main(void) {
    return run_blackbox_tests();
}
