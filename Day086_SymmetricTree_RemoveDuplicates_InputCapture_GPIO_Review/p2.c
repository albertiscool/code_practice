#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 2【刪除排序陣列中的重複項 (Remove Duplicates from Sorted Array - LeetCode #26)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠/IC設計廠雙指針原地修改經典題)
// 題目說明：給定一個升序排列的整數陣列 nums，請你「原地 (In-Place)」刪除重複出現的元素，
//           使每個元素只出現一次，返回刪除後陣列的新長度 k。
//           元素的相對順序應該保持一致。
//           你必須在不使用額外陣列空間的情況下完成，使用 O(1) 額外空間並在原地修改輸入陣列。
// 範例 1：
//    輸入：nums = [1,1,2]
//    輸出：k = 2, nums = [1,2,_]
//    解釋：函式應返回 k = 2，並且 nums 的前兩個元素被修改為 1 和 2。
// 範例 2：
//    輸入：nums = [0,0,1,1,1,2,2,3,3,4]
//    輸出：k = 5, nums = [0,1,2,3,4,_,_,_,_,_]
//    解釋：函式應返回 k = 5，並且 nums 的前五個元素被修改為 0, 1, 2, 3, 4。
// 限制條件：
//    - 1 <= nums.length <= 3 * 10^4
//    - -100 <= nums[i] <= 100
//    - nums 已按升序排列
// ============================================================================

int removeDuplicates(int* nums, int numsSize) {
    if (nums == NULL || numsSize == 0) {
        return 0;
    }
    
    // slow 指向目前已確認的不重複序列的最後一個元素
    int slow = 0;
    
    // fast 指針從 1 開始向後探索新元素
    for (int fast = 1; fast < numsSize; fast++) {
        if (nums[fast] != nums[slow]) {
            slow++;
            nums[slow] = nums[fast]; // 發現新元素，搬移到 slow 的下一個位置
        }
    }
   
    // 長度為最後的索引 + 1
    return slow + 1;
}

/*
 * ============================================================================
 * 【原始錯誤實作紀錄與盲點分析】
 * 錯誤原因：
 * 1. 陣列越界 (Out-of-Bounds Buffer Overflow)：
 *    內層的 `while(nums[slow] == nums[fast])` 沒有檢查 `fast < numsSize`！
 *    當陣列後續全部重複（例如 [1, 1, 1]），fast 就會一直加加，衝出陣列邊界讀取野記憶體。
 * 2. 邊界賦值越界：
 *    即便內層跳出時恰好 fast == numsSize，下方的 `nums[++slow] = nums[fast];`
 *    依然會去讀取已經越界的 nums[numsSize] 垃圾值並覆寫進陣列。
 * 3. 結構冗餘：
 *    雙指針原地覆寫不需要內層 while 巢狀迴圈，只要外層一個單向 for 迴圈即可 $O(N)$ 搞定。
 * ============================================================================
int removeDuplicates_wrong(int* nums, int numsSize) {
    if(nums == NULL)
    {
        return 0;
    }
    if(numsSize == 1)
    {
        return 1;
    }
    int slow = 0;
    int fast = 0;
    int k = 0;
    while(fast < numsSize)
    {
        while(nums[slow] == nums[fast]) // ❌ 盲點 1：缺少 fast < numsSize 防護，越界狂讀
        {
            fast++;
        }
        k++;
        nums[++slow] = nums[fast]; // ❌ 盲點 2：fast 走到尾端時會讀取 nums[numsSize] 越界記憶體
    }
   
    return k;
}
*/

int main(void) {
    /* 測試測資 1 */
    int nums1[] = {1, 1, 2};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    int k1 = removeDuplicates(nums1, n1);
    printf("測資 1 結果: k = %d (預期: 2), 前 %d 個元素: ", k1, k1);
    for (int i = 0; i < k1; i++) {
        printf("%d ", nums1[i]);
    }
    printf("\n");

    /* 測試測資 2 */
    int nums2[] = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    int k2 = removeDuplicates(nums2, n2);
    printf("測資 2 結果: k = %d (預期: 5), 前 %d 個元素: ", k2, k2);
    for (int i = 0; i < k2; i++) {
        printf("%d ", nums2[i]);
    }
    printf("\n");

    /* 測試測資 3 (單一元素邊界) */
    int nums3[] = {1};
    int n3 = sizeof(nums3) / sizeof(nums3[0]);
    int k3 = removeDuplicates(nums3, n3);
    printf("測資 3 結果: k = %d (預期: 1), 前 %d 個元素: ", k3, k3);
    for (int i = 0; i < k3; i++) {
        printf("%d ", nums3[i]);
    }
    printf("\n");

    return 0;
}
