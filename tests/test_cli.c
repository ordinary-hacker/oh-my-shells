#include "../lib/unity/src/unity.h"
#include "../include/cli.h"
#include <string.h>

void setUp(void) {}
void tearDown(void) {}

void test_init_command(void) {
    Command cmd;
    init_command(&cmd);

    TEST_ASSERT_NULL(cmd.command);
    TEST_ASSERT_NULL(cmd.shell_id);
    TEST_ASSERT_NULL(cmd.term);
    TEST_ASSERT_NULL(cmd.lhost);
    TEST_ASSERT_EQUAL_INT(4444, cmd.lport);
    TEST_ASSERT_NULL(cmd.encoding);
    TEST_ASSERT_NULL(cmd.shell);
    TEST_ASSERT_EQUAL_INT(0, cmd.no_color);
    TEST_ASSERT_EQUAL_INT(0, cmd.only_listener);
    TEST_ASSERT_EQUAL_INT(0, cmd.only_payload);
}

void test_parse_cli_help(void) {
    Command cmd;
    char *argv[] = {"oh-my-shells", "--help"};
    int result = parse_cli(2, argv, &cmd);
    TEST_ASSERT_EQUAL_INT(1, result);
    cleanup_command(&cmd);
}

void test_parse_cli_no_args(void) {
    Command cmd;
    char *argv[] = {"oh-my-shells"};
    int result = parse_cli(1, argv, &cmd);
    TEST_ASSERT_EQUAL_INT(-1, result);
    cleanup_command(&cmd);
}

void test_parse_cli_list_command(void) {
    Command cmd;
    char *argv[] = {"oh-my-shells", "list", "--os", "linux", "--type", "reverse"};
    int result = parse_cli(6, argv, &cmd);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_STRING("list", cmd.command);
    TEST_ASSERT_EQUAL_STRING("linux", cmd.os);
    TEST_ASSERT_EQUAL_STRING("reverse", cmd.type);
    cleanup_command(&cmd);
}

void test_parse_cli_generate_command(void) {
    Command cmd;
    char *argv[] = {"oh-my-shells", "generate", "bash_i", "-H", "192.168.1.1", "-P", "4444"};
    int result = parse_cli(7, argv, &cmd);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_STRING("generate", cmd.command);
    TEST_ASSERT_EQUAL_STRING("bash_i", cmd.shell_id);
    TEST_ASSERT_EQUAL_STRING("192.168.1.1", cmd.lhost);
    TEST_ASSERT_EQUAL_INT(4444, cmd.lport);
    cleanup_command(&cmd);
}

void test_parse_cli_search_command(void) {
    Command cmd;
    char *argv[] = {"oh-my-shells", "search", "netcat"};
    int result = parse_cli(3, argv, &cmd);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_STRING("search", cmd.command);
    TEST_ASSERT_EQUAL_STRING("netcat", cmd.term);
    cleanup_command(&cmd);
}

void test_parse_cli_aliases(void) {
    Command cmd;
    char *argv[] = {"oh-my-shells", "l"};
    int result = parse_cli(2, argv, &cmd);
    TEST_ASSERT_EQUAL_INT(0, result);
    TEST_ASSERT_EQUAL_STRING("list", cmd.command);
    cleanup_command(&cmd);
}

void test_parse_cli_invalid_port(void) {
    Command cmd;
    char *argv[] = {"oh-my-shells", "generate", "bash_i", "-P", "99999"};
    int result = parse_cli(5, argv, &cmd);
    TEST_ASSERT_EQUAL_INT(-1, result);
    cleanup_command(&cmd);
}

int run_cli_tests(void) {
    UNITY_BEGIN();
    RUN_TEST(test_init_command);
    RUN_TEST(test_parse_cli_help);
    RUN_TEST(test_parse_cli_no_args);
    RUN_TEST(test_parse_cli_list_command);
    RUN_TEST(test_parse_cli_generate_command);
    RUN_TEST(test_parse_cli_search_command);
    RUN_TEST(test_parse_cli_aliases);
    RUN_TEST(test_parse_cli_invalid_port);
    return UNITY_END();
}

int main(void) {
    return run_cli_tests();
}
