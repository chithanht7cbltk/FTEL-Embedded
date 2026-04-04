/**
 ******************************************************************************
 * @file    my_cmd_line.h
 * @author  Tobi
 * @date    04/04/2026
 * @brief   Custom command line parser with enhanced features
 ******************************************************************************
 */

#ifndef __MY_CMD_LINE_H__
#define __MY_CMD_LINE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

/* Configuration */
#define MY_CMD_MAX_NAME_LEN     16
#define MY_CMD_MAX_ARGS         8
#define MY_CMD_MAX_ARG_LEN      32

/* Return codes */
#define MY_CMD_OK               0x00
#define MY_CMD_ERR_NULL       0x01
#define MY_CMD_ERR_NOT_FOUND  0x02
#define MY_CMD_ERR_TOO_LONG   0x03
#define MY_CMD_ERR_TOO_MANY_ARGS 0x04
#define MY_CMD_ERR_EXECUTION  0x05

/* Command function prototype */
typedef int32_t (*my_cmd_func_t)(int argc, char* argv[]);

/* Command table entry */
typedef struct {
    const char*     name;       /* Command name */
    my_cmd_func_t   func;       /* Command function */
    const char*     help;       /* Help description */
} my_cmd_entry_t;

/* Command parser handle */
typedef struct {
    const my_cmd_entry_t* table;    /* Command table */
    uint16_t              table_size; /* Number of commands */
    char                  last_cmd[MY_CMD_MAX_NAME_LEN];
    uint8_t               last_result;
} my_cmd_handle_t;

/* API functions */
uint8_t     my_cmd_init(my_cmd_handle_t* handle, const my_cmd_entry_t* table, uint16_t table_size);
uint8_t     my_cmd_execute(my_cmd_handle_t* handle, const char* input);
uint8_t     my_cmd_execute_simple(const my_cmd_entry_t* table, uint16_t table_size, const char* input);
void        my_cmd_print_help(const my_cmd_entry_t* table, uint16_t table_size);
uint16_t    my_cmd_get_count(const my_cmd_entry_t* table);

#ifdef __cplusplus
}
#endif

#endif /* __MY_CMD_LINE_H__ */
