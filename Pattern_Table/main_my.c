/**
 ******************************************************************************
 * @file    main_my.c
 * @author  Tobi
 * @date    04/04/2026
 * @brief   Main entry point for custom cmd_line parser tests
 ******************************************************************************
 */

#include "my_cmd_line.h"

/* External test function */
extern void my_cmd_run_all_tests(void);

int main(void) {
    /* Run all tests for custom cmd_line parser */
    my_cmd_run_all_tests();
    
    return 0;
}
