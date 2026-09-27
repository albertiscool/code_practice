#include <stdio.h>
#include <limits.h>
#include <stdbool.h>

/* 
 * 題目 3: 實作 myAtoi 字串轉整數 (LeetCode #8 - String to Integer (atoi))
 * 難度: 🟡 瑞昱 / 群聯 / 系統廠 韌體工程師必考！字串指標走訪與溢位防禦
 * 
 * 題目說明:
 * 請實作 myAtoi(const char* s) 函式，將字串解析並轉換為 32 位元有號整數：
 * 1. 忽略前導空白：跳過開頭所有的空格 ' '。
 * 2. 正負號檢查：檢查下一個字元是否為 '+' 或 '-' (若無符號則預設為正數)。
 * 3. 數字讀取：連續讀取後續的數字字元 ('0' 到 '9')，直到遇到非數字字元或字串結尾為止。
 * 4. 32 位元溢位飽和截斷：
 *    - 若數值大於 2^31 - 1，回傳 INT_MAX (2147483647)。
 *    - 若數值小於 -2^31，回傳 INT_MIN (-2147483648)。
 * 5. 若無有效數字讀入，回傳 0。
 * 
 * 範例 1:
 *   輸入: s = "42"
 *   輸出: 42
 * 
 * 範例 2:
 *   輸入: s = "   -042"
 *   輸出: -42
 * 
 * 範例 3:
 *   輸入: s = "1337c0d3"
 *   輸出: 1337 (遇到 'c' 停止讀取)
 * 
 * 範例 4:
 *   輸入: s = "0-1"
 *   輸出: 0 (讀取 '0' 後遇到 '-' 終止)
 * 
 * 範例 5:
 *   輸入: s = "-91283472332"
 *   輸出: -2147483648 (低於 INT_MIN，截斷飽和)
 * 
 * 限制條件:
 * - 只能使用 32 位元整數空間 (或使用常數防護乘 10 溢位判定)
 */
int myAtoi(const char* s) {
    if (s == NULL) {
        return 0;
    }

    int i = 0;
    int sign = 1;
    int result = 0;

    // 1. 跳過前導空格
    while (s[i] == ' ') {
        i++;
    }

    // 2. 正負號檢查 (必須同時處理 '+' 與 '-')
    if (s[i] == '+' || s[i] == '-') {
        sign = (s[i] == '-') ? -1 : 1;
        i++;
    }

    // 3. 逐字讀取數字，並進行 32 位元溢位飽和防護
    while (s[i] >= '0' && s[i] <= '9') {
        int digit = s[i] - '0';

        // 溢位判斷 (INT_MAX = 2147483647, INT_MAX / 10 = 214748364)
        // 情況 A: result > 214748364，乘 10 必定溢位
        // 情況 B: result == 214748364，且個位數 digit > 7，加上去必定溢位
        if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7)) {
            return (sign == 1) ? INT_MAX : INT_MIN;
        }

        result = result * 10 + digit;
        i++; // 關鍵！必須推進指標，避免無窮迴圈
    }

    return sign * result;
}

/* 
 * ============================================================================
 * 🔍【原始代碼盲點檢視與解析】
 * ============================================================================
 * 你原先寫的代碼：
 * 
 * int myAtoi(const char* s) {
 *     int i = 0;
 *     int result = 0;
 *     int minus = 1;
 *     while(s[i] == ' ') {
 *         i++;
 *     }
 *     if(s[i] == '-') { // 盲點 1: 缺少對 '+' 號的處理，若輸入 "+123" 會讀不到數字
 *         minus = -1;
 *         i++;
 *     }
 *     while((s[i] >= '0') && (s[i] <= '9')) {
 *         // 盲點 2: 數學式兩邊同時消去 (s[i]-'0')，等價於只判斷 result > INT_MAX / 10，
 *         // 漏掉了「result == INT_MAX / 10 且尾數 > 7」的關鍵邊界！
 *         if(result + (int) (s[i] - '0') > (INT_MAX / 10 + (int) (s[i] - '0'))) {
 *             return minus*INT_MAX; // 盲點 3: 負數下限是 INT_MIN (-2147483648)，
 *                                   // 而 minus * INT_MAX 只有 -2147483647 (少扣了 1)
 *         }
 *         else {
 *             result = result * 10 + (int) (s[i] - '0');
 *         }
 *         // 盲點 4: 致命無窮迴圈！迴圈內完全沒有寫 i++，導致程式卡死在同一個字元！
 *     }
 *     return minus*result;
 * }
 * 
 * 💡【白板題解題心法 - 32 位元整數溢位防禦公式】
 * 記住 32-bit INT_MAX = 2147483647：
 * 只要記住兩道防線：
 * 1. result > INT_MAX / 10 (已經大於 214748364，乘 10 必爆)
 * 2. result == INT_MAX / 10 && digit > 7 (剛好是 214748364，個位數超過 7 必爆)
 * 觸發時直接回傳 (sign == 1) ? INT_MAX : INT_MIN。
 * ============================================================================
 */

int main(void) {
    printf("=== Day 101 - 白板題 3: 實作 myAtoi 字串轉整數 (LeetCode #8) ===\n\n");

    const char* tests[] = {
        "42",
        "   -042",
        "1337c0d3",
        "0-1",
        "-91283472332",
        "words and 987",
        "+-12",
        "21474836460"
    };

    int expected[] = {
        42,
        -42,
        1337,
        0,
        INT_MIN,
        0,
        0,
        INT_MAX
    };

    int num_tests = sizeof(tests) / sizeof(tests[0]);
    for (int i = 0; i < num_tests; i++) {
        int res = myAtoi(tests[i]);
        printf("Test %d: s = \"%s\"\n", i + 1, tests[i]);
        printf("  預期輸出: %d\n", expected[i]);
        printf("  實際輸出: %d (%s)\n\n", res, res == expected[i] ? "PASS" : "FAIL");
    }

    return 0;
}
