/**
 ******************************************************************************
 * @file    my_cmd_line.c
 * @author  Tobi
 * @date    04/04/2026
 * @brief   Custom command line parser implementation
 ******************************************************************************
 */

#include <string.h>
#include <stdio.h>
#include "my_cmd_line.h"

/**
 * @brief Khởi tạo bộ xử lý phân tích cú pháp dòng lệnh
 * 
 * Mô tả:
 *   Hàm này sẽ thiết lập một "handle" - cấu trúc quản lý trạng thái
 *   cho bộ phân tích cú pháp. Handle lưu giữ bảng lệnh, kích thước bảng,
 *   và các thông tin trạng thái khác.
 * 
 * Các bước thực hiện:
 *   Bước 1: Kiểm tra tham số đầu vào có hợp lệ không (NULL, size = 0)
 *   Bước 2: Gán bảng lệnh vào handle
 *   Bước 3: Lưu kích thước bảng
 *   Bước 4: Đặt kết quả lệnh cuối cùng về mặc định (OK)
 *   Bước 5: Xóa sạch tên lệnh cuối cùng
 *   Bước 6: Trả về mã thành công
 * 
 * @param handle      Con trỏ tới cấu trúc handle cần khởi tạo
 * @param table       Con trỏ tới mảng các bảng lệnh
 * @param table_size  Số lượng phần tử trong bảng
 * @return            MY_CMD_OK nếu thành công, MY_CMD_ERR_NULL nếu tham số NULL
 */
uint8_t my_cmd_init(my_cmd_handle_t* handle, const my_cmd_entry_t* table, uint16_t table_size) {
    /* Bước 1: Kiểm tra tham số đầu vào - nếu có NULL hoặc size = 0 thì báo lỗi */
    if (handle == NULL || table == NULL || table_size == 0) {
        return MY_CMD_ERR_NULL;
    }
    
    /* Bước 2: Lưu con trỏ tới bảng lệnh vào handle */
    handle->table      = table;
    
    /* Bước 3: Lưu số lượng lệnh trong bảng */
    handle->table_size = table_size;
    
    /* Bước 4: Đặt kết quả thực thi lệnh cuối cùng về mặc định là OK */
    handle->last_result = MY_CMD_OK;
    
    /* Bước 5: Xóa sạch bộ đệm tên lệnh cuối cùng (đặt toàn bộ về 0) */
    memset(handle->last_cmd, 0, sizeof(handle->last_cmd));
    
    /* Bước 6: Trả về mã thành công */
    return MY_CMD_OK;
}

/**
 * @brief Phân tích chuỗi đầu vào và thực thi lệnh tương ứng
 * 
 * Mô tả:
 *   Đây là hàm CORE của bộ phân tích cú pháp. Nó sẽ:
 *   - Tách tên lệnh từ chuỗi đầu vào
 *   - Tách các tham số (arguments) đi kèm
 *   - Tìm kiếm tên lệnh trong bảng lệnh
 *   - Gọi hàm tương ứng nếu tìm thấy
 * 
 * Các bước thực hiện:
 *   Bước 1: Kiểm tra tham số đầu vào (handle, input có NULL không?)
 *   Bước 2: Bỏ qua các khoảng trắng đầu dòng (space, tab)
 *   Bước 3: Kiểm tra xem input có rỗng không chỉ chứa whitespace/newline)
 *   Bước 4: Tách tên lệnh (command name) cho đến khi gặp khoảng trắng hoặc kết thúc chuỗi
 *   Bước 5: Kiểm tra độ dài tên lệnh (không được vượt quá MAX_NAME_LEN)
 *   Bước 6: Lưu tên lệnh vào handle.last_cmd để theo dõi
 *   Bước 7: Tách các tham số (arguments) - lặp qua từng arg một
 *      - Bước 7.1: Bỏ qua khoảng trắng giữa các arg
 *      - Bước 7.2: Kiểm tra đã hết input chưa
 *      - Bước 7.3: Kiểm tra giới hạn số lượng arg (MAX_ARGS)
 *      - Bước 7.4: Lưu vị trí bắt đầu của arg
 *      - Bước 7.5: Tìm vị trí kết thúc arg
 *   Bước 8: Tìm kiếm tên lệnh trong bảng lệnh (dùng strcmp)
 *   Bước 9: Nếu tìm thấy -> thực thi hàm tương ứng
 *   Bước 10: Nếu không tìm thấy -> trả về lỗi NOT_FOUND
 * 
 * @param handle  Con trỏ tới handle đã được khởi tạo
 * @param input   Chuỗi lệnh đầu vào (vd: "led on red")
 * @return        Mã kết quả thực thi (OK, NOT_FOUND, TOO_LONG, v.v.)
 */
