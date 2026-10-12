#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #75: Sort Colors
 * 
 * 題目描述：
 * 給定一個包含 n 個元素的整數陣列 nums，裡面的元素分別代表三種顏色：
 * 0 代表紅色、1 代表白色、2 代表藍色。
 * 請在「原地 (in-place)」對它們進行排序，使得相同顏色的元素相鄰，並按照紅、白、藍 (0, 1, 2) 的順序排列。
 * 
 * 限制條件：
 * - 不能使用 C 語言標準函式庫的排序函式 (如 qsort)。
 * - n == numsSize
 * - 1 <= n <= 300
 * - nums[i] 的值僅會是 0, 1 或 2
 * - 進階挑戰：你能否設計出僅掃描一次 (One-pass) 且空間複雜度為 O(1) 的演算法？
 * 
 * 範例 1:
 *   輸入: nums = [2,0,2,1,1,0]
 *   輸出: [0,0,1,1,2,2]
 * 
 * 範例 2:
 *   輸入: nums = [2,0,1]
 *   輸出: [0,1,2]
 * 
 * 範例 3:
 *   輸入: nums = [0]
 *   輸出: [0]
 */
void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void sortColors(int* nums, int numsSize) {
    int left = 0;
    int right = numsSize - 1;
    int curr = 0;
    while(curr <= right)
    {
        if(nums[curr] == 0)
        {
            swap(&nums[curr], &nums[left]);
            left++;
            curr++;
        }
        else if(nums[curr] == 2)
        {
            swap(&nums[curr], &nums[right]);
            right--;
        }
        else
        {
            curr++;
        }
    }
}

/*
 * =====================================================================================
 * 【面試盲點剖析 & 邊界除錯精華】 (LeetCode #75: Sort Colors)
 * =====================================================================================
 * 原始盲點：
 *   迴圈終止條件誤寫為 `while (curr < right)`，導致當 `curr == right` 時提早跳出迴圈。
 * 
 * 致命案例剖析：
 *   以 nums = [2, 0, 1] 為例：
 *   1. 初始: left = 0, right = 2, curr = 0
 *   2. nums[curr] == 2: 與 nums[right] 交換 -> nums 變為 [1, 0, 2], right 減 1 變 1, curr 仍為 0
 *   3. nums[curr] == 1: curr 加 1 變 1
 *   4. 此時 curr = 1, right = 1：
 *      - 若為 `curr < right` (1 < 1 為 false)，迴圈直接終止！
 *        結果陣列停在 [1, 0, 2]（nums[1]=0 完全未被檢查），輸出錯誤！
 *      - 若修正為 `curr <= right` (1 <= 1 為 true)，則檢查 nums[1] == 0，
 *        與 nums[left=0] 交換 -> nums 變為 [0, 1, 2], left=1, curr=2，完美通過！
 * 
 * 核心不變量 (Invariant) 觀念：
 *   - [0, left - 1]     : 全為 0
 *   - [left, curr - 1]  : 全為 1
 *   - [curr, right]     : 尚未探索的未知區間 (Unexplored)
 *   - [right + 1, n - 1]: 全為 2
 *   當且僅當未知區間清空（即 curr > right）時，演算法才算完成。
 *   因此終止條件必須是 `while (curr <= right)`！
 * =====================================================================================
 */

int main(void) {
    printf("=== Test P1: Sort Colors ===\n");

    // Test 1: [2,0,2,1,1,0] -> [0,0,1,1,2,2]
    int nums1[] = {2, 0, 2, 1, 1, 0};
    int expected1[] = {0, 0, 1, 1, 2, 2};
    sortColors(nums1, 6);
    for (int i = 0; i < 6; i++) {
        assert(nums1[i] == expected1[i]);
    }
    printf("Test 1 Passed: [2,0,2,1,1,0] -> [0,0,1,1,2,2]\n");

    // Test 2: [2,0,1] -> [0,1,2]
    int nums2[] = {2, 0, 1};
    int expected2[] = {0, 1, 2};
    sortColors(nums2, 3);
    for (int i = 0; i < 3; i++) {
        assert(nums2[i] == expected2[i]);
    }
    printf("Test 2 Passed: [2,0,1] -> [0,1,2]\n");

    // Test 3: [0] -> [0]
    int nums3[] = {0};
    int expected3[] = {0};
    sortColors(nums3, 1);
    assert(nums3[0] == expected3[0]);
    printf("Test 3 Passed: [0] -> [0]\n");

    // Test 4: [2, 1, 0] -> [0, 1, 2]
    int nums4[] = {2, 1, 0};
    int expected4[] = {0, 1, 2};
    sortColors(nums4, 3);
    for (int i = 0; i < 3; i++) {
        assert(nums4[i] == expected4[i]);
    }
    printf("Test 4 Passed: [2, 1, 0] -> [0, 1, 2]\n");

    // Test 5: [1, 2, 0] -> [0, 1, 2]
    int nums5[] = {1, 2, 0};
    int expected5[] = {0, 1, 2};
    sortColors(nums5, 3);
    for (int i = 0; i < 3; i++) {
        assert(nums5[i] == expected5[i]);
    }
    printf("Test 5 Passed: [1, 2, 0] -> [0, 1, 2]\n");

    printf("All P1 Local Tests Passed!\n");
    return 0;
}
