#include <stdio.h>

// ============================================================================
// 題目 3【移除元素 (Remove Element - LeetCode #27)】
// 難度：🟢 系統廠高頻基本題 | 出題頻率：🔥🔥🔥🔥🔥 (系統廠/科技廠陣列快慢指標必考題)
// 題目說明：給你一個陣列 nums 和一個數值 val，你需要「原地 (in-place)」移除所有數值等於 val 的元素，
//           並返回移除後陣列的新長度 k。
//           元素的順序可以改變。你不需要考慮陣列中超出新長度後面的元素。
// 範例 1：
//    輸入：nums = [3,2,2,3], val = 3
//    輸出：k = 2, nums = [2,2,_,_]
// 範例 2：
//    輸入：nums = [0,1,2,2,3,0,4,2], val = 2
//    輸出：k = 5, nums = [0,1,3,0,4,_,_,_]
// 限制條件：
//    - 0 <= nums.length <= 100
//    - 0 <= nums[i] <= 50
//    - 0 <= val <= 100
//    - 空間複雜度必須為 O(1)！
// ============================================================================

int removeElement(int* nums, int numsSize, int val) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    int slow = 0;
    int fast = 0;
    while(fast < numsSize)
    {
        if(nums[fast] != val)
        {
            nums[slow++] = nums[fast];
        }
        fast++;
    }
    return slow;
}

int main(void) {
    /* 測試測資 1: [3,2,2,3], val = 3 -> 長度 2, 前兩位為 2, 2 */
    int nums1[] = {3, 2, 2, 3};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int k1 = removeElement(nums1, n1, 3);
    printf("測資 1 結果: 長度 = %d, 陣列 = [", k1);
    for (int i = 0; i < k1; i++) printf("%d%s", nums1[i], (i == k1 - 1) ? "" : ", ");
    printf("] (預期長度: 2, 內容為 2, 2)\n");

    /* 測試測資 2: [0,1,2,2,3,0,4,2], val = 2 -> 長度 5, 內容不含 2 */
    int nums2[] = {0, 1, 2, 2, 3, 0, 4, 2};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    int k2 = removeElement(nums2, n2, 2);
    printf("測資 2 結果: 長度 = %d, 陣列 = [", k2);
    for (int i = 0; i < k2; i++) printf("%d%s", nums2[i], (i == k2 - 1) ? "" : ", ");
    printf("] (預期長度: 5)\n");

    /* 測試測資 3: 空陣列 [], val = 1 -> 長度 0 */
    int nums3[1] = {0};
    int k3 = removeElement(nums3, 0, 1);
    printf("測資 3 結果: 長度 = %d (預期長度: 0)\n", k3);

    return 0;
}
