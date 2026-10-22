#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #260 - Single Number III
 * 
 * 題目描述：
 * 給定一個整數陣列 nums，其中恰好有兩個元素只出現一次，其餘所有元素皆恰好出現兩次。
 * 請找出這兩個只出現一次的元素。你可以按任意順序回傳答案。
 * 
 * 你必須設計一個時間複雜度為 O(n) 且僅使用常數額外空間 O(1) 的演算法來解決此問題。
 * 
 * 範例 1：
 * 輸入：nums = [1, 2, 1, 3, 2, 5]
 * 輸出：[3, 5]
 * 解釋：[5, 3] 也是有效答案。
 * 
 * 範例 2：
 * 輸入：nums = [-1, 0]
 * 輸出：[-1, 0]
 * 
 * 範例 3：
 * 輸入：nums = [0, 1]
 * 輸出：[1, 0]
 * 
 * 限制條件：
 * 2 <= nums.length <= 3 * 10^4
 * -2^31 <= nums[i] <= 2^31 - 1
 * 除兩個只出現一次的整數外，陣列中其餘整數皆恰好出現兩次。
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* singleNumber(int* nums, int numsSize, int* returnSize) {
    // 請於此處實作純白板解法
    int xor1 = 0;
    for(int i = 0; i < numsSize; i++)
    {
        xor1 ^= nums[i];
    }
    int diff = xor1 & (-xor1);
}

int main() {
    int nums1[] = {1, 2, 1, 3, 2, 5};
    int retSize1 = 0;
    int* res1 = singleNumber(nums1, 6, &retSize1);
    printf("Test 1: [%d, %d] (Expected [3, 5] or [5, 3])\n", res1 ? res1[0] : -1, res1 ? res1[1] : -1);
    free(res1);

    int nums2[] = {-1, 0};
    int retSize2 = 0;
    int* res2 = singleNumber(nums2, 2, &retSize2);
    printf("Test 2: [%d, %d] (Expected [-1, 0] or [0, -1])\n", res2 ? res2[0] : -1, res2 ? res2[1] : -1);
    free(res2);

    int nums3[] = {0, 1};
    int retSize3 = 0;
    int* res3 = singleNumber(nums3, 2, &retSize3);
    printf("Test 3: [%d, %d] (Expected [0, 1] or [1, 0])\n", res3 ? res3[0] : -1, res3 ? res3[1] : -1);
    free(res3);

    return 0;
}
