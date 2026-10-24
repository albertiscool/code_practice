#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #1486 - XOR Operation in an Array
 * 
 * 題目描述：
 * 給定兩個整數 n 和 start。
 * 定義陣列 nums，其長度為 n，且元素規則為：nums[i] = start + 2 * i（0 <= i < n）。
 * 請計算並回傳 nums 中所有元素做位元互斥或（Bitwise XOR）運算的結果。
 * 
 * 範例 1：
 * 輸入：n = 5, start = 0
 * 輸出：8
 * 解釋：陣列 nums = [0, 2, 4, 6, 8]，0 ^ 2 ^ 4 ^ 6 ^ 8 = 8。
 * 
 * 範例 2：
 * 輸入：n = 4, start = 3
 * 輸出：8
 * 解釋：陣列 nums = [3, 5, 7, 9]，3 ^ 5 ^ 7 ^ 9 = 8。
 * 
 * 限制條件：
 * 1 <= n <= 1000
 * 0 <= start <= 1000
 * n == nums.length
 */

/*
 * =========================================================================
 * 【LeetCode 官方提交成績】：
 * - 狀態：Accepted (通過 54/54 測試案例)
 * - 執行時間：0 ms (擊敗 100.00% C 語言提交)
 * - 記憶體：8.4 MB
 * - Submission ID：2179704794
 * -------------------------------------------------------------------------
 * 【使用者原始白板程式碼】：
 * int xorOperation(int n, int start) {
 *     int curr = start;
 *     int xor = 0;
 *     for(int i = 0; i < n; i++)
 *     {
 *         curr = start + 2 * i;
 *         xor ^= curr;
 *     }
 *     return xor;
 * }
 * -------------------------------------------------------------------------
 * 【演算法與效能剖析】：
 * 1. 邏輯完全精準：
 *    - 嚴格依照公式 nums[i] = start + 2 * i 產生元素，並利用複合賦值運算子 ^= 進行原地累積。
 * 2. 複雜度：
 *    - 時間複雜度：O(n)，單層線性迴圈，耗時 0 ms 擊敗 100%！
 *    - 空間複雜度：O(1)，僅使用兩個暫存變數 curr 與 xor，零記憶體額外負擔。
 * =========================================================================
 */

int xorOperation(int n, int start) {
    int curr = start;
    int xor = 0;
    for (int i = 0; i < n; i++) {
        curr = start + 2 * i;
        xor ^= curr;
    }
    return xor;
}

int main() {
    printf("Test 1: %d (Expected: 8)\n", xorOperation(5, 0));
    printf("Test 2: %d (Expected: 8)\n", xorOperation(4, 3));
    printf("Test 3: %d (Expected: 2)\n", xorOperation(1, 2));

    return 0;
}
