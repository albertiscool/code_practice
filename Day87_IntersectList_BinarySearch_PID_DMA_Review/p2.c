#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 2【二分搜尋法 (Binary Search - LeetCode #704)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (一線科技廠/IC設計廠白板手寫最高頻演算法)
// 題目說明：給定一個含有 n 個元素的升序整數陣列 nums 和一個目標值 target，
//           請寫一個函式搜尋 nums 中的 target。如果目標值存在返回下標，否則返回 -1。
//           你必須寫出時間複雜度為 O(log n) 的演算法。
// 範例 1：
//    輸入：nums = [-1,0,3,5,9,12], target = 9
//    輸出：4
//    解釋：9 出現在 nums 中並且下標為 4。
// 範例 2：
//    輸入：nums = [-1,0,3,5,9,12], target = 2
//    輸出：-1
//    解釋：2 不存在 nums 中因此返回 -1。
// 限制條件：
//    - 1 <= nums.length <= 10^4
//    - -10^4 < nums[i], target < 10^4
//    - nums 中的所有元素是不重複的。
//    - nums 已經按照升序排序。
// ============================================================================

int search(int* nums, int numsSize, int target) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    if(nums == NULL)
    {
        return -1;
    }
    int first = 0;
    int last = numsSize - 1;
    while(first <= last)
    {
        int mid = first + (last - first) / 2;
        if(nums[mid] == target)
        {
            return mid;
        }
        else if(nums[mid] < target)
        {
            first = mid + 1;
        }
        else
        {
            last = mid - 1;
        }
    }
    return -1;
}

int main(void) {
    /* 測試測資 1: 目標存在 */
    int nums1[] = {-1, 0, 3, 5, 9, 12};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int ans1 = search(nums1, n1, 9);
    printf("測資 1 結果: 下標 %d (預期: 4)\n", ans1);

    /* 測試測資 2: 目標不存在 */
    int nums2[] = {-1, 0, 3, 5, 9, 12};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    int ans2 = search(nums2, n2, 2);
    printf("測資 2 結果: 下標 %d (預期: -1)\n", ans2);

    /* 測試測資 3: 單一元素邊界 */
    int nums3[] = {5};
    int n3 = sizeof(nums3) / sizeof(nums3[0]);
    int ans3 = search(nums3, n3, 5);
    printf("測資 3 結果: 下標 %d (預期: 0)\n", ans3);

    return 0;
}
