#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #1470: Shuffle the Array
 * (群聯 HackMD 考古題原題：陣列重排、DMA 雙通道交錯緩衝區映射)
 * 
 * 題目描述：
 * 給定一個包含 2n 個元素的整數陣列 nums，其格式為 [x1, x2, ..., xn, y1, y2, ..., yn]。
 * 請按照 [x1, y1, x2, y2, ..., xn, yn] 的格式重新排列陣列，並返回結果。
 * 
 * 限制條件：
 * - 1 <= n <= 500
 * - nums.length == 2n
 * - 1 <= nums[i] <= 10^3
 * 
 * 進階思考（韌體白板極限加分項）：
 * - 注意到 1 <= nums[i] <= 1000 < 1024 (即每個數字在二進位中最多只佔 10 bits)。
 * - 而 C 語言的 int 至少有 32 bits！你能否利用位元運算，將 xi 與 yi 暫時打包壓縮進同一個 int 中，
 *   進而達成「不需要額外 malloc 全新陣列的原地 (In-place) O(1) 空間重組」？
 * 
 * 範例 1:
 *   輸入: nums = [2,5,1,3,4,7], n = 3
 *   輸出: [2,3,5,4,1,7]
 *   解釋: x1=2, x2=5, x3=1, y1=3, y2=4, y3=7，重排後為 [2,3,5,4,1,7]。
 * 
 * 範例 2:
 *   輸入: nums = [1,2,3,4,4,3,2,1], n = 4
 *   輸出: [1,4,2,3,3,2,4,1]
 * 
 * 範例 3:
 *   輸入: nums = [1,1,2,2], n = 2
 *   輸出: [1,2,1,2]
 */

int* shuffle(int* nums, int numsSize, int n, int* returnSize) {
    // 【極致滿分：原地 (In-place) 位元壓縮與逆向解包 (O(1) 額外空間)】
    // 階段 1：將 (xi, yi) 同時壓縮進前半段 nums[i] 中
    // 低 16 位保留 xi (0~1000)，高 16 位存放 yi (0~1000)
    for (int i = 0; i < n; i++) {
        nums[i] = nums[i] | (nums[i + n] << 16);
    }

    // 階段 2：逆向 (從後往前) 解包至最終位置，避免尚未讀取的 nums[i] 被覆蓋！
    for (int i = n - 1; i >= 0; i--) {
        int y = nums[i] >> 16;
        int x = nums[i] & 0xFFFF;
        nums[2 * i + 1] = y;
        nums[2 * i] = x;
    }

    *returnSize = numsSize;
    return nums;
}

/*
 * =====================================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * =====================================================================================
 * int* shuffle(int* nums, int numsSize, int n, int* returnSize) {
 *     int slow = 0;
 *     int fast = n;
 *     int curr = 0;
 *     while(curr < numsSize)
 *     {
 *         nums[curr] = (nums[slow] << 16) | nums[fast];
 *         curr += 2;
 *         slow++;
 *         fast++;
 *     }
 *     int p = 0;
 *     while(p < numsSize)
 *     {
 *         nums[p+1] = nums[p] & 0x0000ffff;
 *         nums[p] = nums[p] >> 16;
 *         p += 2;
 *     }
 *     *returnSize = numsSize;
 *     return nums;
 * }
 * =====================================================================================
 * 【面試亮點剖析 & 記憶體覆蓋踩踏盲點】 (LeetCode #1470: Shuffle the Array)
 * =====================================================================================
 * 
 * 1. 驚豔的面試直覺：位元打包壓縮 (Bitwise Packing)：
 *    - 你敏銳地察覺到 1 <= nums[i] <= 1000，二進位最多只佔 10 bits，
 *      而 C 語言的 int 有 32 bits，可以用 `<< 16` 將兩個數字塞進同一個 int！
 *    - 這個思路在群聯與 IC 設計廠面試中，是直接從「一般工程師」躍升為「架構級頂尖工程師」的王牌加分點！
 * 
 * 2. 原始程式碼的致命踩踏盲點 (Forward Overwrite / Memory Stomp)：
 *    - 原始寫法在第 1 個迴圈中：
 *      `nums[curr] = (nums[slow] << 16) | nums[fast];`
 *      * 當 curr = 0 時，寫入 nums[0]，此時 slow = 0, fast = n。
 *      * 當 curr = 2 時，寫入 nums[2]，此時 slow = 1, fast = n + 1。
 *      * 當 curr = 4 時，寫入 nums[4]，此時 slow = 2！
 *    - 踩雷瞬間：在上一輪 (curr = 2) 時，nums[2] 已經被覆蓋成打包後的數值了！
 *      當 slow 走到 2 時，讀取的 `nums[slow]` 已經不再是原始的 x3，造成原本的資料永久丟失！
 * 
 * 3. 完美修正關鍵：【原地壓縮 ＋ 逆向解包 (Reverse Unpacking)】：
 *    - 步驟 1：只把 (xi, yi) 壓縮進前半段 `nums[i] = nums[i] | (nums[i + n] << 16)`。
 *      此時後半段 `nums[i + n]` 完全沒被修改，前半段的原數值也完整保留在低 16 位，零資料遺失！
 *    - 步驟 2：從後往前（i = n - 1 倒數到 0）逆向解包填入 `nums[2*i]` 與 `nums[2*i+1]`。
 *      因為 2*i 永遠大於等於 i，逆向寫入的目標索引永遠在當前讀取索引的右方，
 *      絕對不可能覆蓋到左方尚未讀取的 nums[i]，達成 100% 安全的原地 O(1) 空間操作！
 * =====================================================================================
 */

int main(void) {
    printf("=== Test P2: Shuffle the Array ===\n");

    // Test 1: [2,5,1,3,4,7], n = 3 -> [2,3,5,4,1,7]
    int t1[] = {2, 5, 1, 3, 4, 7};
    int exp1[] = {2, 3, 5, 4, 1, 7};
    int s1 = 0;
    int* ans1 = shuffle(t1, 6, 3, &s1);
    assert(s1 == 6);
    for (int i = 0; i < 6; i++) {
        assert(ans1[i] == exp1[i]);
    }
    printf("Test 1 Passed: [2,5,1,3,4,7] -> [2,3,5,4,1,7]\n");

    // Test 2: [1,2,3,4,4,3,2,1], n = 4 -> [1,4,2,3,3,2,4,1]
    int t2[] = {1, 2, 3, 4, 4, 3, 2, 1};
    int exp2[] = {1, 4, 2, 3, 3, 2, 4, 1};
    int s2 = 0;
    int* ans2 = shuffle(t2, 8, 4, &s2);
    assert(s2 == 8);
    for (int i = 0; i < 8; i++) {
        assert(ans2[i] == exp2[i]);
    }
    printf("Test 2 Passed: [1,2,3,4,4,3,2,1] -> [1,4,2,3,3,2,4,1]\n");

    // Test 3: [1,1,2,2], n = 2 -> [1,2,1,2]
    int t3[] = {1, 1, 2, 2};
    int exp3[] = {1, 2, 1, 2};
    int s3 = 0;
    int* ans3 = shuffle(t3, 4, 2, &s3);
    assert(s3 == 4);
    for (int i = 0; i < 4; i++) {
        assert(ans3[i] == exp3[i]);
    }
    printf("Test 3 Passed: [1,1,2,2] -> [1,2,1,2]\n");

    printf("All P2 Local Tests Passed!\n");
    return 0;
}
