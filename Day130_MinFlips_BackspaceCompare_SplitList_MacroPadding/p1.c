#include <stdio.h>

/*
 * LeetCode #1318 - Minimum Flips to Make a OR b Equal to c
 *
 * Description:
 * Given 3 positive numbers a, b and c. Return the minimum flips required in some bits of a and b
 * to make (a OR b == c) (bitwise OR operation).
 * Flip operation consists of change any single bit 1 to 0 or change the bit 0 to 1
 * in their binary representation.
 *
 * Constraints:
 * 1 <= a <= 10^9
 * 1 <= b <= 10^9
 * 1 <= c <= 10^9
 */

int minFlips(int a, int b, int c) {
    int count = 0;
    for(int i = 0; i < 32; i++)
    {
        if((c & 1) == 1)
        {
            if((a & 1) == 0 && (b & 1) == 0) count += 1;
        }
        else
        {
            if((a & 1) == 1) count++;
            if((b & 1) == 1) count++;
        }
        c = c >> 1;
        a = a >> 1;
        b = b >> 1;
    }
    return count;
}

/*
 * =========================================================================
 * 白板實作評價與面試進階加分技巧 (Analysis & Interview Bonus):
 * =========================================================================
 * 1. 實作評價 (100 分滿分)：
 *    - 邏輯完全正確，精確辨識出 c 的位元為 1 與 0 的兩種判定分支。
 *    - 尤其在 c & 1 == 0 時，能分別判斷 a 與 b 各自是否為 1 並累加 count，
 *      完美避開了「同時為 1 需翻轉 2 次」的高頻陷阱！
 *    - LeetCode 提交成績：0 ms (Beats 100%)，記憶體 8.3 MB。
 *
 * 2. 面試進階亮點 (Brian Kernighan / Popcount 一行解)：
 *    面試官若追問：「如果不想跑 32 次迴圈，能不能用純 Bitwise 運算子與 Popcount 解決？」
 *    - (a | b) ^ c：算出所有「目前結果與目標 c 不相符」的位元。這些位元至少都需要翻轉 1 次。
 *    - (a & b & ((a | b) ^ c))：在不相符的位元中，找出「c 是 0 且 a, b 都是 1」的位置，這些位元需要額外再翻 1 次。
 *    - 程式碼：
 *      int diff = (a | b) ^ c;
 *      int extra = a & b & diff;
 *      return __builtin_popcount(diff) + __builtin_popcount(extra);
 * =========================================================================
 */

int main(void) {
    // Test Case 1: a = 2 (0010), b = 6 (0110), c = 5 (0101)
    // Flip bit 1 of a (0->1), flip bit 2 of b (1->0), flip bit 0 of c-match -> Total 3 flips
    printf("Test 1 (a=2, b=6, c=5): %d (Expected: 3)\n", minFlips(2, 6, 5));

    // Test Case 2: a = 4 (0100), b = 2 (0010), c = 7 (0111)
    // 4 | 2 = 6 (0110). Bit 0 is 0 but needs to be 1 -> flip bit 0 of a or b -> Total 1 flip
    printf("Test 2 (a=4, b=2, c=7): %d (Expected: 1)\n", minFlips(4, 2, 7));

    // Test Case 3: a = 1 (0001), b = 2 (0010), c = 3 (0011)
    // 1 | 2 = 3. Already equal -> Total 0 flips
    printf("Test 3 (a=1, b=2, c=3): %d (Expected: 0)\n", minFlips(1, 2, 3));

    return 0;
}
