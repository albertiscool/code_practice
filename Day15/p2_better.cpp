void reverse_string(char *str) {
    // 1. 防呆：如果是空指標就直接結束
    if (str == NULL) return;

    // 2. 宣告兩個指標，一個站頭，一個站尾
    char *start = str;
    char *end = str;

    // 3. 讓 end 指標一路往後衝，直到撞到字串結尾的 '\0'
    while (*end != '\0') {
        end++;
    }
    // 這時候 end 踩在 '\0' 上，我們要讓它往後退一步，踩在最後一個有效字元上
    end--;

    // 4. 開始首尾互換，直到兩個人相遇或交錯
    while (start < end) {
        // 利用你最熟悉的 swap 邏輯，直接交換這兩個地址裡的值
        char temp = *start;
        *start = *end;
        *end = temp;

        // start 往右走一步，end 往左走一步 (完全不需要 i 變數)
        start++;
        end--;
    }
}