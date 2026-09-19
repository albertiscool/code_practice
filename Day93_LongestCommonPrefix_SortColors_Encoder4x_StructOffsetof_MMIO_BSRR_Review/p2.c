#include <stdio.h>
#include <stdlib.h>

// ============================================================================
// 題目 2【顏色分類 / 荷蘭國旗問題 (Sort Colors - LeetCode #75)】
// 難度：🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (聯發科/瑞昱/外商一線廠白板雙/三指標經典題)
// 題目說明：給定一個包含紅色、白色和藍色共 n 個元素的陣列 nums，
//           將它們「原地 (in-place)」排序，使得相同顏色的元素相鄰，並按照紅、白、藍順序排列。
//           我們使用整數 0、1 和 2 分別表示紅色、白色和藍色。
// 限制條件：
//    - n == nums.length
//    - 1 <= n <= 300
//    - nums[i] 為 0、1 或 2。
//    - ⚠️ 嚴禁使用庫函式庫的 sort()！
//    - 進階挑戰：你能否設計出時間複雜度為 O(N) 且「僅遍歷一次 (One-pass)」與額外空間 O(1) 的演算法？
// 範例 1：
//    輸入：nums = [2,0,2,1,1,0]
//    輸出：[0,0,1,1,2,2]
// 範例 2：
//    輸入：nums = [2,0,1]
//    輸出：[0,1,2]
// ============================================================================

void sortColors(int* nums, int numsSize) {
    /*
     * 正確解答 (荷蘭國旗問題 Dutch National Flag - Dijkstra 三指標原地排序):
     * 1. p0   指向 0 應當放置的最右側邊界 (左指標)
     * 2. p2   指向 2 應當放置的最左側邊界 (右指標)
     * 3. curr 當前遍歷檢驗的指針 (中指標)
     *
     * 規則：
     * - 若 nums[curr] == 0: 與 nums[p0] 交換，p0++, curr++
     * - 若 nums[curr] == 1: 已經在正確區域，curr++
     * - 若 nums[curr] == 2: 與 nums[p2] 交換，p2-- (注意: 此時 curr 不能遞增，
     *                       因為從後面換過來的元素未經檢驗，需留在原位下一輪再判斷！)
     */
    if (nums == NULL || numsSize <= 1) {
        return;
    }

    int p0 = 0;
    int curr = 0;
    int p2 = numsSize - 1;

    while (curr <= p2) {
        if (nums[curr] == 0) {
            int temp = nums[curr];
            nums[curr] = nums[p0];
            nums[p0] = temp;
            p0++;
            curr++;
        } else if (nums[curr] == 2) {
            int temp = nums[curr];
            nums[curr] = nums[p2];
            nums[p2] = temp;
            p2--;
            // ⚠️ 關鍵細節：curr 不動！因為換過來的新數字尚未檢驗過
        } else {
            // nums[curr] == 1
            curr++;
        }
    }
}

/* ============================================================================
 * ❌ 原始實作盲點分析 (已保留供複習檢討):
 * ============================================================================
 * void sortColors_wrong(int* nums, int numsSize) {
 *     int p0 = 0;
 *     int p2 = numsSize - 1;
 *     for(int i = 0; i < numsSize; i++)
 *     {
 *         // ⚠️ 盲點 1: 直接賦值 nums[p0++] = nums[i] 不是交換 (Swap)！
 *         //           這會將原本存放在 nums[p0] 或 nums[p2] 上的原始資料直接覆蓋抹煞！
 *         //           例如當把 2 丟給 p2 時，原本 p2 位置上的數字被覆蓋掉，導致永遠遺失。
 *         if(nums[i] == 0)
 *         {
 *             nums[p0++] = nums[i];
 *         }
 *         else if(nums[i] == 2)
 *         {
 *             nums[p2--] = nums[i];
 *         }
 *     }
 *     // ⚠️ 盲點 2 (語法錯誤): 迴圈步進寫成 'po++' (字母 o 代替數字 0)，導致編譯失敗:
 *     //           error: 'po' undeclared; did you mean 'p2'?
 *     for(p0; p0 < p2; po++)
 *     {
 *         nums[p0] = 1;
 *     }
 * }
 * ============================================================================ */

int main(void) {
    /* 測試測資 1: [2,0,2,1,1,0] -> [0,0,1,1,2,2] */
    int nums1[] = {2, 0, 2, 1, 1, 0};
    int n1 = sizeof(nums1) / sizeof(nums1[0]);
    sortColors(nums1, n1);
    printf("測資 1 結果: ");
    for (int i = 0; i < n1; i++) printf("%d ", nums1[i]);
    printf("(預期: 0 0 1 1 2 2)\n");

    /* 測試測資 2: [2,0,1] -> [0,1,2] */
    int nums2[] = {2, 0, 1};
    int n2 = sizeof(nums2) / sizeof(nums2[0]);
    sortColors(nums2, n2);
    printf("測資 2 結果: ");
    for (int i = 0; i < n2; i++) printf("%d ", nums2[i]);
    printf("(預期: 0 1 2)\n");

    /* 測試測資 3: 單一元素 [0] -> [0] */
    int nums3[] = {0};
    int n3 = sizeof(nums3) / sizeof(nums3[0]);
    sortColors(nums3, n3);
    printf("測資 3 結果: %d (預期: 0)\n", nums3[0]);

    return 0;
}
