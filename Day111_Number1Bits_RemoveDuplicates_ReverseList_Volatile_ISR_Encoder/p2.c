#include <stdio.h>
#include <assert.h>

/**
 * LeetCode #26: Remove Duplicates from Sorted Array
 * 
 * 題目描述：
 * 給定一個已排序的整數陣列 nums，請你「原地（in-place）」刪除重複出現的元素，
 * 使每個元素只出現一次，並回傳移除重複後陣列的新長度 k。
 * 元素的新相對順序應保持不變。
 * 
 * 限制條件：
 * - 1 <= numsSize <= 3 * 10^4
 * - -100 <= nums[i] <= 100
 * - nums 已按升序（遞增）排序
 * - 必須使用 O(1) 額外空間進行原地修改
 * 
 * 範例 1:
 *   輸入: nums = [1,1,2]
 *   輸出: k = 2, nums 前兩個元素為 [1,2]
 * 
 * 範例 2:
 *   輸入: nums = [0,0,1,1,1,2,2,3,3,4]
 *   輸出: k = 5, nums 前五個元素為 [0,1,2,3,4]
 * 
 * 面試深意：
 * 電子五哥（廣達、緯創）與系統廠白板必考第一名！考驗雙指標（快慢指標）的「原地覆寫不變量」與邊界條件，杜絕任何額外陣列配置。
 */

int removeDuplicates(int* nums, int numsSize) {
    if (nums == NULL || numsSize == 0) {
        return 0;
    }
    
    int slow = 0;
    for (int fast = 1; fast < numsSize; fast++) {
        if (nums[fast] != nums[slow]) {
            nums[++slow] = nums[fast];
        }
    }
    return slow + 1; // 回傳新長度 k (索引從 0 開始，個數為 slow + 1)
}

int main(void) {
    printf("=== Test P2: Remove Duplicates from Sorted Array ===\n");
    // Test 1: [1, 1, 2] -> 2, [1, 2]
    int nums1[] = {1, 1, 2};
    int k1 = removeDuplicates(nums1, 3);
    printf("Test 1 Result k = %d: ", k1);
    for (int i = 0; i < k1; i++) printf("%d ", nums1[i]);
    printf("\n");
    assert(k1 == 2);
    assert(nums1[0] == 1 && nums1[1] == 2);

    // Test 2: [0,0,1,1,1,2,2,3,3,4] -> 5, [0,1,2,3,4]
    int nums2[] = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int k2 = removeDuplicates(nums2, 10);
    printf("Test 2 Result k = %d: ", k2);
    for (int i = 0; i < k2; i++) printf("%d ", nums2[i]);
    printf("\n");
    assert(k2 == 5);
    assert(nums2[0] == 0 && nums2[1] == 1 && nums2[2] == 2 && nums2[3] == 3 && nums2[4] == 4);

    // Test 3: Single element [7] -> 1, [7]
    int nums3[] = {7};
    int k3 = removeDuplicates(nums3, 1);
    assert(k3 == 1 && nums3[0] == 7);
    printf("Test 3 Passed (Single element)\n");

    printf("All tests passed!\n");
    return 0;
}

/* =========================================================================
 * 🚩 【Albert 白板實戰複盤與陣列雙指標原地操作深度解析】
 * =========================================================================
 * 
 * 🏆 LeetCode 提交結果：
 * - 狀態：Accepted (362/362 測試案例全數通過！)
 * - 執行時間：0 ms (Beats 100.00%)
 * - 記憶體：12.8 MB
 * - Submission ID: 2166108452
 * 
 * 原始撰寫程式碼片段：
 * -------------------------------------------------------------------------
 * int removeDuplicates(int* nums, int numsSize) {
 *     if(nums == NULL || numsSize == 0 || numsSize == 1) {
 *         return nums;                      // ⚠️ 回傳型別錯誤：回傳了 int* 而非 int
 *     }
 *     int slow = 0;
 *     int fast = 0;
 *     while(fast < numsSize) {
 *         if(nums[slow] != nums[fast]) {
 *             nums[++slow] = nums[fast];
 *             fast++;
 *         } else {
 *             fast++;
 *         }
 *     }
 *     return nums;                          // ⚠️ 題目要求回傳長度 k，但回傳了指標 nums
 * }
 * -------------------------------------------------------------------------
 * 
 * 💡 邏輯亮點（核心思路完全正確）：
 * 1. 成功想到快慢指標（Two Pointers）原地覆寫（In-place Overwrite）的概念！
 * 2. `nums[++slow] = nums[fast]` 先移慢指標再覆寫，精準保留第一個元素不被覆蓋，嚴格 O(1) 額外空間！
 * 
 * ❌ 盲點 1：函式回傳型別與語意偏差（Compile Error: returning int* from int）
 * -------------------------------------------------------------------------
 * - 函式簽名為 `int removeDuplicates(...)`，回傳型別是整數 `int`。
 * - 題目要求回傳的是「去除重複項後的陣列長度 k」。
 * - 當原碼寫成 `return nums;` 時：
 *   編譯器會報錯警告：`warning: returning 'int *' from a function with return type 'int' makes integer from pointer without a cast [-Wint-conversion]`。
 *   在 LeetCode 與注重型別安全的韌體編譯器上，直接被列為編譯錯誤（Compile Error）。
 * - 修正：`slow` 是當前已保留之最後一個不重複元素的 0-based 索引，
 *   因此不重複元素的總個數為 `slow + 1`！結尾應寫 `return slow + 1;`。
 * 
 * ❌ 盲點 2：邊界條件簡化與快指標起始優化
 * -------------------------------------------------------------------------
 * - 邊界判斷 `if (numsSize == 1)` 其實無需單獨抽 if：
 *   當 `numsSize == 1` 時，`slow = 0`，若 `fast` 從 1 開始出發，
 *   `fast < numsSize` (1 < 1) 條件直接為假，不進入迴圈，
 *   自然回傳 `slow + 1 = 0 + 1 = 1`！
 * - 故邊界只需 `if (nums == NULL || numsSize == 0) return 0;` 即可優雅涵蓋所有極端狀況。
 * ========================================================================= */
