# Pattern Table Exercise - Command Line Parser

**Author:** Tobi  
**Date:** 04/04/2026  
**Purpose:** Learn and demonstrate the Pattern Table design pattern

---

## Overview

This exercise explores the **Pattern Table** design pattern through a command-line parser implementation. The Pattern Table pattern uses a table (array) of function pointers associated with patterns (commands) to create extensible, maintainable code.

### Structure

```
Pattern_Table/
├── unit_test/              # Exercise 1: Unit tests for original cmd_line
│   ├── cmd_line_test.h
│   └── cmd_line_test.c
├── my_cmd_line/            # Exercise 2: Custom cmd_line implementation
│   ├── my_cmd_line.h
│   ├── my_cmd_line.c
│   └── my_cmd_line_test.c
├── main_original.c         # Test runner for original
├── main_my.c               # Test runner for custom
├── Makefile                # Build configuration
└── README.md               # This file
```

---

## Exercise 1: Unit Test for Original cmd_line

**Location:** `unit_test/`

### What it tests:
- NULL table handling
- NULL command handling
- Valid command execution
- Unknown command detection
- Command length validation
- Commands with spaces, newlines, carriage returns
- Sequential command execution
- Case sensitivity

### Key Files:
- `cmd_line_test.h` - Test framework definitions
- `cmd_line_test.c` - 10 comprehensive test cases

### How to run:
```bash
make run_original
```

---

## Exercise 2: Custom cmd_line Implementation

**Location:** `my_cmd_line/`

### Enhancements over original:
1. **Argument Parsing**: Supports multiple arguments with argc/argv
2. **Handle-based Design**: Stateful parser with context
3. **Better Error Codes**: More granular error reporting
4. **Simple API**: One-shot execution option
5. **Help System**: Built-in help printing
6. **Last Command Tracking**: Remembers last executed command
7. **Better Validation**: Input sanitization and bounds checking

### Features:
- Configurable limits (max args, arg length, command name length)
- Whitespace handling (spaces, tabs, newlines, CRLF)
- Command state tracking
- Comprehensive error codes

### Key Files:
- `my_cmd_line.h` - API definitions and types
- `my_cmd_line.c` - Implementation
- `my_cmd_line_test.c` - 15 test cases covering all features

### How to run:
```bash
make run_my
```

---

## Pattern Table Pattern Explained

### Core Concept:

```c
// 1. Define function pointer type
typedef int32_t (*pf_cmd_func)(uint8_t* argv);

// 2. Define table entry structure
typedef struct {
    const int8_t* cmd;        // Pattern to match
    pf_cmd_func   func;       // Function to call
    const int8_t* info;       // Metadata
} cmd_line_t;

// 3. Create command table
cmd_line_t cmd_table[] = {
    {"help",  cmd_help,  "Show help"},
    {"reset", cmd_reset, "Reset system"},
    {NULL,    NULL,      NULL}  // Sentinel
};

// 4. Parse and dispatch
for (each entry in table) {
    if (input matches entry.cmd) {
        entry.func(args);  // Call associated function
        break;
    }
}
```

### Benefits:
- **Extensible**: Add commands by adding table entries
- **Maintainable**: All commands in one place
- **Clean**: No long if-else chains
- **Testable**: Easy to test individual components

---

## Building & Running

### Build all tests:
```bash
make all
```

### Run all tests:
```bash
make run
```

### Run specific test suite:
```bash
make run_original   # Original cmd_line tests
make run_my         # Custom cmd_line tests
```

### Clean build:
```bash
make clean
```

### Show help:
```bash
make help
```

---

## Error Codes

### Original cmd_line:
- `CMD_TBL_NOT_FOUND` (0) - NULL table
- `CMD_SUCCESS` (1) - Command executed successfully
- `CMD_NOT_FOUND` (2) - Command not in table
- `CMD_TOO_LONG` (3) - Command exceeds MAX_CMD_SIZE

### Custom cmd_line:
- `MY_CMD_OK` (0x00) - Success
- `MY_CMD_ERR_NULL` (0x01) - NULL parameter
- `MY_CMD_ERR_NOT_FOUND` (0x02) - Command not found
- `MY_CMD_ERR_TOO_LONG` (0x03) - Command name too long
- `MY_CMD_ERR_TOO_MANY_ARGS` (0x04) - Too many arguments
- `MY_CMD_ERR_EXECUTION` (0x05) - Command execution failed

---

## Design Principles Applied

1. **Clean Code**: Clear function names, consistent formatting
2. **Single Responsibility**: Each function does one thing
3. **Defensive Programming**: NULL checks, bounds checking
4. **Testability**: Comprehensive unit tests
5. **Documentation**: Clear comments and doxygen-style docs
6. **Extensibility**: Easy to add new commands

---

## Lessons Learned

1. **Pattern Table** eliminates complex conditional logic
2. **Function pointers** enable flexible dispatch mechanisms
3. **Table-driven design** makes code data-driven
4. **Unit testing** validates edge cases systematically
5. **Good abstractions** improve code reusability

---

## Future Enhancements

- Command aliases (multiple names for same function)
- Wildcard/pattern matching in command names
- Command history tracking
- Auto-completion support
- Hierarchical command tables (subcommands)

---

*End of README*
