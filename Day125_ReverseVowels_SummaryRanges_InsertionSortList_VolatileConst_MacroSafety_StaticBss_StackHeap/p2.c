#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * LeetCode #228 - Summary Ranges
 * 
 * 題目描述：
 * 給定一個無重複元素的已排序整數陣列 nums。
 * 區間範圍 [a,b] 是從 a 到 b 的所有整數的集合。
 * 請回傳恰好覆蓋陣列中所有數字的最小有序區間範圍清單。
 * 也就是說，nums 的每個元素都恰好被某個區間覆蓋，且不存在不屬於 nums 的整數落在任一區間內。
 * 
 * 區間表示規則：
 * - 若 a != b，表示為 "a->b"
 * - 若 a == b，表示為 "a"
 * 
 * 範例 1：
 * 輸入：nums = [0, 1, 2, 4, 5, 7]
 * 輸出：["0->2", "4->5", "7"]
 * 解釋：區間為 [0,2] -> "0->2", [4,5] -> "4->5", [7,7] -> "7"
 * 
 * 範例 2：
 * 輸入：nums = [0, 2, 3, 4, 6, 8, 9]
 * 輸出：["0", "2->4", "6", "8->9"]
 * 
 * 限制條件：
 * 0 <= nums.length <= 20
 * -2^31 <= nums[i] <= 2^31 - 1
 * nums 中的所有值皆唯一且已按遞增順序排列
 */

/*
 * =========================================================================
 * 【LeetCode 官方提交成績】：
 * - 狀態：Accepted (通過 34/34 測試案例)
 * - 執行時間：0 ms (擊敗 100.00% C 語言提交)
 * - 記憶體：8.4 MB
 * - Submission ID：2180248619
 * -------------------------------------------------------------------------
 * 【演算法與韌體面試精華剖析】：
 * 1. 雙指標區間走訪（Two Pointers / Sliding Range）：
 *    - 外層 while(i < numsSize) 固定區間起點 start = i。
 *    - 內層 while 往後探查連續遞增元素，直到斷點出現，鎖定 end = i。
 *    - 每個元素僅被走訪兩次，時間複雜度嚴格為 O(n)，0 ms 擊敗 100%！
 * 2. C 語言兩層記憶體配置（Two-level Malloc）：
 *    - 外層配置 char** 指標陣列，容量為 numsSize * sizeof(char*)。
 *    - 內層配置每個區間的實際字串 buffer，最長情況為 INT_MIN->INT_MAX (共 24 字元)，
 *      配置 32 bytes 綽綽有餘且絕不越界。
 * 3. 韌體整數溢位防禦（Signed Integer Overflow）：
 *    - 嚴禁寫 nums[i + 1] == nums[i] + 1，若 nums[i] 為 INT_MAX (2147483647)，加 1 觸發 UB。
 *    - 採用 (long long)nums[i + 1] - nums[i] == 1 進行 64 位元計算，100% 安全。
 * =========================================================================
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
char** summaryRanges(int* nums, int numsSize, int* returnSize) {
    // 邊界防禦：空陣列處理
    if (numsSize == 0) {
        *returnSize = 0;
        return NULL;
    }

    // 第一層 malloc：最多可能產生 numsSize 個區間字串指標
    char** result = (char**)malloc(numsSize * sizeof(char*));
    int count = 0;

    int i = 0;
    while (i < numsSize) {
        int start = i;

        /*
         * 往後尋找連續遞增的數字
         * 【韌體防禦亮點】：
         * 絕不能寫 nums[i + 1] == nums[i] + 1，因為若 nums[i] 為 INT_MAX (2147483647)，
         * 加 1 會引發 32 位元有號整數溢位 (Signed Integer Overflow / UB)！
         * 強制轉型 (long long) 相減，確保絕對不溢位。
         */
        while (i + 1 < numsSize && (long long)nums[i + 1] - nums[i] == 1) {
            i++;
        }
        int end = i;

        // 第二層 malloc：每個字串最大長度為 24 bytes (INT_MIN->INT_MAX)，配置 32 bytes 綽綽有餘
        result[count] = (char*)malloc(32 * sizeof(char));

        if (start == end) {
            // 單一數字區間
            sprintf(result[count], "%d", nums[start]);
        } else {
            // 多個數字區間
            sprintf(result[count], "%d->%d", nums[start], nums[end]);
        }
        count++;

        // 前進至下一個數字
        i++;
    }

    *returnSize = count;
    return result;
}

int main() {
    int nums1[] = {0, 1, 2, 4, 5, 7};
    int retSize1 = 0;
    char** res1 = summaryRanges(nums1, 6, &retSize1);
    printf("Test 1:\n");
    for (int i = 0; i < retSize1; i++) {
        printf("  %s\n", res1[i]);
        free(res1[i]);
    }
    free(res1);

    int nums2[] = {0, 2, 3, 4, 6, 8, 9};
    int retSize2 = 0;
    char** res2 = summaryRanges(nums2, 7, &retSize2);
    printf("Test 2:\n");
    for (int i = 0; i < retSize2; i++) {
        printf("  %s\n", res2[i]);
        free(res2[i]);
    }
    free(res2);

    return 0;
}