uint8_t my_cmd_execute(my_cmd_handle_t* handle, const char* input) {
    char cmd_name[MY_CMD_MAX_NAME_LEN];   // Bộ đệm chứa tên lệnh
    char* argv[MY_CMD_MAX_ARGS];          // Mảng con trỏ chứa các arguments
    int   argc = 0;                       // Số lượng arguments
    int   i;                              // Biến lặp
    
    /* Bước 1: Kiểm tra tham số đầu vào - handle hoặc input là NULL thì lỗi */
    if (handle == NULL || input == NULL) {
        return MY_CMD_ERR_NULL;
    }
    
    /* Bước 2: Bỏ qua các khoảng trắng ở đầu (space ' ' hoặc tab '\t') */
    while (*input == ' ' || *input == '\t') {
        input++;
    }
    
    /* Bước 3: Kiểm tra xem input có rỗng không (hết chuỗi hoặc chỉ có newline) */
    if (*input == '\0' || *input == '\n' || *input == '\r') {
        return MY_CMD_ERR_NOT_FOUND;
    }
    
    /* Bước 4: Tách tên lệnh - đọc từng ký tự cho đến khi gặp khoảng trắng hoặc newline */
    memset(cmd_name, 0, sizeof(cmd_name));  // Xóa sạch bộ đệm trước khi dùng
    i = 0;
    while (*input && *input != ' ' && *input != '\t' && 
           *input != '\n' && *input != '\r') {
        /* Bước 5: Kiểm tra độ dài tên lệnh - nếu quá dài thì báo lỗi */
        if (i < MY_CMD_MAX_NAME_LEN - 1) {
            cmd_name[i++] = *input;  // Thêm ký tự vào tên lệnh
        } else {
            return MY_CMD_ERR_TOO_LONG;  // Vượt quá giới hạn!
        }
        input++;  // Chuyển sang ký tự tiếp theo
    }
    cmd_name[i] = '\0';  // Thêm ký tự null để kết thúc chuỗi C-string
    
    /* Bước 6: Lưu tên lệnh vào handle để theo dõi (dùng cho debug/history) */
    strncpy(handle->last_cmd, cmd_name, MY_CMD_MAX_NAME_LEN - 1);
    
    /* Bước 7: Tách các tham số (arguments) */
    argc = 0;  // Đặt lại bộ đếm arg về 0
    while (*input) {
        /* Bước 7.1: Bỏ qua khoảng trắng giữa các argument */
        while (*input == ' ' || *input == '\t') {
            input++;
        }
        
        /* Bước 7.2: Kiểm tra xem đã hết input chưa (gặp null hoặc newline) */
        if (*input == '\0' || *input == '\n' || *input == '\r') {
            break;  // Hết input -> thoát khỏi vòng lặp tách arg
        }
        
        /* Bước 7.3: Kiểm tra giới hạn số lượng argument */
        if (argc >= MY_CMD_MAX_ARGS - 1) {
            return MY_CMD_ERR_TOO_MANY_ARGS;  // Quá nhiều arg!
        }
        
        /* Bước 7.4: Lưu vị trí bắt đầu của argument hiện tại */
        argv[argc] = (char*)input;
        argc++;  // Tăng bộ đếm argument
        
        /* Bước 7.5: Tìm vị trí kết thúc của argument (gặp khoảng trắng hoặc newline) */
        while (*input && *input != ' ' && *input != '\t' && 
               *input != '\n' && *input != '\r') {
            input++;
        }
        
        /* Nếu chưa hết chuỗi, cần null-terminate argument hiện tại */
        /* Lưu ý: Đoạn code này tạm thời bỏ qua việc modify input string */
        /* Trong thực tế, nên tạo bản sao của input để tránh modify string gốc */
    }
    
    /* Bước 8: Tìm kiếm tên lệnh trong bảng lệnh */
    for (i = 0; i < handle->table_size; i++) {
        /* So sánh tên lệnh nhập vào với tên lệnh trong bảng */
        if (strcmp(handle->table[i].name, cmd_name) == 0) {
            
            /* Bước 9: Nếu tìm thấy -> thực thi hàm tương ứng */
            int32_t result = handle->table[i].func(argc, argv);
            
            /* Lưu kết quả thực thi vào handle */
            handle->last_result = (result == 0) ? MY_CMD_OK : MY_CMD_ERR_EXECUTION;
            
            /* Trả về kết quả */
            return handle->last_result;
        }
    }
    
    /* Bước 10: Nếu không tìm thấy tên lệnh trong bảng -> báo lỗi */
    handle->last_result = MY_CMD_ERR_NOT_FOUND;
    return MY_CMD_ERR_NOT_FOUND;
}

