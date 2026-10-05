#include <stdio.h>
#include <stdlib.h>

/**
 * ============================================================================
 * 【Day 108 - 題目二】有序陣列的平方 (LeetCode #977 - Squares of a Sorted Array)
 * 【考試頻率】電子五哥 (廣達 Quanta、緯創 Wistron、和碩 Pegatron) 白板演算法高頻題
 * 【LeetCode 難度】Easy (雙指標向內收斂)
 * ============================================================================
 * 【題目說明】
 * 給定一個按「非遞減順序（遞增）」排序的整數陣列 nums。
 * 請回傳一個新陣列，該陣列包含每個數字的平方，並且同樣按「非遞減順序」排序。
 * 
 * ⚠️ 考官時間複雜度要求：請在 O(N) 時間複雜度內完成，不得使用 O(N log N) 的暴力平方後排序。
 * 
 * 【範例 1】
 * 輸入: nums = [-4, -1, 0, 3, 10]
 * 輸出: [0, 1, 9, 16, 100]
 * 解釋: 平方後為 [16, 1, 0, 9, 100]，排序後為 [0, 1, 9, 16, 100]。
 * 
 * 【範例 2】
 * 輸入: nums = [-7, -3, 2, 3, 11]
 * 輸出: [4, 9, 9, 49, 121]
 * 
 * 【限制條件】
 * 1. 1 <= nums.length <= 10^4
 * 2. -10^4 <= nums[i] <= 10^4
 * 3. nums 已按非遞減順序排序。
 * ============================================================================
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* sortedSquares(int* nums, int numsSize, int* returnSize) {
    *returnSize = numsSize;
    int left = 0;
    int right = numsSize - 1;
    int* result = (int*)malloc(numsSize * sizeof(int));
    int curr = numsSize - 1;

    // 💡 關鍵邊界修正：必須是 left <= right（包含等號！）
    // 當 left == right 時，正好剩最後一個中間元素（絕對值最小者），必須填入 result[0]
    while (left <= right) {
        if (nums[left] * nums[left] > nums[right] * nums[right]) {
            result[curr] = nums[left] * nums[left];
            left++;
        } else {
            result[curr] = nums[right] * nums[right];
            right--;
        }
        curr--;
    }
    return result;
}

/*
===============================================================================
【原始程式碼盲點與改進分析】
-------------------------------------------------------------------------------
1. ⚠️ 雙指標終止條件致命盲點（漏填最後一個元素）：
   - 原寫法：`while(left < right)`
   - 實測輸出：
     輸入：`[-4, -1, 0, 3, 10]`
     預期：`[0, 1, 9, 16, 100]`
     原程式輸出：`[-1094795586, 1, 9, 16, 100]`
   - 盲點剖析：
     當雙指標往中間收斂，走到 `left == right`（即遇到最後一個元素 `0`）時：
     `left < right` 判定為 false，迴圈直接提前結束！
     導致 `result[0]`（`curr = 0`）完全沒被寫入，留下了未初始化的記憶體垃圾值 `-1094795586`！
     甚至當 `numsSize == 1` 時，迴圈一次都進不去。
   - 訂正方法：
     將條件改為 `while(left <= right)`，保證最後一個元素也能被順利計算並填入 `result[0]`。

-------------------------------------------------------------------------------
【實測成績】
- 官方評判：Accepted (通過全部測資) 🎉
- 執行時間：0 ms (Beats 100.00% C submissions) ⚡
- 記憶體消耗：25.4 MB (Beats 100.00% C submissions) 🚀
- 提交 ID：2162691073

-------------------------------------------------------------------------------
【原始程式碼存檔】
int* sortedSquares(int* nums, int numsSize, int* returnSize) {
    // 請在此處撰寫你的程式碼
    
    *returnSize = numsSize;
    int left = 0;
    int right = numsSize - 1;
    int* result = (int*) malloc(numsSize * sizeof(int));
    int curr = numsSize - 1;
    while(left < right)
    {
        if(nums[left] * nums[left] > nums[right] * nums[right])
        {
            result[curr] = nums[left] * nums[left];
            left++;
        }
        else
        {
            result[curr] = nums[right] * nums[right];
            right--;
        }
        curr--;
    }
    return result;
}
===============================================================================
*/
