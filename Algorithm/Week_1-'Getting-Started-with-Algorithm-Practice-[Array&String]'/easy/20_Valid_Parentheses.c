bool isValid(char* s) {
    int len = strlen(s);

    // stack đơn giản bằng array
    char stack[len];
    int top = -1;

    for (int i = 0; i < len; i++) {
        char c = s[i];

        // nếu là opening → push
        if (c == '(' || c == '{' || c == '[') {
            stack[++top] = c;
        } 
        else {
            // nếu stack rỗng → sai
            if (top == -1) return false;

            char topChar = stack[top--];

            // check matching
            if ((c == ')' && topChar != '(') ||
                (c == ']' && topChar != '[') ||
                (c == '}' && topChar != '{')) {
                return false;
            }
        }
    }

    // cuối cùng stack phải rỗng
    return top == -1;
}