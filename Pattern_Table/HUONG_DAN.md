# Hướng dẫn Pattern Table - Command Line Parser

**Tác giả:** Tobi  
**Ngày:** 04/04/2026

---

## 📚 Mục lục

1. [Pattern Table là gì?](#pattern-table-là-gì)
2. [Bài tập 1: Unit Test cho cmd_line gốc](#bài-tập-1-unit-test)
3. [Bài tập 2: Tự code cmd_line của riêng mình](#bài-tập-2-custom-implementation)
4. [Cách build và chạy test](#cách-build-và-chạy-test)
5. [Giải thích chi tiết](#giải-thích-chi-tiết)

---

## 🎯 Pattern Table là gì?

### Định nghĩa:
**Pattern Table** là một design pattern sử dụng một bảng (array) các function pointer để map giữa một "pattern" (mẫu) và function cần thực thi.

### Tại sao dùng?
- ❌ Thay vì dùng nhiều `if-else` hoặc `switch-case`
- ✅ Dễ dàng thêm/xóa commands
- ✅ Code sạch sẽ, dễ bảo trì
- ✅ Dễ test và debug

### Ví dụ cơ bản:

```c
// Cách CỦ: Dùng if-else (dài dòng, khó maintain)
if (strcmp(cmd, "help") == 0) {
    cmd_help();
} else if (strcmp(cmd, "reset") == 0) {
    cmd_reset();
} else if (strcmp(cmd, "status") == 0) {
    cmd_status();
}
// ... thêm 100 lệnh nữa => RẤT DÀI!

// Cách MỚI: Dùng Pattern Table (sạch sẽ, dễ mở rộng)
cmd_line_t cmd_table[] = {
    {"help",   cmd_help,   "Hiển thị trợ giúp"},
    {"reset",  cmd_reset,  "Reset hệ thống"},
    {"status", cmd_status, "Xem trạng thái"},
    {NULL, NULL, NULL}  // Đánh dấu kết thúc
};

// Tự động tìm và chạy lệnh
for (int i = 0; cmd_table[i].cmd != NULL; i++) {
    if (strcmp(input, cmd_table[i].cmd) == 0) {
        cmd_table[i].func(args);  // <- Magic!
        break;
    }
}
```

---

## 📝 Bài tập 1: Unit Test

### Vị trí: `unit_test/`

### Đã test những gì?

| STT | Test Case | Mục đích |
|-----|-----------|----------|
| 1 | `test_null_table` | Truyền NULL table => lỗi |
| 2 | `test_null_command` | Truyền NULL command => không crash |
| 3 | `test_valid_command` | Lệnh đúng => thành công |
| 4 | `test_unknown_command` | Lệnh lạ => not found |
| 5 | `test_command_too_long` | Tên quá dài => error |
| 6 | `test_command_with_spaces` | Có khoảng trắng => vẫn chạy |
| 7 | `test_command_with_newline` | Có \n => vẫn chạy |
| 8 | `test_command_with_cr` | Có \r => vẫn chạy |
| 9 | `test_multiple_commands_seq` | Chạy nhiều lệnh liên tiếp |
| 10 | `test_case_sensitivity` | Phân biệt HOA/thường |

### Cấu trúc test:

```c
// 1. Định nghĩa bảng test cases
static test_case_t test_suite[] = {
    {"tên_test", function_test, KẾT_QUẢ_MONG_ĐỢI},
    // ...
};

// 2. Chạy tất cả tests
for (mỗi test) {
    kết_quả = test_function();
    if (kết_quả == MONG_ĐỢI) {
        printf("[PASS]");
    } else {
        printf("[FAIL]");
    }
}
```

### Cách viết test case mới:

```c
// Bước 1: Viết function test
static uint8_t test_của_bạn(void) {
    uint8_t result = cmd_line_parser(table, "lệnh_mới");
    return (result == KẾT_QUẢ_MONG_ĐỢI) ? TEST_PASS : TEST_FAIL;
}

// Bước 2: Thêm vào test_suite
static test_case_t test_suite[] = {
    // ...
    {"tên_test_mới", test_của_bạn, TEST_FAIL},
};
```

---

## 🚀 Bài tập 2: Custom Implementation

### Vị trí: `my_cmd_line/`

### Cải tiến so với bản gốc:

| Tính năng | Bản gốc | Bản của mình |
|-----------|---------|--------------|
| Parse arguments | ❌ | ✅ (argc/argv) |
| Quản lý state | ❌ | ✅ (handle) |
| Mã lỗi | 4 loại | 6 loại (chi tiết hơn) |
| Help system | ❌ | ✅ |
| Giới hạn args | ❌ | ✅ (MAX_ARGS) |
| Last command | ❌ | ✅ (tracking) |

### Kiến trúc:

```
┌─────────────────────────────────────┐
│         my_cmd_handle_t             │
│  ┌────────────────────────────┐    │
│  │ table: cmd_entry_t[]       │    │
│  │ table_size: uint16_t       │    │
│  │ last_cmd: char[16]         │    │
│  │ last_result: uint8_t       │    │
│  └────────────────────────────┘    │
└─────────────────────────────────────┘
              ↓
┌─────────────────────────────────────┐
│      my_cmd_execute()               │
│  1. Parse command name             │
│  2. Parse arguments                │
│  3. Search in table                │
│  4. Execute function               │
│  5. Return result                  │
└─────────────────────────────────────┘
```

### API chính:

```c
// 1. Khởi tạo
my_cmd_handle_t handle;
my_cmd_init(&handle, cmd_table, table_size);

// 2. Thực thi lệnh
my_cmd_execute(&handle, "help arg1 arg2");

// 3. Hoặc dùng nhanh (không cần init)
my_cmd_execute_simple(cmd_table, size, "help");

// 4. In trợ giúp
my_cmd_print_help(cmd_table, size);

// 5. Đếm số lệnh
uint16_t count = my_cmd_get_count(cmd_table);
```

### Ví dụ sử dụng:

```c
#include "my_cmd_line.h"

// Bước 1: Viết các function
int32_t cmd_led(int argc, char* argv[]) {
    printf("LED command: %d args\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("  arg[%d]: %s\n", i, argv[i]);
    }
    return 0;
}

int32_t cmd_motor(int argc, char* argv[]) {
    printf("Motor command\n");
    return 0;
}

// Bước 2: Tạo bảng lệnh
my_cmd_entry_t my_commands[] = {
    {"led",   cmd_led,   "Điều khiển LED"},
    {"motor", cmd_motor, "Điều khiển động cơ"},
};

// Bước 3: Sử dụng
int main() {
    my_cmd_handle_t handle;
    my_cmd_init(&handle, my_commands, 2);
    
    my_cmd_execute(&handle, "led on red");
    my_cmd_execute(&handle, "motor speed 100");
    
    return 0;
}
```

---

## 🛠️ Cách Build và Chạy Test

### Trên Windows:

#### Cách 1: Dùng script có sẵn
```bash
cd "d:\FPT Embedded Bootcamp\Project\ak-base-kit-stm32l151-main\application\sources\Pattern_Table"
run_tests.bat
```

#### Cách 2: Compile thủ công (nếu có GCC)
```bash
# Test bản gốc
gcc -Wall -Wextra -std=c99 -I. -I../common \
    ../common/cmd_line.c \
    unit_test/cmd_line_test.c \
    main_original.c \
    -o test_original.exe

./test_original.exe

# Test bản custom
gcc -Wall -Wextra -std=c99 -I. -I../common \
    my_cmd_line/my_cmd_line.c \
    my_cmd_line/my_cmd_line_test.c \
    main_my.c \
    -o test_my.exe

./test_my.exe
```

### Trên Linux/Mac:

```bash
cd application/sources/Pattern_Table
make run
```

### Nếu chưa có compiler:

**Cài đặt GCC:**

**Windows:**
1. Tải MinGW-w64: https://www.mingw-w64.org/
2. Hoặc MSYS2: https://www.msys2.org/
3. Thêm vào PATH

**Linux (Ubuntu/Debian):**
```bash
sudo apt install gcc
```

**Mac:**
```bash
xcode-select --install
```

---

## 📖 Giải thích chi tiết

### Cơ chế Pattern Table hoạt động:

```
Input: "help arg1 arg2"
        ↓
┌───────────────────────────┐
│  1. Parse command name    │
│  => cmd = "help"          │
└───────────────────────────┘
        ↓
┌───────────────────────────┐
│  2. Loop through table    │
│  ┌─────────────────────┐ │
│  │ table[0]: "help"    │ │ => MATCH!
│  │ table[1]: "reset"   │ │
│  │ table[2]: "status"  │ │
│  └─────────────────────┘ │
└───────────────────────────┘
        ↓
┌───────────────────────────┐
│  3. Call function         │
│  => table[0].func(args)   │
│  => cmd_help(args)        │
└───────────────────────────┘
        ↓
    Execute!
```

### Tại sao NULL terminated?

```c
cmd_line_t table[] = {
    {"help",  func1, "info1"},
    {"reset", func2, "info2"},
    {NULL, NULL, NULL}  // <- Quan trọng!
};

// Giúp loop biết khi nào dừng
for (int i = 0; table[i].cmd != NULL; i++) {
    // Tự động dừng ở NULL
}
```

### Advantage của function pointer:

```c
// Định nghĩa type
typedef int32_t (*cmd_func_t)(uint8_t* argv);

// Sử dụng
cmd_func_t my_func = &cmd_help;  // Trỏ tới function
my_func(args);                    // Gọi qua pointer
```

---

## 💡 Mẹo debug

### 1. Print debug:
```c
printf("[DEBUG] cmd = '%s'\n", cmd);
printf("[DEBUG] table[%d].cmd = '%s'\n", i, table[i].cmd);
printf("[DEBUG] strcmp result = %d\n", strcmp_result);
```

### 2. Check NULL pointers:
```c
if (table == NULL) {
    printf("ERROR: NULL table!\n");
    return ERROR;
}
```

### 3. Validate input:
```c
if (strlen(cmd) > MAX_CMD_SIZE) {
    printf("ERROR: Command too long!\n");
    return CMD_TOO_LONG;
}
```

---

## 📊 So sánh kết quả

### Bản gốc:
- ✅ Test NULL table handling
- ✅ Test valid commands
- ✅ Test error conditions
- ❌ Không parse arguments
- ❌ Không có state management

### Bản custom:
- ✅ Tất cả test của bản gốc
- ✅ Parse arguments (argc/argv)
- ✅ State management (handle)
- ✅ Better error codes
- ✅ Help system
- ✅ Command tracking

---

## 🎓 Bài học rút ra

1. **Pattern Table** giúp code dễ mở rộng
2. **Function pointer** là cốt lõi của pattern
3. **Unit test** giúp code đáng tin cậy hơn
4. **Good abstractions** giúp code sạch sẽ
5. **Defensive programming** tránh bugs

---

## 🔗 Tham khảo

- Original code: `../common/cmd_line.c`
- Unit test: `unit_test/cmd_line_test.c`
- Custom impl: `my_cmd_line/my_cmd_line.c`
- Full docs: `README.md`

---

**Chúc bạn học tốt! 🚀**
