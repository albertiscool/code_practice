#include <stdio.h>
#include <stdbool.h>

/* 
 * 題目 2: 搜尋旋轉排序陣列 (LeetCode #33 - Search in Rotated Sorted Array)
 * 難度: 🟡 一線 IC 設計廠 (聯發科、聯詠、群聯) 核心演算法天王題
 * 
 * 題目說明:
 * 整數陣列 nums 原本按嚴格升序排序 (所有數值互不相同)。
 * 在傳遞給函式之前，nums 在某個未知的軸心 (Pivot) 進行了旋轉 (例如 [0,1,2,4,5,6,7] 可能變為 [4,5,6,7,0,1,2])。
 * 給定旋轉後的陣列 nums 以及一個目標整數 target：
 * - 若 target 存在於陣列中，請回傳其索引 (0-indexed)。
 * - 若不存在，請回傳 -1。
 * 
 * 範例 1:
 *   輸入: nums = [4, 5, 6, 7, 0, 1, 2], target = 0
 *   輸出: 4
 * 
 * 範例 2:
 *   輸入: nums = [4, 5, 6, 7, 0, 1, 2], target = 3
 *   輸出: -1
 * 
 * 範例 3:
 *   輸入: nums = [1], target = 0
 *   輸出: -1
 * 
 * 限制條件:
 * - 時間複雜度要求: 嚴格 O(log N)
 * - 空間複雜度要求: O(1)
 */
int search(int* nums, int numsSize, int target) {
    if (nums == NULL || numsSize == 0) {
        return -1;
    }

    int left = 0;
    int right = numsSize - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;

        if (nums[mid] == target) {
            return mid;
        }

        // 核心判斷：哪一半是有序的？
        // 情況 1：左半段 [left ... mid] 是嚴格/連續有序的
        if (nums[left] <= nums[mid]) {
            // target 是否落在這個連續遞增的左區間內？
            if (nums[left] <= target && target < nums[mid]) {
                right = mid - 1; // 確定在左半段
            } else {
                left = mid + 1;  // 否則必然在右半段
            }
        } 
        // 情況 2：右半段 [mid ... right] 是嚴格/連續有序的
        else {
            // target 是否落在這個連續遞增的右區間內？
            if (nums[mid] < target && target <= nums[right]) {
                left = mid + 1;  // 確定在右半段
            } else {
                right = mid - 1; // 否則必然在左半段
            }
        }
    }

    return -1;
}

/* 
 * ============================================================================
 * 🔍【原始代碼盲點檢視與解析】
 * ============================================================================
 * 你原先寫的代碼：
 * 
 *     if(nums[left] < nums[mid] && target < nums[mid]) // 左邊遞增
 *     {
 *         right = mid - 1;
 *     }
 *     else
 *     {
 *         left = mid + 1;
 *     }
 * 
 * 盲點 1: 缺少左邊界檢查 (nums[left] <= target)
 *   以 nums = [4, 5, 6, 7, 0, 1, 2], target = 0 為例：
 *   此時 left = 0(4), mid = 3(7)。
 *   雖然 target < nums[mid] (0 < 7) 成立，但 target(0) 根本不在 [4, 5, 6, 7] 裡面！
 *   因為它比 nums[left](4) 還小，真正的 0 落在右半段 [0, 1, 2]！
 *   你的條件會誤判並把 right 縮到 mid - 1，導致搜尋範圍砍掉正確答案。
 * 
 * 盲點 2: 旋轉陣列二分搜尋的「兩層判斷架構」被扁平化
 *   二分搜尋在旋轉陣列中不能只用一個單純的 if-else，必須分兩層思考：
 *   第一層：先判斷「哪一邊是單調遞增的純淨區間」（左邊純淨 or 右邊純淨）
 *   第二層：在那個純淨區間內，用 (left <= target && target < mid) 雙邊包夾檢查 target 是否在裡面。
 *   如果在裡面，就往裡面搜；如果不在裡面，就反向搜另一半！
 * ============================================================================
 */

int main(void) {
    printf("=== Day 101 - 白板題 2: 搜尋旋轉排序陣列 (LeetCode #33) ===\n\n");

    // 測試案例 1
    int nums1[] = {4, 5, 6, 7, 0, 1, 2};
    int size1 = sizeof(nums1) / sizeof(nums1[0]);
    int target1 = 0;
    int res1 = search(nums1, size1, target1);
    printf("Test 1: target = %d\n", target1);
    printf("預期輸出: 4\n");
    printf("實際輸出: %d (%s)\n\n", res1, res1 == 4 ? "PASS" : "FAIL");

    // 測試案例 2
    int nums2[] = {4, 5, 6, 7, 0, 1, 2};
    int size2 = sizeof(nums2) / sizeof(nums2[0]);
    int target2 = 3;
    int res2 = search(nums2, size2, target2);
    printf("Test 2: target = %d\n", target2);
    printf("預期輸出: -1\n");
    printf("實際輸出: %d (%s)\n\n", res2, res2 == -1 ? "PASS" : "FAIL");

    // 測試案例 3: 單一元素
    int nums3[] = {1};
    int size3 = sizeof(nums3) / sizeof(nums3[0]);
    int target3 = 0;
    int res3 = search(nums3, size3, target3);
    printf("Test 3: target = %d\n", target3);
    printf("預期輸出: -1\n");
    printf("實際輸出: %d (%s)\n\n", res3, res3 == -1 ? "PASS" : "FAIL");

    // 測試案例 4: 目標在旋轉點左側
    int nums4[] = {4, 5, 6, 7, 0, 1, 2};
    int size4 = sizeof(nums4) / sizeof(nums4[0]);
    int target4 = 5;
    int res4 = search(nums4, size4, target4);
    printf("Test 4: target = %d\n", target4);
    printf("預期輸出: 1\n");
    printf("實際輸出: %d (%s)\n", res4, res4 == 1 ? "PASS" : "FAIL");

    return 0;
}
