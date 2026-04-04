/**
 ******************************************************************************
 * @file    main_original.c
 * @author  Tobi
 * @date    04/04/2026
 * @brief   Main entry point for original cmd_line parser tests
 ******************************************************************************
 */

#include "cmd_line_test.h"

int main(void) {
    /* Run all tests for original cmd_line parser */
    uint8_t result = run_all_tests();
    
    return (result == TEST_PASS) ? 0 : 1;
}