/**
 * @brief Thực thi lệnh đơn giản (không cần khởi tạo handle trước)
 * 
 * Mô tả:
 *   Đây là hàm tiện ích cho trường hợp chỉ cần thực thi một lệnh duy nhất
 *   mà không cần quản lý trạng thái lâu dài. Hàm sẽ tự động tạo handle
 *   tạm thời, thực thi, rồi trả về kết quả.
 * 
 * Các bước thực hiện:
 *   Bước 1: Tạo một handle tạm thời trên stack
 *   Bước 2: Khởi tạo handle với bảng lệnh và kích thước cho trước
 *   Bước 3: Kiểm tra xem khởi tạo có thành công không
 *   Bước 4: Gọi hàm execute với handle tạm và chuỗi input
 *   Bước 5: Trả về kết quả từ execute
 * 
 * Ưu điểm:
 *   - Không cần khai báo biến handle riêng
 *   - Tiện lợi cho việc thực thi nhanh
 * 
 * Nhược điểm:
 *   - Không theo dõi được lịch sử lệnh
 *   - Không truy xuất được last_cmd, last_result sau khi thực thi
 * 
 * @param table       Con trỏ tới bảng lệnh
 * @param table_size  Số lượng lệnh trong bảng
 * @param input       Chuỗi lệnh đầu vào
 * @return            Mã kết quả thực thi
 */
uint8_t my_cmd_execute_simple(const my_cmd_entry_t* table, uint16_t table_size, const char* input) {
    /* Bước 1 & 2: Tạo handle tạm và khởi tạo ngay */
    my_cmd_handle_t handle;
    uint8_t result;
    
    result = my_cmd_init(&handle, table, table_size);
    
    /* Bước 3: Kiểm tra khởi tạo có thành công không */
    if (result != MY_CMD_OK) {
        return result;  // Nếu lỗi thì trả về ngay
    }
    
    /* Bước 4 & 5: Thực thi và trả về kết quả */
    return my_cmd_execute(&handle, input);
}

/**
 * @brief In danh sách trợ giúp của tất cả các lệnh
 * 
 * Mô tả:
 *   Hàm này sẽ duyệt qua toàn bộ bảng lệnh và in ra thông tin
 *   của từng lệnh dưới dạng bảng dễ đọc. Hữu ích cho user
 *   muốn biết có những lệnh nào available.
 * 
 * Các bước thực hiện:
 *   Bước 1: Kiểm tra table có NULL không
 *   Bước 2: In tiêu đề header
 *   Bước 3: Duyệt qua từng phần tử trong bảng
 *      - Bước 3.1: Kiểm tra tên lệnh có NULL không (phòng thủ)
 *      - Bước 3.2: In tên lệnh và mô tả (nếu có)
 *   Bước 4: In footer và tổng số lệnh
 * 
 * @param table       Con trỏ tới bảng lệnh
 * @param table_size  Số lượng lệnh trong bảng
 */
void my_cmd_print_help(const my_cmd_entry_t* table, uint16_t table_size) {
    int i;
    
    /* Bước 1: Kiểm tra table có NULL không */
    if (table == NULL) {
        printf("Error: NULL table\n");
        return;
    }
    
    /* Bước 2: In tiêu đề header */
    printf("\nAvailable Commands:\n");
    printf("-------------------\n");
    
    /* Bước 3: Duyệt qua từng lệnh trong bảng và in thông tin */
    for (i = 0; i < table_size; i++) {
        /* Bước 3.1: Kiểm tra tên lệnh có hợp lệ không */
        if (table[i].name != NULL) {
            /* Bước 3.2: In tên lệnh (căn trái 12 ký tự) và mô tả */
            /* Nếu help là NULL thì in "No description" */
            printf("  %-12s - %s\n", table[i].name, 
                   (table[i].help != NULL) ? table[i].help : "No description");
        }
    }
    
    /* Bước 4: In footer và tổng số lệnh */
    printf("-------------------\n");
    printf("Total: %d commands\n\n", table_size);
}

/**
 * @brief Đếm số lượng lệnh hợp lệ trong bảng
 * 
 * Mô tả:
 *   Hàm này sẽ đếm số lượng entry có tên lệnh khác NULL
 *   trong bảng. Hữu ích để biết có bao nhiêu lệnh available
 *   mà không cần phải tự theo dõi kích thước bảng.
 * 
 * Các bước thực hiện:
 *   Bước 1: Kiểm tra table có NULL không
 *   Bước 2: Lặp qua các phần tử, đếm từng phần tử có tên != NULL
 *   Bước 3: Dừng khi gặp phần tử có tên == NULL (sentinel)
 *   Bước 4: Trả về số lượng đã đếm được
 * 
 * @param table  Con trỏ tới bảng lệnh
 * @return       Số lượng lệnh hợp lệ
 */
uint16_t my_cmd_get_count(const my_cmd_entry_t* table) {
    uint16_t count = 0;
    
    /* Bước 1: Kiểm tra table có NULL không */
    if (table == NULL) {
        return 0;
    }
    
    /* Bước 2 & 3: Đếm các phần tử có tên khác NULL, dừng khi gặp NULL */
    while (table[count].name != NULL) {
        count++;
    }
    
    /* Bước 4: Trả về kết quả */
    return count;
}
