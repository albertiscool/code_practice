#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #2220 - Minimum Bit Flips to Convert Number
 * 
 * 考點對照 (HackMD 群聯面試題)：
 * 1. 位元 XOR 差異比對與翻轉計數 (Bit Flips)。
 * 2. 暫存器遮罩快速提取與消除最低位 1：n & (n - 1) 或 __builtin_popcount。
 * 
 * 題目描述：
 * 一次「位元翻轉 (Bit Flip)」是指將數字二進位表示中的某一個 bit 由 0 變 1，或是由 1 變 0。
 * 給定兩個整數 start 與 goal，請回傳將 start 轉換成 goal 所需的「最少位元翻轉次數」。
 * 
 * 限制條件：
 * 0 <= start, goal <= 10^9
 */

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * -------------------------------------------------------------------------
 * int minBitFlips(int start, int goal) {
 *     // 請於此處實作純白板解法
 *     int a = ~(start & goal);
 *     int count = 0;
 *     while(a != 0)
 *     {
 *         a = a & (a - 1);
 *         count++;
 *     }
 *     return count;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 邏輯運算子誤用：
 *    - 欲找出兩個數字「哪些位元不同」，必須使用 XOR 互斥或運算 (`start ^ goal`)。
 *      因為 XOR 的特性是「相異為 1，相同為 0」：
 *      0 ^ 1 = 1, 1 ^ 0 = 1 (需要翻轉)
 *      0 ^ 0 = 0, 1 ^ 1 = 0 (位元相同，不需翻轉)
 *    - 原寫法使用 NAND (`~(start & goal)`)：
 *      a. 原本高位皆為 0 的高位元（例如 bit 31~30）經過 ~ 取反全變成 1，最高符號位變負數。
 *      b. 當 a 包含負數甚至是 INT_MIN 時，執行 `a - 1` 觸發 signed integer overflow Runtime Error！
 *      c. 兩者皆為 0 的位元也被錯誤計入翻轉次數。
 * 2. 型別安全：
 *    - 進行位元運算與遮罩操作時，使用 unsigned int 儲存差異遮罩，可防止符號擴展與溢位陷阱。
 * =========================================================================
 */

int minBitFlips(int start, int goal) {
    // 1. 利用 XOR 找出所有相異位元（需要翻轉的位元設為 1）
    unsigned int a = (unsigned int)(start ^ goal);
    int count = 0;
    
    // 2. 經典 Brian Kernighan 演算法：每次 a & (a - 1) 消除最右側的 1
    while (a != 0) {
        a = a & (a - 1);
        count++;
    }
    
    return count;
    
    // 架構級單週期硬體優化 (ARM POPCNT):
    // return __builtin_popcount((unsigned int)(start ^ goal));
}

int main() {
    printf("Test 1: start = 10, goal = 7 -> %d (Expected: 3)\n", minBitFlips(10, 7));
    printf("Test 2: start = 3, goal = 4 -> %d (Expected: 3)\n", minBitFlips(3, 4));
    return 0;
}
