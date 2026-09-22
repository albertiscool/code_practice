#include <stdio.h>

// ============================================================================
// 題目 3【移動零 (Move Zeroes - LeetCode #283)】
// 難度：🟢 系統廠高頻基本題 | 出題頻率：🔥🔥🔥🔥🔥 (廣達/緯創/台達電陣列雙指針常客)
// 題目說明：給定一個陣列 nums，編寫一個函式將所有 0 移動到陣列的末尾，
//           同時保持非零元素的相對順序。
//           請注意，你必須在不複製陣列的情況下「原地 (in-place)」對陣列進行修改。
// 範例 1：
//    輸入：nums = [0,1,0,3,12]
//    輸出：[1,3,12,0,0]
// 範例 2：
//    輸入：nums = [0]
//    輸出：[0]
// 限制條件：
//    - 1 <= nums.length <= 10^4
//    - -2^31 <= nums[i] <= 2^31 - 1
//    - 空間複雜度必須為嚴格 O(1)！
//    - 儘量減少操作次數。
// ============================================================================

void moveZeroes(int* nums, int numsSize) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    int slow = 0;
    int fast = 0;
    while(fast < numsSize)
    {
        if(nums[fast] != 0)
        {
            nums[slow++] = nums[fast];
        }
        fast++;
    }
    while(slow < numsSize)
    {
        nums[slow++] = 0;
    }
}

int main(void) {
    /* 測試測資 1: [0, 1, 0, 3, 12] -> [1, 3, 12, 0, 0] */
    int nums1[] = {0, 1, 0, 3, 12};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    moveZeroes(nums1, n1);
    printf("測資 1: [");
    for (int i = 0; i < n1; i++) printf("%d%s", nums1[i], (i == n1 - 1) ? "" : ", ");
    printf("] (預期: [1, 3, 12, 0, 0])\n");

    /* 測試測資 2: [0] -> [0] */
    int nums2[] = {0};
    moveZeroes(nums2, 1);
    printf("測資 2: [%d] (預期: [0])\n", nums2[0]);

    /* 測試測資 3: [2, 1] (無零) -> [2, 1] */
    int nums3[] = {2, 1};
    moveZeroes(nums3, 2);
    printf("測資 3: [%d, %d] (預期: [2, 1])\n", nums3[0], nums3[1]);

    /* 測試測資 4: [0, 0, 0] (全零) -> [0, 0, 0] */
    int nums4[] = {0, 0, 0};
    moveZeroes(nums4, 3);
    printf("測資 4: [%d, %d, %d] (預期: [0, 0, 0])\n", nums4[0], nums4[1], nums4[2]);

    return 0;
}
