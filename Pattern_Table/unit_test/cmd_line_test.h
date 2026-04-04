/**
 ******************************************************************************
 * @file    cmd_line_test.h
 * @author  Tobi
 * @date    04/04/2026
 * @brief   Unit Test framework for cmd_line parser
 ******************************************************************************
 */

#ifndef __CMD_LINE_TEST_H__
#define __CMD_LINE_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Test result codes */
#define TEST_PASS    0
#define TEST_FAIL    1

/* Test function prototype */
typedef uint8_t (*pf_test_func)(void);

/* Test case structure */
typedef struct {
    const char* test_name;
    pf_test_func  test_func;
    uint8_t       result;
} test_case_t;

/* Test suite runner */
uint8_t run_all_tests(void);
uint8_t run_single_test(uint8_t test_index);
void    print_test_summary(void);

#ifdef __cplusplus
}
#endif

#endif /* __CMD_LINE_TEST_H__ */
