/**
 ******************************************************************************
 * @file    cmd_line_test.c
 * @author  Tobi
 * @date    04/04/2026
 * @brief   Unit Test implementation for cmd_line parser
 ******************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include "cmd_line_test.h"
#include "cmd_line.h"

/* Forward declaration of test functions */
static uint8_t test_null_table(void);
static uint8_t test_null_command(void);
static uint8_t test_valid_command(void);
static uint8_t test_unknown_command(void);
static uint8_t test_command_too_long(void);
static uint8_t test_command_with_spaces(void);
static uint8_t test_command_with_newline(void);
static uint8_t test_command_with_carriage_return(void);
static uint8_t test_multiple_commands_sequential(void);
static uint8_t test_command_case_sensitivity(void);

/* Test command table */
static int32_t dummy_func_1(uint8_t* argv) {
    (void)argv;
    return 0;
}

static int32_t dummy_func_2(uint8_t* argv) {
    (void)argv;
    return 0;
}

static int32_t dummy_func_3(uint8_t* argv) {
    (void)argv;
    return 0;
}

static cmd_line_t test_cmd_table[] = {
    {"help",    dummy_func_1, "Display help information"},
    {"reset",   dummy_func_2, "Reset the system"},
    {"status",  dummy_func_3, "Show system status"},
    {NULL,      NULL,         NULL}  /* Must be NULL terminated */
};

/* Global test counter */
static uint8_t tests_passed = 0;
static uint8_t tests_failed = 0;
static uint8_t total_tests   = 0;

/* Test suite definition */
static test_case_t test_suite[] = {
    {"test_null_table",            test_null_table,            TEST_FAIL},
    {"test_null_command",          test_null_command,          TEST_FAIL},
    {"test_valid_command",         test_valid_command,         TEST_FAIL},
    {"test_unknown_command",       test_unknown_command,       TEST_FAIL},
    {"test_command_too_long",      test_command_too_long,      TEST_FAIL},
    {"test_command_with_spaces",   test_command_with_spaces,   TEST_FAIL},
    {"test_command_with_newline",  test_command_with_newline,  TEST_FAIL},
    {"test_command_with_cr",       test_command_with_carriage_return, TEST_FAIL},
    {"test_multiple_commands_seq", test_multiple_commands_sequential, TEST_FAIL},
    {"test_case_sensitivity",      test_command_case_sensitivity,     TEST_FAIL},
};

#define NUM_TESTS (sizeof(test_suite) / sizeof(test_case_t))

/**
 * @brief Test: NULL command table should return CMD_TBL_NOT_FOUND
 */
