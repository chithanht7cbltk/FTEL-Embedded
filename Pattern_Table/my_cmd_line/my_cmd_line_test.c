/**
 ******************************************************************************
 * @file    my_cmd_line_test.c
 * @author  Tobi
 * @date    04/04/2026
 * @brief   Unit Test for custom cmd_line implementation
 ******************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include "my_cmd_line.h"

/* Test counters */
static int g_test_help_count    = 0;
static int g_test_reset_count   = 0;
static int g_test_status_count  = 0;
static int g_test_set_count     = 0;
static int g_test_get_count_val = 0;

/* Test command implementations */
static int32_t cmd_help(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    g_test_help_count++;
    printf("[CMD] help executed (count: %d)\n", g_test_help_count);
    return 0;
}

static int32_t cmd_reset(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    g_test_reset_count++;
    printf("[CMD] reset executed (count: %d)\n", g_test_reset_count);
    return 0;
}

static int32_t cmd_status(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    g_test_status_count++;
    printf("[CMD] status executed (count: %d)\n", g_test_status_count);
    return 0;
}

static int32_t cmd_set(int argc, char* argv[]) {
    g_test_set_count++;
    printf("[CMD] set executed (count: %d, argc: %d)\n", g_test_set_count, argc);
    for (int i = 0; i < argc; i++) {
        printf("  arg[%d]: %s\n", i, argv[i]);
    }
    return 0;
}

static int32_t cmd_get(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    g_test_get_count_val++;
    printf("[CMD] get executed (count: %d)\n", g_test_get_count_val);
    return 0;
}

static int32_t cmd_fail(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    printf("[CMD] fail executed (intentional failure)\n");
    return -1;  /* Simulate failure */
}

/* Command table */
static my_cmd_entry_t test_cmd_table[] = {
    {"help",   cmd_help,   "Show help information"},
    {"reset",  cmd_reset,  "Reset the system"},
    {"status", cmd_status, "Show system status"},
    {"set",    cmd_set,    "Set a value (set <key> <value>)"},
    {"get",    cmd_get,    "Get a value (get <key>)"},
    {"fail",   cmd_fail,   "Command that always fails"},
};

#define TEST_CMD_TABLE_SIZE (sizeof(test_cmd_table) / sizeof(my_cmd_entry_t))

/* Test result tracking */
static int tests_run    = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/* Test macros */
#define TEST_ASSERT(condition, test_name) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("[PASS] %s\n", test_name); \
    } else { \
        tests_failed++; \
        printf("[FAIL] %s\n", test_name); \
    } \
} while(0)

/* Test functions */
static void test_init_null_params(void) {
    uint8_t result;
    
    printf("\n--- Test: Initialize with NULL parameters ---\n");
    
    result = my_cmd_init(NULL, test_cmd_table, TEST_CMD_TABLE_SIZE);
    TEST_ASSERT(result == MY_CMD_ERR_NULL, "Init with NULL handle");
    
    result = my_cmd_init(&(my_cmd_handle_t){0}, NULL, TEST_CMD_TABLE_SIZE);
    TEST_ASSERT(result == MY_CMD_ERR_NULL, "Init with NULL table");
    
    result = my_cmd_init(&(my_cmd_handle_t){0}, test_cmd_table, 0);
    TEST_ASSERT(result == MY_CMD_ERR_NULL, "Init with zero size");
}

static void test_init_valid_params(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Initialize with valid parameters ---\n");
    
    result = my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    TEST_ASSERT(result == MY_CMD_OK, "Init with valid params");
    TEST_ASSERT(handle.table_size == TEST_CMD_TABLE_SIZE, "Table size set correctly");
}

static void test_execute_null_params(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Execute with NULL parameters ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    result = my_cmd_execute(NULL, "help");
    TEST_ASSERT(result == MY_CMD_ERR_NULL, "Execute with NULL handle");
    
    result = my_cmd_execute(&handle, NULL);
    TEST_ASSERT(result == MY_CMD_ERR_NULL, "Execute with NULL input");
}

static void test_execute_valid_commands(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Execute valid commands ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    result = my_cmd_execute(&handle, "help");
    TEST_ASSERT(result == MY_CMD_OK, "Execute 'help'");
    
    result = my_cmd_execute(&handle, "reset");
    TEST_ASSERT(result == MY_CMD_OK, "Execute 'reset'");
    
    result = my_cmd_execute(&handle, "status");
    TEST_ASSERT(result == MY_CMD_OK, "Execute 'status'");
}

static void test_execute_unknown_command(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Execute unknown command ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    result = my_cmd_execute(&handle, "unknown");
    TEST_ASSERT(result == MY_CMD_ERR_NOT_FOUND, "Unknown command returns NOT_FOUND");
    
    result = my_cmd_execute(&handle, "xyz123");
    TEST_ASSERT(result == MY_CMD_ERR_NOT_FOUND, "Random string returns NOT_FOUND");
}

static void test_execute_command_with_args(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Execute command with arguments ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    result = my_cmd_execute(&handle, "set key value");
    TEST_ASSERT(result == MY_CMD_OK, "Command with 2 args");
    TEST_ASSERT(g_test_set_count > 0, "Command function called");
    
    result = my_cmd_execute(&handle, "get key");
    TEST_ASSERT(result == MY_CMD_OK, "Command with 1 arg");
}

