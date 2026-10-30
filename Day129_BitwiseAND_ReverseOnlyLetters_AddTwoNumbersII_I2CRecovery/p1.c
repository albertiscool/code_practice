#include <stdio.h>

/*
 * LeetCode #201 - Bitwise AND of Numbers Range
 *
 * Description:
 * Given two integers left and right that represent the range [left, right],
 * return the bitwise AND of all numbers in this range, inclusive.
 *
 * Constraints:
 * 0 <= left <= right <= 2^31 - 1
 */

int rangeBitwiseAnd(int left, int right) {
    // 最佳解法：Brian Kernighan 抹除最低位 1 / 公共二進位前綴
    // 當 right > left 時，表示區間內必定跨越二進位跳變，最低位的 1 在連續 AND 後必然變為 0
    while (right > left) {
        right &= (right - 1);
    }
    return right;
}

/*
 * =========================================================================
 * 原始白板實作與盲點深度剖析 (Preserved User's Original Code & Blind Spots):
 * =========================================================================
 * 原始程式碼：
 * int rangeBitwiseAnd(int left, int right) {
 *     int result = left;
 *     for(int i = left + 1; i <= right; i++)
 *     {
 *         result &= i;
 *     }
 *     return result;
 * }
 *
 * 盲點深度剖析：
 * 1. 致命的有號整數溢位 (Signed Integer Overflow) 與死迴圈：
 *    題目條件 0 <= left <= right <= 2^31 - 1 (INT_MAX = 2147483647)。
 *    當 right = 2147483647 時，迴圈執行到 i = 2147483647 結束後執行 i++，
 *    32-bit 有號整數會發生溢位變成負數 (-2147483648)，
 *    導致 i <= right 條件永遠為真，陷入無窮迴圈當機！
 * 
 * 2. 演算法時間複雜度超時 (Time Limit Exceeded, TLE)：
 *    若 left = 1, right = 2147483647，O(N) 迴圈需跑 20 億次，
 *    在 LeetCode 與嵌入式系統中必然超時！
 * 
 * 3. 韌體位元運算本質：公共前綴 (Common Binary Prefix)
 *    只要 left 與 right 不同，區間內所有較低的位元必然會在遞增過程中至少經歷一次 0。
 *    位元 AND 只要遇到一個 0 就全為 0，因此最終結果就是 left 與 right 的「二進位最高公共前綴」。
 *    - 解法 A：同時右移 left 與 right 直到相等，再左移回來 (O(log N))。
 *    - 解法 B：Brian Kernighan 技巧，不斷 right &= (right - 1) 抹除最低位 1 直到 <= left (極致簡潔)。
 * =========================================================================
 */

int main(void) {
    // Test Case 1: left = 5, right = 7 -> Expected: 4
    // 5 = 101, 6 = 110, 7 = 111 -> 5 & 6 & 7 = 100 (4)
    printf("Test 1 (5, 7): %d (Expected: 4)\n", rangeBitwiseAnd(5, 7));

    // Test Case 2: left = 0, right = 0 -> Expected: 0
    printf("Test 2 (0, 0): %d (Expected: 0)\n", rangeBitwiseAnd(0, 0));

    // Test Case 3: left = 1, right = 2147483647 -> Expected: 0
    printf("Test 3 (1, 2147483647): %d (Expected: 0)\n", rangeBitwiseAnd(1, 2147483647));

    // Test Case 4: left = 6, right = 7 -> Expected: 6
    printf("Test 4 (6, 7): %d (Expected: 6)\n", rangeBitwiseAnd(6, 7));

    return 0;
}
