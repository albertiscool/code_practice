#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// ============================================================================
// 題目 2【兩數之和 (Two Sum - LeetCode #1)】
// 演算法：雙重遍歷搜尋法 ($O(N^2)$ 時間, $O(1)$ 空間)
// ============================================================================
int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
    // 雙重迴圈枚舉所有可能的 (i, j) 組合
    for (int i = 0; i < numsSize - 1; i++)
    {
        for (int j = i + 1; j < numsSize; j++)
        {
            // 找到和為 target 的兩個數字
            if (nums[i] + nums[j] == target)
            {
                // 動態配置大小為 2 的整數陣列 (2 * sizeof(int))
                int* result = (int*)malloc(2 * sizeof(int));
                result[0] = i;
                result[1] = j;
                *returnSize = 2; // 設定回傳陣列長度為 2
                return result;
            }
        }
    }

    *returnSize = 0;
    return NULL;
}

int main()
{
    int returnSize;

    // 測試 1: nums = [2, 7, 11, 15], target = 9 -> 預期下標: [0, 1]
    int nums1[4] = {2, 7, 11, 15};
    int* res1 = twoSum(nums1, 4, 9, &returnSize);
    assert(returnSize == 2);
    printf("測試 1 下標: [%d, %d] (預期: [0, 1])\n", res1[0], res1[1]);
    assert((res1[0] == 0 && res1[1] == 1) || (res1[0] == 1 && res1[1] == 0));
    free(res1);

    // 測試 2: nums = [3, 2, 4], target = 6 -> 預期下標: [1, 2]
    int nums2[3] = {3, 2, 4};
    int* res2 = twoSum(nums2, 3, 6, &returnSize);
    assert(returnSize == 2);
    printf("測試 2 下標: [%d, %d] (預期: [1, 2])\n", res2[0], res2[1]);
    assert((res2[0] == 1 && res2[1] == 2) || (res2[0] == 2 && res2[1] == 1));
    free(res2);

    // 測試 3: nums = [3, 3], target = 6 -> 預期下標: [0, 1]
    int nums3[2] = {3, 3};
    int* res3 = twoSum(nums3, 2, 6, &returnSize);
    assert(returnSize == 2);
    printf("測試 3 下標: [%d, %d] (預期: [0, 1])\n", res3[0], res3[1]);
    assert(res3[0] == 0 && res3[1] == 1);
    free(res3);

    printf("\n🎉 p2.c 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討：
int* twoSum_original(int* nums, int numsSize, int target, int* returnSize)
{
    int i = 0;
    int j = 1;
    
    // 💡 盲點 1：本題 nums 陣列是「未排序 (Unsorted)」的！
    // 雙指標跳躍法（如果小於 target 就推進）只適用在「已由小到大排序好」的陣列。
    // 在未排序陣列中（例如 [3, 2, 4], target=6），比大小跳躍會錯過正解 [1, 2]！
    // 在純 C 語言面試中，最標準做法就是使用雙重 for 迴圈 (i從0到N-2, j從i+1到N-1)！
    
    // 💡 盲點 2（malloc 大小）：
    // 原版寫了 malloc(sizeof(*returnSize))，這只配置了 1 個 int (4 Bytes) 的大小！
    // 正確需配置 2 個 int：malloc(2 * sizeof(int))。
    return NULL;
}
*/
