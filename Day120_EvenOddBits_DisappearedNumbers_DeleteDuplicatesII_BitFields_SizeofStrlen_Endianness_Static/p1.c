#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #2595 - Number of Even and Odd Bits
 * 
 * 題目描述：
 * 給定一個正整數 n。
 * 考慮 n 的二進位表示（從右向左，以 0 為起點索引）：
 * - 偶數位元 (Even index)：第 0, 2, 4, 6, ... 個 bit。
 * - 奇數位元 (Odd index)：第 1, 3, 5, 7, ... 個 bit。
 * 
 * 請回傳一個大小為 2 的整數陣列 [even, odd]，其中：
 * - even 代表 n 在所有「偶數索引」上值為 1 的 bit 總數。
 * - odd 代表 n 在所有「奇數索引」上值為 1 的 bit 總數。
 * 
 * 限制條件：
 * 1 <= n <= 1000
 */

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本 - 一次 AC / 0 ms)】
 * -------------------------------------------------------------------------
 * int* evenOddBit(int n, int* returnSize) {
 *     unsigned int even =(unsigned int) n & 0x55555555;
 *     unsigned int odd =(unsigned int) n & 0xAAAAAAAA;
 *     int count_even = 0;
 *     int count_odd = 0;
 *     while(even > 0)
 *     {
 *         even = even & (even - 1);
 *         count_even++;
 *     }
 *     while(odd > 0)
 *     {
 *         odd = odd & (odd - 1);
 *         count_odd++;
 *     }
 * 
 *     *returnSize = 2;
 *     int* res = (int*)malloc(2 * sizeof(int));
 *     res[0] = count_even;
 *     res[1] = count_odd;
 *     return res;
 * }
 * -------------------------------------------------------------------------
 * 【設計亮點剖析】：
 * 1. 工業級暫存器遮罩直覺：
 *    - 偶數位元遮罩 0x55555555 (0101_0101...) 與 奇數位元遮罩 0xAAAAAAAA (1010_1010...)，
 *      單週期一步完成所有奇偶位元的平行提取！
 * 2. 結合 Brian Kernighan 高效消元：
 *    - 利用 `even & (even - 1)` 與 `odd & (odd - 1)` 快速消除最右側的 1，
 *      達到 0 冗餘分支、極致常數時間計數。
 * 3. 遠端實測：1003/1003 全測資 Accepted，執行時間 0 ms (Beats 100%)！
 * =========================================================================
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* evenOddBit(int n, int* returnSize) {
    unsigned int even = (unsigned int)n & 0x55555555;
    unsigned int odd = (unsigned int)n & 0xAAAAAAAA;
    int count_even = 0;
    int count_odd = 0;

    while (even > 0) {
        even = even & (even - 1);
        count_even++;
    }
    while (odd > 0) {
        odd = odd & (odd - 1);
        count_odd++;
    }

    *returnSize = 2;
    int* res = (int*)malloc(2 * sizeof(int));
    res[0] = count_even;
    res[1] = count_odd;
    return res;
}

int main() {
    int size = 0;
    int* r1 = evenOddBit(17, &size);
    printf("Test 1: n = 17 -> [%d, %d] (Expected: [2, 0])\n", r1[0], r1[1]);
    free(r1);

    int* r2 = evenOddBit(2, &size);
    printf("Test 2: n = 2 -> [%d, %d] (Expected: [0, 1])\n", r2[0], r2[1]);
    free(r2);

    return 0;
}
