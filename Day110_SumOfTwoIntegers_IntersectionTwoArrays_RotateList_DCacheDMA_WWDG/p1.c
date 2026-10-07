#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #371: Sum of Two Integers
 * 
 * 題目描述：
 * 給定兩個整數 a 與 b，在「不使用」算術運算子 + 與 - 的前提下，計算並回傳兩數之和。
 * 
 * 限制條件：
 * - -1000 <= a, b <= 1000
 * 
 * 範例 1:
 *   輸入: a = 1, b = 2
 *   輸出: 3
 * 
 * 範例 2:
 *   輸入: a = 2, b = 3
 *   輸出: 5
 */

int getSum(int a, int b) {
    // 循環處理進位，直到沒有進位為止 (b == 0)
    while (b != 0) {
        // 1. 抓出進位值：必須先強制轉型為 unsigned int，避免負數在 C 語言中有號左移未定義行為 (UB)
        unsigned int carry = ((unsigned int)(a & b)) << 1;

        // 2. 本位和：XOR 互斥或運算
        a = a ^ b;

        // 3. 將進位值帶入下一輪運算
        b = (int)carry;
    }

    return a;
}

int main(void) {
    printf("=== Test P1: Sum of Two Integers ===\n");
    assert(getSum(1, 2) == 3);
    printf("Test 1 Passed: 1 + 2 = 3\n");

    assert(getSum(2, 3) == 5);
    printf("Test 2 Passed: 2 + 3 = 5\n");

    assert(getSum(-1, 1) == 0);
    printf("Test 3 Passed: -1 + 1 = 0\n");

    assert(getSum(-2, -3) == -5);
    printf("Test 4 Passed: -2 + -3 = -5\n");

    assert(getSum(1, 3) == 4);
    printf("Test 5 Passed: 1 + 3 = 4\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 原始嘗試盲點剖析與韌體架構深入複盤】
 * =========================================================================
 * 
 * 原始撰寫程式碼：
 * -------------------------------------------------------------------------
 * int getSum(int a, int b) {
 *     int carry = (a & b) << 1;
 *     int sum = (a ^ b);
 *     
 *     while(carry != 0)
 *     {
 *         sum = (sum ^ carry);
 *         carry = (sum & carry) << 1;
 *     }
 * 
 *     return sum;
 * }
 * -------------------------------------------------------------------------
 * 
 * ❌ 盲點 1：變數覆寫順序陷阱（Variable Mutation Hazard 導致死迴圈）
 * -------------------------------------------------------------------------
 * 在迴圈內部：
 *   sum = (sum ^ carry);         // 第 1 步：sum 被提前更新為「新的 sum」！
 *   carry = (sum & carry) << 1;  // 第 2 步：計算 carry 時，拿到的不是「舊 sum」，而是「新 sum」！
 * 
 * 數學推演（以 a = 2 (0b010), b = 3 (0b011) 為例）：
 *   - 進入迴圈前：
 *       sum = a ^ b = 2 ^ 3 = 1 (0b001)
 *       carry = (a & b) << 1 = (2) << 1 = 4 (0b100)
 *   - 進入迴圈第 1 次迭代：
 *       sum = sum ^ carry = 1 ^ 4 = 5 (0b101)  <-- sum 變成了 5！
 *       carry = (sum & carry) << 1 = (5 & 4) << 1 = (4) << 1 = 8 (0b1000)！
 *       --> carry 不僅沒有縮小到 0，反而從 4 膨脹變成了 8！
 *   - 第 2 次迭代：
 *       sum = 5 ^ 8 = 13 (0b01101)
 *       carry = (13 & 8) << 1 = 16 (0b10000)
 *   --> 系統直接陷入無窮死迴圈 (Infinite Loop) 並最終整數溢位崩潰！
 * 
 * 💡 破解之道：
 * 必須在 sum 被覆寫之前，先用臨時變數暫存 carry，或者直接讓 a 代表本位和、b 代表進位：
 *   unsigned int carry = ((unsigned int)(a & b)) << 1;
 *   a = a ^ b;
 *   b = carry;
 * 
 * -------------------------------------------------------------------------
 * ❌ 盲點 2：負數左移之 C 語言未定義行為（Signed Left Shift UB）
 * -------------------------------------------------------------------------
 * 當輸入包含負數（如 a = -1, b = 1）時：
 *   a & b 在底層二補數下最高位符號位為 1（負數）。
 * 在 C99/C11 標準中，對「負數」執行有號左移 (Signed <<) 是標準的 Undefined Behavior，
 * 在 LeetCode 或 GCC 開啟優化時會直接報出：
 *   "runtime error: left shift of negative value -2147483648"
 * 
 * 💡 工業級韌體解法：
 * 執行位元移位前，強制轉型成無號數：
 *   unsigned int carry = ((unsigned int)(a & b)) << 1;
 * 運算完成後再以有號整數帶入。
 * ========================================================================= */
