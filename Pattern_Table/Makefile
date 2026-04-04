###############################################################################
# Makefile for Pattern_Table - Command Line Parser Tests
# Author: Tobi
# Date:   04/04/2026
###############################################################################

# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g -I. -I../common
LDFLAGS =

# Directories
UNIT_TEST_DIR = unit_test
MY_CMD_DIR   = my_cmd_line
COMMON_DIR   = ../common

# Source files
COMMON_SRCS = $(COMMON_DIR)/cmd_line.c
UNIT_TEST_SRCS = $(UNIT_TEST_DIR)/cmd_line_test.c
MY_CMD_SRCS = $(MY_CMD_DIR)/my_cmd_line.c
MY_TEST_SRCS = $(MY_CMD_DIR)/my_cmd_line_test.c

# Object files
COMMON_OBJS = $(COMMON_SRCS:.c=.o)
UNIT_TEST_OBJS = $(UNIT_TEST_SRCS:.c=.o)
MY_CMD_OBJS = $(MY_CMD_SRCS:.c=.o)
MY_TEST_OBJS = $(MY_TEST_SRCS:.c=.o)

# Targets
TARGET_ORIG = test_cmd_line_original
TARGET_MY = test_cmd_line_my

###############################################################################
# Build rules
###############################################################################

.PHONY: all clean run_original run_my run

all: $(TARGET_ORIG) $(TARGET_MY)

# Test original cmd_line parser
$(TARGET_ORIG): $(UNIT_TEST_OBJS) $(COMMON_OBJS) main_original.o
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Built: $(TARGET_ORIG)"

# Test custom cmd_line parser
$(TARGET_MY): $(MY_TEST_OBJS) $(MY_CMD_OBJS) main_my.o
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Built: $(TARGET_MY)"

# Compile rules
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

###############################################################################
# Run targets
###############################################################################

run_original: $(TARGET_ORIG)
	@echo ""
	@echo "Running original cmd_line tests..."
	@echo ""
	./$(TARGET_ORIG)

run_my: $(TARGET_MY)
	@echo ""
	@echo "Running custom cmd_line tests..."
	@echo ""
	./$(TARGET_MY)

run: all
	@echo ""
	@echo "=========================================="
	@echo "  Running ALL Pattern_Table Tests"
	@echo "=========================================="
	@echo ""
	@echo "--- Original cmd_line parser tests ---"
	./$(TARGET_ORIG)
	@echo ""
	@echo "--- Custom cmd_line parser tests ---"
	./$(TARGET_MY)

###############################################################################
# Clean
###############################################################################

clean:
	rm -f *.o $(COMMON_DIR)/*.o $(UNIT_TEST_DIR)/*.o $(MY_CMD_DIR)/*.o
	rm -f $(TARGET_ORIG) $(TARGET_MY)
	@echo "Cleaned build artifacts"

###############################################################################
# Help
###############################################################################

help:
	@echo "Pattern_Table Makefile Targets:"
	@echo "  make all           - Build all test executables"
	@echo "  make run           - Build and run all tests"
	@echo "  make run_original  - Build and run original cmd_line tests"
	@echo "  make run_my        - Build and run custom cmd_line tests"
	@echo "  make clean         - Remove build artifacts"
	@echo "  make help          - Show this help message"