static void test_execute_command_with_whitespace(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Commands with various whitespace ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    result = my_cmd_execute(&handle, "  help");
    TEST_ASSERT(result == MY_CMD_OK, "Command with leading spaces");
    
    result = my_cmd_execute(&handle, "help  arg1  arg2");
    TEST_ASSERT(result == MY_CMD_OK, "Command with multiple spaces");
    
    result = my_cmd_execute(&handle, "\thelp");
    TEST_ASSERT(result == MY_CMD_OK, "Command with leading tab");
}

static void test_execute_command_with_newline(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Commands with newline terminators ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    result = my_cmd_execute(&handle, "help\n");
    TEST_ASSERT(result == MY_CMD_OK, "Command with LF");
    
    result = my_cmd_execute(&handle, "help\r");
    TEST_ASSERT(result == MY_CMD_OK, "Command with CR");
    
    result = my_cmd_execute(&handle, "help\r\n");
    TEST_ASSERT(result == MY_CMD_OK, "Command with CRLF");
}

static void test_command_too_long(void) {
    my_cmd_handle_t handle;
    char long_cmd[MY_CMD_MAX_NAME_LEN + 10];
    uint8_t result;
    
    printf("\n--- Test: Command name too long ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    memset(long_cmd, 'A', sizeof(long_cmd) - 1);
    long_cmd[sizeof(long_cmd) - 1] = '\0';
    
    result = my_cmd_execute(&handle, long_cmd);
    TEST_ASSERT(result == MY_CMD_ERR_TOO_LONG, "Too long command returns error");
}

static void test_too_many_args(void) {
    my_cmd_handle_t handle;
    char input[256];
    uint8_t result;
    
    printf("\n--- Test: Too many arguments ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    /* Create command with many arguments */
    strcpy(input, "set");
    for (int i = 0; i < MY_CMD_MAX_ARGS + 5; i++) {
        char arg[16];
        sprintf(arg, " arg%d", i);
        strcat(input, arg);
    }
    
    result = my_cmd_execute(&handle, input);
    TEST_ASSERT(result == MY_CMD_ERR_TOO_MANY_ARGS, "Too many args returns error");
}

static void test_command_failure(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Command execution failure ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    result = my_cmd_execute(&handle, "fail");
    TEST_ASSERT(result == MY_CMD_ERR_EXECUTION, "Failed command returns error");
}

static void test_simple_api(void) {
    uint8_t result;
    
    printf("\n--- Test: Simple API (no handle) ---\n");
    
    result = my_cmd_execute_simple(test_cmd_table, TEST_CMD_TABLE_SIZE, "help");
    TEST_ASSERT(result == MY_CMD_OK, "Simple execute - valid command");
    
    result = my_cmd_execute_simple(test_cmd_table, TEST_CMD_TABLE_SIZE, "unknown");
    TEST_ASSERT(result == MY_CMD_ERR_NOT_FOUND, "Simple execute -unknown command");
    
    result = my_cmd_execute_simple(NULL, TEST_CMD_TABLE_SIZE, "help");
    TEST_ASSERT(result == MY_CMD_ERR_NULL, "Simple execute - NULL table");
}

static void test_help_print(void) {
    printf("\n--- Test: Print help ---\n");
    
    my_cmd_print_help(test_cmd_table, TEST_CMD_TABLE_SIZE);
    TEST_ASSERT(1, "Print help executed");
    
    my_cmd_print_help(NULL, 0);
    TEST_ASSERT(1, "Print help with NULL table handled");
}

static void test_get_count(void) {
    uint16_t count;
    
    printf("\n--- Test: Get command count ---\n");
    
    count = my_cmd_get_count(test_cmd_table);
    TEST_ASSERT(count == TEST_CMD_TABLE_SIZE, "Get count returns correct value");
    
    count = my_cmd_get_count(NULL);
    TEST_ASSERT(count == 0, "Get count with NULL table returns 0");
}

static void test_last_command(void) {
    my_cmd_handle_t handle;
    uint8_t result;
    
    printf("\n--- Test: Last command tracking ---\n");
    
    my_cmd_init(&handle, test_cmd_table, TEST_CMD_TABLE_SIZE);
    
    result = my_cmd_execute(&handle, "status");
    TEST_ASSERT(result == MY_CMD_OK, "Execute status");
    TEST_ASSERT(strcmp(handle.last_cmd, "status") == 0, "Last command tracked");
}

/**
 * @brief Run all tests
 */
void my_cmd_run_all_tests(void) {
    printf("\n");
    printf("=======================================================\n");
    printf("  My CMD_LINE Parser Unit Test Suite\n");
    printf("  Author: Tobi | Date: 04/04/2026\n");
    printf("  Enhanced version with argument parsing & better API\n");
    printf("=======================================================\n");
    
    test_init_null_params();
    test_init_valid_params();
    test_execute_null_params();
    test_execute_valid_commands();
    test_execute_unknown_command();
    test_execute_command_with_args();
    test_execute_command_with_whitespace();
    test_execute_command_with_newline();
    test_command_too_long();
    test_too_many_args();
    test_command_failure();
    test_simple_api();
    test_help_print();
    test_get_count();
    test_last_command();
    
    printf("\n");
    printf("=======================================================\n");
    printf("  Test Summary\n");
    printf("=======================================================\n");
    printf("  Total:  %d\n", tests_run);
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("=======================================================\n\n");
    
    if (tests_failed == 0) {
        printf("All tests PASSED!\n\n");
    } else {
        printf("Some tests FAILED!\n\n");
    }
}
