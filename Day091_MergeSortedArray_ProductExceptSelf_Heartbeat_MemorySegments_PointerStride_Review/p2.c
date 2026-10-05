#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 2【除自身以外陣列的乘積 (Product of Array Except Self - LeetCode #238)】
// 難度：🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (一線外商/聯發科/瑞昱經典高頻白板題)
// 題目說明：給你一個整數陣列 nums，返回一個陣列 answer，
//           其中 answer[i] 等於 nums 中除了 nums[i] 之外其餘各元素的乘積。
//           題目保證 nums 任何前綴或後綴的乘積都在 32 位元整數範圍內。
// 限制條件：
//    - 2 <= nums.length <= 10^5
//    - -30 <= nums[i] <= 30
//    - 請設計一個時間複雜度為 O(N) 的演算法。
//    - ⚠️ 禁止使用「除法運算子 (/)」！
//    - 進階挑戰：你能否設計出額外空間複雜度為 O(1) 的演算法？（輸出的 answer 陣列不計入額外空間）
// 範例 1：
//    輸入：nums = [1,2,3,4]
//    輸出：[24,12,8,6]
// 範例 2：
//    輸入：nums = [-1,1,0,-3,3]
//    輸出：[0,0,9,0,0]
// ============================================================================

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* productExceptSelf(int* nums, int numsSize, int* returnSize) {
    /*
     * 正確解答 (前後綴乘積 / 雙向遍歷 - 時間 O(N)、額外空間 O(1)):
     * 1. 輸出陣列 ans[i] = (i 左邊所有元素的乘積) * (i 右邊所有元素的乘積)。
     * 2. 第一趟由左往右：left 初始為 1，ans[i] = left，隨後 left *= nums[i]。
     * 3. 第二趟由右往左：right 初始為 1 (乘法單位元素)，ans[j] *= right，隨後 right *= nums[j]。
     */
    if (nums == NULL || numsSize == 0) {
        *returnSize = 0;
        return NULL;
    }

    int *ans = (int*) malloc(numsSize * sizeof(int));
    if (!ans) {
        *returnSize = 0;
        return NULL;
    }

    // 第一趟：計算左側乘積
    int left = 1;
    for (int i = 0; i < numsSize; i++) {
        ans[i] = left;
        left *= nums[i];
    }

    // 第二趟：乘上右側乘積
    int right = 1;
    for (int j = numsSize - 1; j >= 0; j--) {
        ans[j] *= right;
        right *= nums[j];
    }

    *returnSize = numsSize;
    return ans;
}

/* ============================================================================
 * ❌ 原始實作盲點分析 (已保留供複習檢討):
 * ============================================================================
 * int* productExceptSelf_wrong(int* nums, int numsSize, int* returnSize) {
 *     if(nums == NULL)
 *     {
 *         return 0;
 *     }
 *     // ⚠️ 盲點 1 (語法錯誤): 分號放進括號內 sizeof(int);) 導致編譯直接失敗
 *     int *ans = (int*) malloc(numsSize * sizeof(int);)
 * 
 *     int left = 1;
 *     // ⚠️ 盲點 2: 乘法的單位元素是 1，若設 right = 0，後面相乘會全部歸零！
 *     int right = 0;
 * 
 *     for(int i = 0; i < numsSize; i++)
 *     {
 *         ans[i] = left;
 *         // ⚠️ 盲點 3: 應該累積原始陣列元素 nums[i]，若寫 left *= ans[i]，
 *         //           因為 ans[i] 剛剛才被設為 left，變成 left *= left！
 *         left *= ans[i];
 *     }
 * 
 *     // ⚠️ 盲點 4: 往回走應該是 j--，寫成 j++ 會造成無窮迴圈 (Infinite Loop)
 *     for(int j = numsSize - 1; j >= 0; j++)
 *     {
 *         ans[j] *= right;
 *         // ⚠️ 盲點 5: 同盲點 3，右側累積應乘上原始陣列元素 nums[j]，而非 ans[j]
 *         right *= ans[j];
 *     }
 *     *returnSize = numsSize;
 *     return ans;
 * }
 * ============================================================================ */

int main(void) {
    /* 測試測資 1: [1,2,3,4] -> [24,12,8,6] */
    int nums1[] = {1, 2, 3, 4};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int returnSize1 = 0;
    int* ans1 = productExceptSelf(nums1, n1, &returnSize1);
    printf("測資 1 結果: ");
    if (ans1) {
        for (int i = 0; i < returnSize1; i++) {
            printf("%d ", ans1[i]);
        }
        free(ans1);
    }
    printf("(預期: 24 12 8 6)\n");

    /* 測試測資 2: [-1,1,0,-3,3] -> [0,0,9,0,0] */
    int nums2[] = {-1, 1, 0, -3, 3};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    int returnSize2 = 0;
    int* ans2 = productExceptSelf(nums2, n2, &returnSize2);
    printf("測資 2 結果: ");
    if (ans2) {
        for (int i = 0; i < returnSize2; i++) {
            printf("%d ", ans2[i]);
        }
        free(ans2);
    }
    printf("(預期: 0 0 9 0 0)\n");

    return 0;
}
