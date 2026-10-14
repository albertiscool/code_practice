#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #338: Counting Bits
 * 
 * 題目描述：
 * 給定一個非負整數 n，對於 0 <= i <= n 中的每一個整數 i，
 * 計算其二進位表示法中 1 的個數 (Popcount / Hamming Weight)，
 * 並以一個長度為 n + 1 的動態整數陣列返回結果。
 * 
 * 限制條件：
 * - 0 <= n <= 10^5
 * - 進階挑戰：你能否在線性時間 O(N) 且僅單次遍歷 (Single-pass) 的情況下完成？
 * - 嚴禁直接呼叫內建函式 (如 __builtin_popcount)。
 * 
 * 範例 1:
 *   輸入: n = 2
 *   輸出: [0,1,1]
 *   解釋:
 *     0 --> 0
 *     1 --> 1
 *     2 --> 10 (1 個 1)
 * 
 * 範例 2:
 *   輸入: n = 5
 *   輸出: [0,1,1,2,1,2]
 *   解釋:
 *     0 --> 0
 *     1 --> 1
 *     2 --> 10 (1 個 1)
 *     3 --> 11 (2 個 1)
 *     4 --> 100 (1 個 1)
 *     5 --> 101 (2 個 1)
 */

int* countBits(int n, int* returnSize) {
    *returnSize = n + 1;
    int* result = (int*)malloc((*returnSize) * sizeof(int));
    result[0] = 0;

    for (int i = 1; i <= n; i++) {
        // 利用位元動態規劃 (Bitwise DP):
        // i & (i - 1) 會消去 i 的最低位 1，而 result[i & (i - 1)] 之前早已算好！
        // 因此 i 的 1 的個數就是 result[i & (i - 1)] 再加上剛剛消去的那 1 個！
        result[i] = result[i & (i - 1)] + 1;
    }

    return result;
}

/*
 * =====================================================================================
 * 【面試盲點剖析 & 邊界除錯精華】 (LeetCode #338: Counting Bits)
 * =====================================================================================
 * 原始盲點剖析：
 * 1. 誤改外層迴圈上限參數 n (變數污染導致提早退出)：
 *    原程式碼在內層迴圈寫了：
 *    `while(n != 0) { n = n & (n - 1); count++; }`
 *    - 當外層進入 i = 0 時，內層迴圈把 n 自身不斷抹除直到 n 變成 0！
 *    - 回到外層 for 迴圈後，檢查條件 `i <= n` (即 1 <= 0) 瞬間為 false，整個迴圈提早結束！
 *      導致無論輸入多少，輸出永遠只有長度為 1 的 [0]！
 *    - 即使要用暴力法，也必須用臨時變數 `int temp = i; while(temp != 0)...`，絕不可破壞 n。
 * 
 * 2. 演算法維度升級：從 O(N log N) 暴力法跨入 O(N) 位元動態規劃 (Bitwise DP)：
 *    - 若對每個數字都跑 while 迴圈數 1，總時間複雜度為 O(N * 32) = O(N log N)。
 *    - 但請注意：你用的 `n & (n - 1)` 其實正是最頂級的動態規劃轉移方程！
 *      `i & (i - 1)` 的值必然嚴格小於 i，代表它的答案在前面的迴圈「早就已經算過並存在 result 裡了」！
 *    - 因此：`result[i] = result[i & (i - 1)] + 1;`
 *      只需一行，0 迴圈、0 分支，單次遍歷 O(N) 直接秒殺！
 * 
 * 另解 (最低位奇偶推導法)：
 *    `result[i] = result[i >> 1] + (i & 1);`
 * =====================================================================================
 */

int main(void) {
    printf("=== Test P2: Counting Bits ===\n");

    // Test 1: n = 2 -> [0, 1, 1]
    int size1 = 0;
    int* ans1 = countBits(2, &size1);
    assert(size1 == 3);
    assert(ans1[0] == 0 && ans1[1] == 1 && ans1[2] == 1);
    free(ans1);
    printf("Test 1 Passed: n = 2 -> [0, 1, 1]\n");

    // Test 2: n = 5 -> [0, 1, 1, 2, 1, 2]
    int size2 = 0;
    int* ans2 = countBits(5, &size2);
    assert(size2 == 6);
    int exp2[] = {0, 1, 1, 2, 1, 2};
    for (int i = 0; i < 6; i++) {
        assert(ans2[i] == exp2[i]);
    }
    free(ans2);
    printf("Test 2 Passed: n = 5 -> [0, 1, 1, 2, 1, 2]\n");

    // Test 3: n = 0 -> [0]
    int size3 = 0;
    int* ans3 = countBits(0, &size3);
    assert(size3 == 1 && ans3[0] == 0);
    free(ans3);
    printf("Test 3 Passed: n = 0 -> [0]\n");

    printf("All P2 Local Tests Passed!\n");
    return 0;
}
