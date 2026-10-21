#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * LeetCode #415 - Add Strings
 * 
 * 題目描述：
 * 給定兩個以字串形式表示的非負整數 num1 和 num2，
 * 請以字串形式回傳它們的「和」。
 * 
 * 限制條件：
 * 1 <= num1.length, num2.length <= 10^4
 * num1 和 num2 只包含數字 '0'-'9'
 * num1 和 num2 都不包含任何前導零，除了數字 0 本身
 */

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * -------------------------------------------------------------------------
 * char* addStrings(char* num1, char* num2) {
 *     int len1 = strlen(num1);
 *     int len2 = strlen(num2);
 *     int len = ((len1 > len2) ? len1 : len2) + 1;
 *     char* result = (char*) malloc(len * sizeof(char));
 *     int curr = 0;
 *     int sum = 0;
 *     int carry = 0;
 *     while(len1 > 0 || len2 > 0 || carry > 0)
 *     {
 *         if(len1 > 0)
 *         {
 *             sum += num1[curr] - '0';
 *             len1--;
 *         }
 *         if(len2 > 0)
 *         {
 *             sum += num2[curr] - '0';
 *             len2--;
 *         }
 *         sum += carry;
 *         if(sum > 9)
 *         {
 *             sum -= 10;
 *             carry = 1;
 *         }
 *         else
 *         {
 *             carry = 0;
 *         }
 *         result[curr] = sum + '0';
 *         curr++;
 *         sum = 0;
 *     }
 *     return result;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 對齊與加法方向顛倒（高位 vs 低位）：
 *    - 原始程式碼從 `curr = 0` 開始加，這是加法的「最高位」而非「個位數」！
 *      例如 "11" 和 "123"，num1[0] 是 '1'（十位），num2[0] 是 '1'（百位），
 *      直接相加會導致數位相對位置完全錯位。
 *    - 正確做法：必須比照小學直式加法，從字串最右側（個位數）開始向前推進：
 *      `int i = strlen(num1) - 1; int j = strlen(num2) - 1;`
 * 2. 緩衝區大小不足與缺失 '\0' 字串結束符：
 *    - 若有進位，結果字串長度最大為 max(len1, len2) + 1，再加上結尾的 '\0'，
 *      因此至少需要配置 `max_len + 2` 個字元空間。
 *    - 原始程式碼沒有在結尾補上 `'\0'`，導致輸出時持續越界讀取記憶體（Heap-buffer-overflow）。
 * 3. 結果反轉：
 *    - 從個位數算起的結果是倒序存入 result（如 '4', '3', '1'），
 *      計算完成後只需透過雙指標前後對調翻轉，即可得到正確答案 "134"！
 * =========================================================================
 */

char* addStrings(char* num1, char* num2) {
    int i = strlen(num1) - 1;
    int j = strlen(num2) - 1;
    int max_len = (i > j ? i : j) + 2; // 預留進位最高位與 '\0'
    char* result = (char*)malloc((max_len + 1) * sizeof(char));

    int k = 0;
    int carry = 0;

    // 從個位數開始往前逐位相加
    while (i >= 0 || j >= 0 || carry > 0) {
        int sum = carry;
        if (i >= 0) {
            sum += num1[i] - '0';
            i--;
        }
        if (j >= 0) {
            sum += num2[j] - '0';
            j--;
        }

        result[k++] = (sum % 10) + '0';
        carry = sum / 10;
    }
    result[k] = '\0'; // 嚴謹補上 C 語言字串結束符

    // 雙指標原地反轉為正常的高位在左順序
    int left = 0, right = k - 1;
    while (left < right) {
        char temp = result[left];
        result[left] = result[right];
        result[right] = temp;
        left++;
        right--;
    }

    return result;
}

int main() {
    char* r1 = addStrings("11", "123");
    printf("Test 1: %s (Expected: 134)\n", r1);
    free(r1);

    char* r2 = addStrings("456", "77");
    printf("Test 2: %s (Expected: 533)\n", r2);
    free(r2);

    char* r3 = addStrings("0", "0");
    printf("Test 3: %s (Expected: 0)\n", r3);
    free(r3);

    return 0;
}
