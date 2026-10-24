#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * LeetCode #345 - Reverse Vowels of a String
 * 
 * 題目描述：
 * 給定一個字串 s，僅反轉字串中的所有母音字母，並回傳結果。
 * 母音字母包含 'a', 'e', 'i', 'o', 'u'，且包含大寫與小寫形式，可能出現不止一次。
 * 
 * 範例 1：
 * 輸入：s = "IceCreAm"
 * 輸出："AceCreIm"
 * 解釋：s 中的母音為 ['I', 'e', 'e', 'A']。反轉這些母音後，字串變為 "AceCreIm"。
 * 
 * 範例 2：
 * 輸入：s = "leetcode"
 * 輸出："leotcede"
 * 
 * 限制條件：
 * 1 <= s.length <= 3 * 10^5
 * s 由可列印的 ASCII 字元組成
 */

/*
 * =========================================================================
 * 【LeetCode 官方提交成績】：
 * - 狀態：Accepted (通過 481/481 測試案例)
 * - 執行時間：0 ms (擊敗 100.00% C 語言提交)
 * - 記憶體：9.3 MB
 * - Submission ID：2180220452
 * -------------------------------------------------------------------------
 * 【使用者原始白板程式碼】：
 * char* reverseVowels(char* s) {
 *     int len = strlen(s);
 *     int left = 0;
 *     int right = len - 1;
 *     while(left < right)
 *     {
 *         while(strchr('aeiouAEIOU',s[left]) == NULL) // 盲點 1：單引號是整數常數，編譯報錯；盲點 2：缺少 left < right 邊界保護
 *         {
 *             left++;
 *         }
 *         while(strchr('aeiouAEIOU',s[right]) == NULL)
 *         {
 *             right--;
 *         }
 *         char temp = s[left];
 *         s[left] = s[right];
 *         s[right] = temp;
 *         left++;
 *         right--;
 *     }
 *     return s;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 編譯錯誤（字元常數 vs 字串字面量）：
 *    - 在 C 語言中，單引號 `'...'` 代表單一字元或多字元整數常數（int）。
 *    - strchr(const char *s, int c) 的第一個參數必須是字串指標（雙引號 "aeiouAEIOU"）。
 *      寫成單引號會引發 passing argument 1 of strchr makes pointer from integer without a cast 編譯錯誤！
 * 2. 邊界檢查缺失（緩衝區越界 Buffer Overflow）：
 *    - 內部 while 迴圈沒有加入 left < right 檢查。
 *      若遇到完全沒有母音的字串（如 "rhythm" 或 "bcd"），left 會一路遞增直到陣列越界當機！
 * 3. 交換前守衛：
 *    - 在執行 temp 交換前，應以 if (left < right) 保護，避免指標交錯時發生非母音的錯誤交換。
 * =========================================================================
 */

static inline bool isVowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u' ||
           c == 'A' || c == 'E' || c == 'I' || c == 'O' || c == 'U';
}

char* reverseVowels(char* s) {
    int left = 0;
    int right = strlen(s) - 1;

    while (left < right) {
        // 短路求值邊界防禦：尋找左側母音
        while (left < right && !isVowel(s[left])) {
            left++;
        }
        // 短路求值邊界防禦：尋找右側母音
        while (left < right && !isVowel(s[right])) {
            right--;
        }

        if (left < right) {
            char temp = s[left];
            s[left] = s[right];
            s[right] = temp;
            left++;
            right--;
        }
    }
    return s;
}

int main() {
    char s1[] = "IceCreAm";
    printf("Test 1: %s -> %s (Expected: AceCreIm)\n", "IceCreAm", reverseVowels(s1));

    char s2[] = "leetcode";
    printf("Test 2: %s -> %s (Expected: leotcede)\n", "leetcode", reverseVowels(s2));

    char s3[] = "aA";
    printf("Test 3: %s -> %s (Expected: Aa)\n", "aA", reverseVowels(s3));

    return 0;
}
