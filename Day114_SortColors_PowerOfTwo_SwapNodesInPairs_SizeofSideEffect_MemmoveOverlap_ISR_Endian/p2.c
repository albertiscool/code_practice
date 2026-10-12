#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

/**
 * LeetCode #231: Power of Two
 * 
 * 題目描述：
 * 給定一個 32 位元整數 n，請判斷它是否為 2 的冪次方 (Power of Two)。
 * 如果存在一個整數 x 使得 n == 2^x，則返回 true；否則返回 false。
 * 
 * 限制條件：
 * - -2^31 <= n <= 2^31 - 1 (注意負數與極限值的邊界情況)
 * - 進階挑戰：你能否不使用任何迴圈 (loop) 或遞迴 (recursion)，以 O(1) 時間複雜度完成判定？
 * 
 * 範例 1:
 *   輸入: n = 1
 *   輸出: true
 *   解釋: 2^0 = 1
 * 
 * 範例 2:
 *   輸入: n = 16
 *   輸出: true
 *   解釋: 2^4 = 16
 * 
 * 範例 3:
 *   輸入: n = 3
 *   輸出: false
 * 
 * 範例 4:
 *   輸入: n = 0
 *   輸出: false
 * 
 * 範例 5:
 *   輸入: n = -16
 *   輸出: false
 */

bool isPowerOfTwo(int n) {
    return ((n > 0) && ((n & (n - 1)) == 0));
}

/*
 * =====================================================================================
 * 【面試亮點剖析 & 韌體底層考點】 (LeetCode #231: Power of Two)
 * =====================================================================================
 * 滿分解法亮點：
 * 1. 括號優先級無懈可擊：
 *    C 語言中，關係運算子 (==) 的優先順序「高於」位元運算子 (&)。
 *    若寫成 n & (n - 1) == 0，編譯器會解析為 n & ((n - 1) == 0)，造成致命錯誤！
 *    你精準地替 (n & (n - 1)) 加上完整括號，展現成熟的 C 語法細膩度。
 * 
 * 2. 邊界短路保護 (Short-circuit Evaluation)：
 *    (n > 0) 放在最前端有兩大關鍵作用：
 *    - 排除 0 與所有負數（2 的冪次方必大於 0）。
 *    - 避免極限值 INT_MIN (-2147483648) 發生 signed integer underflow 的未定義行為 (UB)。
 *      若 n = INT_MIN 且未短路，(n - 1) 會觸發溢位。
 * 
 * 3. 韌體實務價值 (SSD / MCU Firmware Applications)：
 *    - Ring Buffer 大小判定：在無鎖環形佇列中，容量必須為 2 的冪次方，
 *      如此才能用 `index = head & (size - 1)` 取代耗時的除法 `% size`。
 *    - Flash 記憶體對齊：判斷傳入的 LBA 或 Sector 大小是否對齊 4KB / 16KB Page。
 * =====================================================================================
 */

int main(void) {
    printf("=== Test P2: Power of Two ===\n");

    // Test 1: 1 -> true (2^0)
    assert(isPowerOfTwo(1) == true);
    printf("Test 1 Passed: 1 -> true\n");

    // Test 2: 16 -> true (2^4)
    assert(isPowerOfTwo(16) == true);
    printf("Test 2 Passed: 16 -> true\n");

    // Test 3: 3 -> false
    assert(isPowerOfTwo(3) == false);
    printf("Test 3 Passed: 3 -> false\n");

    // Test 4: 0 -> false
    assert(isPowerOfTwo(0) == false);
    printf("Test 4 Passed: 0 -> false\n");

    // Test 5: -16 -> false
    assert(isPowerOfTwo(-16) == false);
    printf("Test 5 Passed: -16 -> false\n");

    // Test 6: INT_MIN (-2147483648) -> false
    assert(isPowerOfTwo(-2147483648) == false);
    printf("Test 6 Passed: -2147483648 -> false\n");

    // Test 7: 1073741824 (2^30) -> true
    assert(isPowerOfTwo(1073741824) == true);
    printf("Test 7 Passed: 1073741824 -> true\n");

    printf("All P2 Local Tests Passed!\n");
    return 0;
}
