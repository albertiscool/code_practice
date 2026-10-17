#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #1342: Number of Steps to Reduce a Number to Zero
 * (群聯電訪/筆試精選：位元奇偶判斷、右移除法與計數操作)
 * 
 * 題目描述：
 * 給定一個非負整數 num，請返回將它減少到 0 所需的操作步數。
 * 在每一步操作中：
 * - 如果當前數字是「偶數」，將它除以 2；
 * - 如果當前數字是「奇數」，將它減去 1。
 * 
 * 限制條件：
 * - 0 <= num <= 10^6
 * - 挑戰目標：你能否在時間複雜度 O(log N) 內完成？思考如何用位元運算子 (&、>>) 替代除法與模運算。
 * 
 * 範例 1:
 *   輸入: num = 14
 *   輸出: 6
 *   解釋:
 *     步驟 1) 14 是偶數，除以 2 得到 7
 *     步驟 2) 7 是奇數，減 1 得到 6
 *     步驟 3) 6 是偶數，除以 2 得到 3
 *     步驟 4) 3 是奇數，減 1 得到 2
 *     步驟 5) 2 是偶數，除以 2 得到 1
 *     步驟 6) 1 是奇數，減 1 得到 0
 * 
 * 範例 2:
 *   輸入: num = 8
 *   輸出: 4
 *   解釋: 8 -> 4 -> 2 -> 1 -> 0
 * 
 * 範例 3:
 *   輸入: num = 123
 *   輸出: 12
 * 
 * 範例 4:
 *   輸入: num = 0
 *   輸出: 0
 */

int numberOfSteps(int num) {
    // 【使用者原始白板實作】(Accepted 0 ms / Beats 100%)
    int count = 0;
    while(num != 0)
    {
        if((num % 2) == 0)
        {
            num = num >> 1;
        }
        else
        {
            num--;
        }
        count++;
    }

    return count;
}

/*
 * =====================================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * =====================================================================================
 * int numberOfSteps(int num) {
 *     int count = 0;
 *     while(num != 0)
 *     {
 *         if((num % 2) == 0)
 *         {
 *             num = num >> 1;
 *         }
 *         else
 *         {
 *             num--;
 *         }
 *         count++;
 *     }
 *     return count;
 * }
 * =====================================================================================
 * 【面試亮點剖析 & 韌體位元極限優化】 (LeetCode #1342: Number of Steps)
 * =====================================================================================
 * 
 * 1. 滿分解法亮點 (一次 AC，0 ms / Beats 100%)：
 *    - 精準掌握 while (num != 0) 終止條件，完美通吃 num = 0 的邊界。
 *    - 偶數時正確使用右移 `num = num >> 1` 代替除法，展現出良好的韌體直覺！
 * 
 * 2. 韌體細節再提升 (Bitwise & 1 淘汰模除)：
 *    - 在無硬體除法器（Hardware Divider）的低階 MCU（如 ARM Cortex-M0/M0+）中，
 *      `(num % 2)` 可能會調用軟體除法副程式（__aeabi_idivmod），耗費數十個 CPU 週期。
 *    - 改寫為 `if ((num & 1) == 0)`，編譯器只需一條單週期 AND 指令，效率更高！
 * 
 * 3. 群聯/IC 廠面試官最愛的「硬體架構級思維 (Popcount + CLZ)」：
 *    - 深入二進位本質：
 *      * 只要某個 bit 是 1，就必須經歷 1 次「減 1」操作把它抹平。
 *      * 只要數字大於 0，每前進到下一個更高位，就必須經歷 1 次「右移除以 2」操作。
 *    - 數學規律：
 *      總操作步數 = (最高位 1 所在的位元長度 - 1) + (所有 bit 1 的總數)。
 *    - 範例 num = 14 (二進位 1110)：
 *      * 有效位元長度 = 4 bits (從 bit 3 到 bit 0)
 *      * 1 的數量 (Popcount) = 3
 *      * 總步數 = (4 - 1) + 3 = 6 步！完全相符！
 *    - 利用 ARM Cortex-M 指令 `CLZ` (Count Leading Zeros) 與 Popcount，
 *      可在真正的常數時間 O(1) 內一行算完：
 *      `return (num == 0) ? 0 : (31 - __builtin_clz(num)) + __builtin_popcount(num);`
 * =====================================================================================
 */

int main(void) {
    printf("=== Test P1: Number of Steps to Reduce a Number to Zero ===\n");

    // Test 1: num = 14 -> 6
    assert(numberOfSteps(14) == 6);
    printf("Test 1 Passed: num = 14 -> 6\n");

    // Test 2: num = 8 -> 4
    assert(numberOfSteps(8) == 4);
    printf("Test 2 Passed: num = 8 -> 4\n");

    // Test 3: num = 123 -> 12
    assert(numberOfSteps(123) == 12);
    printf("Test 3 Passed: num = 123 -> 12\n");

    // Test 4: num = 0 -> 0
    assert(numberOfSteps(0) == 0);
    printf("Test 4 Passed: num = 0 -> 0\n");

    // Test 5: num = 1 -> 1
    assert(numberOfSteps(1) == 1);
    printf("Test 5 Passed: num = 1 -> 1\n");

    printf("All P1 Local Tests Passed!\n");
    return 0;
}
