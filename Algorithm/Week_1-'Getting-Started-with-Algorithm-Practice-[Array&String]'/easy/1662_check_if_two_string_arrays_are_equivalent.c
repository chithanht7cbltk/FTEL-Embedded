bool arrayStringsAreEqual(char** word1, int word1Size, char** word2, int word2Size) {
    int i = 0, j = 0;  // index word
    int p = 0, q = 0;  // index char

    while (i < word1Size && j < word2Size) {
        // so sánh ký tự hiện tại
        if (word1[i][p] != word2[j][q]) {
            return false;
        }

        p++;
        q++;

        // nếu hết string con của word1
        if (p == strlen(word1[i])) {
            i++;
            p = 0;
        }

        // nếu hết string con của word2
        if (q == strlen(word2[j])) {
            j++;
            q = 0;
        }
    }

    // phải cùng kết thúc
    return (i == word1Size && j == word2Size);
}