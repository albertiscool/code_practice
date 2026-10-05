#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

// ============================================================================
// 題目 1【刪除排序陣列中的重複項 (Remove Duplicates from Sorted Array - LeetCode #26)】
// 演算法：快慢雙指標原地壓縮法 ($O(N)$ 時間, $O(1)$ 空間)
// ============================================================================
int removeDuplicates(int* nums, int numsSize)
{
    // 邊界防禦：空陣列
    if (numsSize == 0) return 0;

    int slow = 0; // slow 指向目前已確認的不重複陣列最後一個位置

    // fast 負責由左往右掃描整個陣列
    for (int fast = 1; fast < numsSize; fast++)
    {
        // 只要遇到與 nums[slow] 不同的全新數字
        if (nums[fast] != nums[slow])
        {
            slow++;                  // slow 往前推一格
            nums[slow] = nums[fast]; // 將新數字填入
        }
    }

    // 🌟 注意：slow 是陣列索引 (0-indexed)，因此不重複的元素總長度是 slow + 1！
    return slow + 1;
}

int main()
{
    // 測試 1: [1, 1, 2] -> 預期長度 2，陣列前兩項為 [1, 2]
    int nums1[3] = {1, 1, 2};
    int k1 = removeDuplicates(nums1, 3);
    printf("測試 1 新長度: %d (預期: 2)\n", k1);
    assert(k1 == 2 && nums1[0] == 1 && nums1[1] == 2);

    // 測試 2: [0, 0, 1, 1, 1, 2, 2, 3, 3, 4] -> 預期長度 5，陣列為 [0, 1, 2, 3, 4]
    int nums2[10] = {0, 0, 1, 1, 1, 2, 2, 3, 3, 4};
    int k2 = removeDuplicates(nums2, 10);
    printf("測試 2 新長度: %d (預期: 5)\n", k2);
    assert(k2 == 5 && nums2[0] == 0 && nums2[1] == 1 && nums2[2] == 2 && nums2[3] == 3 && nums2[4] == 4);

    // 測試 3: 單一元素 [5] -> 長度 1
    int nums3[1] = {5};
    int k3 = removeDuplicates(nums3, 1);
    assert(k3 == 1 && nums3[0] == 5);

    printf("\n🎉 p1.c 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（快慢指標思維方向完全正確！）：
int removeDuplicates_original(int* nums, int numsSize)
{
    if(numsSize == 0 || numsSize == 1) return numsSize;

    int slow = 0;
    int fast = 1;
    
    while(fast < numsSize)
    {
        // 💡 盲點 1：內層 while 漏掉了 fast < numsSize 防呆！
        // 當 fast 走到陣列最後一格時，fast++ 會變成 numsSize，
        // 下一次比較 nums[fast] 就會記憶體越界讀取垃圾值！
        while(fast < numsSize && nums[slow] == nums[fast])
        {
            fast++;
        }

        // 💡 盲點 2：如果 fast 已經越界到底了，不能再寫入 nums[++slow] = nums[fast]！
        if (fast < numsSize) {
            nums[++slow] = nums[fast];
        }
    }
    
    // 💡 盲點 3：slow 是最後一個元素的下標 (Index)，長度必須回傳 slow + 1！
    return slow + 1;
}
*/