static uint8_t test_null_table(void) {
    uint8_t result = cmd_line_parser(NULL, (uint8_t*)"help");
    return (result == CMD_TBL_NOT_FOUND) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: NULL command should handle gracefully
 */
static uint8_t test_null_command(void) {
    uint8_t result = cmd_line_parser(test_cmd_table, NULL);
    /* Should not crash, result depends on implementation */
    return (result == CMD_NOT_FOUND || result == CMD_SUCCESS) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: Valid command should execute and return CMD_SUCCESS
 */
static uint8_t test_valid_command(void) {
    uint8_t result = cmd_line_parser(test_cmd_table, (uint8_t*)"help");
    return (result == CMD_SUCCESS) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: Unknown command should return CMD_NOT_FOUND
 */
static uint8_t test_unknown_command(void) {
    uint8_t result = cmd_line_parser(test_cmd_table, (uint8_t*)"unknown");
    return (result == CMD_NOT_FOUND) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: Command exceeding MAX_CMD_SIZE should return CMD_TOO_LONG
 */
static uint8_t test_command_too_long(void) {
    uint8_t long_cmd[MAX_CMD_SIZE + 5];
    memset(long_cmd, 'A', sizeof(long_cmd) - 1);
    long_cmd[sizeof(long_cmd) - 1] = '\0';
    
    uint8_t result = cmd_line_parser(test_cmd_table, long_cmd);
    return (result == CMD_TOO_LONG) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: Command followed by spaces should still be parsed correctly
 */
static uint8_t test_command_with_spaces(void) {
    uint8_t result = cmd_line_parser(test_cmd_table, (uint8_t*)"help arg1 arg2");
    return (result == CMD_SUCCESS) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: Command with newline terminator
 */
static uint8_t test_command_with_newline(void) {
    uint8_t result = cmd_line_parser(test_cmd_table, (uint8_t*)"reset\n");
    return (result == CMD_SUCCESS) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: Command with carriage return terminator
 */
static uint8_t test_command_with_carriage_return(void) {
    uint8_t result = cmd_line_parser(test_cmd_table, (uint8_t*)"status\r");
    return (result == CMD_SUCCESS) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: Multiple commands executed sequentially
 */
static uint8_t test_multiple_commands_sequential(void) {
    uint8_t result1 = cmd_line_parser(test_cmd_table, (uint8_t*)"help");
    uint8_t result2 = cmd_line_parser(test_cmd_table, (uint8_t*)"reset");
    uint8_t result3 = cmd_line_parser(test_cmd_table, (uint8_t*)"status");
    
    return ((result1 == CMD_SUCCESS) && 
            (result2 == CMD_SUCCESS) && 
            (result3 == CMD_SUCCESS)) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Test: Command parsing is case-sensitive
 */
static uint8_t test_command_case_sensitivity(void) {
    uint8_t result_upper = cmd_line_parser(test_cmd_table, (uint8_t*)"HELP");
    uint8_t result_mixed = cmd_line_parser(test_cmd_table, (uint8_t*)"Help");
    
    /* Both should return CMD_NOT_FOUND since table has lowercase */
    return ((result_upper == CMD_NOT_FOUND) && 
            (result_mixed == CMD_NOT_FOUND)) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Run all tests in the test suite
 */
uint8_t run_all_tests(void) {
    tests_passed = 0;
    tests_failed = 0;
    total_tests  = NUM_TESTS;
    
    printf("\n========================================\n");
    printf("  CMD_LINE Parser Unit Test Suite\n");
    printf("  Author: Tobi | Date: 04/04/2026\n");
    printf("========================================\n\n");
    
    for (uint8_t i = 0; i < NUM_TESTS; i++) {
        printf("[TEST %02d/%02d] %-35s ", 
               i + 1, NUM_TESTS, test_suite[i].test_name);
        
        uint8_t result = test_suite[i].test_func();
        test_suite[i].result = result;
        
        if (result == TEST_PASS) {
            printf("[PASS]\n");
            tests_passed++;
        } else {
            printf("[FAIL]\n");
            tests_failed++;
        }
    }
    
    printf("\n========================================\n");
    printf("  Test Summary\n");
    printf("========================================\n");
    printf("  Total:  %d\n", total_tests);
    printf("  Passed: %d\n", tests_passed);
    printf("  Failed: %d\n", tests_failed);
    printf("========================================\n\n");
    
    return (tests_failed == 0) ? TEST_PASS : TEST_FAIL;
}

/**
 * @brief Run a single test by index
 */
uint8_t run_single_test(uint8_t test_index) {
    if (test_index >= NUM_TESTS) {
        printf("Error: Test index %d out of range (0-%d)\n", test_index, NUM_TESTS - 1);
        return TEST_FAIL;
    }
    
    printf("\n[TEST] %s\n", test_suite[test_index].test_name);
    uint8_t result = test_suite[test_index].test_func();
    test_suite[test_index].result = result;
    
    if (result == TEST_PASS) {
        printf("  => PASS\n");
    } else {
        printf("  => FAIL\n");
    }
    
    return result;
}

/**
 * @brief Print test summary
 */
void print_test_summary(void) {
    printf("\nTest Results Summary:\n");
    printf("---------------------\n");
    for (uint8_t i = 0; i < NUM_TESTS; i++) {
        printf("%-35s %s\n", 
               test_suite[i].test_name,
               (test_suite[i].result == TEST_PASS) ? "PASS" : "FAIL");
    }
    printf("---------------------\n");
}
