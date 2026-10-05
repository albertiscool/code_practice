#include <stdio.h>
#include <assert.h>

// ============================================================================
// 標準滿分解答：合併兩個有序陣列 (Merge Sorted Array)
// 演算法：倒序三指標法 (Reverse 3-Pointers) - $O(M + N)$ 時間, $O(1)$ 空間
// ============================================================================
void merge(int* nums1, int nums1Size, int m, int* nums2, int nums2Size, int n)
{
    int p1 = m - 1;         // 指向 nums1 有效數字的最後一個 (最大值)
    int p2 = n - 1;         // 指向 nums2 的最後一個 (最大值)
    int p = m + n - 1;      // 指向 nums1 最尾端的放置位置

    // 當兩邊都還有數字可以比較時，誰大誰就放後面
    while (p1 >= 0 && p2 >= 0)
    {
        if (nums1[p1] > nums2[p2])
        {
            nums1[p] = nums1[p1];
            p1--;
        }
        else
        {
            nums1[p] = nums2[p2];
            p2--;
        }
        p--;
    }

    // 收尾：如果 nums2 還有剩餘的較小數字，全部填入 nums1 前面
    // (註：如果 nums1 還有剩，它本來就在正確位置上，不需要任何操作！)
    while (p2 >= 0)
    {
        nums1[p] = nums2[p2];
        p2--;
        p--;
    }
}

// 輔助函式：列印陣列
void printArray(int* nums, int size) {
    printf("[ ");
    for (int i = 0; i < size; i++) {
        printf("%d ", nums[i]);
    }
    printf("]\n");
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    // 測試 1：標準案例 nums1 = [1, 2, 3, 0, 0, 0], nums2 = [2, 5, 6]
    int nums1_1[6] = {1, 2, 3, 0, 0, 0};
    int nums2_1[3] = {2, 5, 6};
    printf("測試 1 合併前: ");
    printArray(nums1_1, 6);
    merge(nums1_1, 6, 3, nums2_1, 3, 3);
    printf("測試 1 合併後: ");
    printArray(nums1_1, 6); // 預期: [1, 2, 2, 3, 5, 6]
    assert(nums1_1[0] == 1 && nums1_1[1] == 2 && nums1_1[2] == 2 && 
           nums1_1[3] == 3 && nums1_1[4] == 5 && nums1_1[5] == 6);

    // 測試 2：nums2 為空
    int nums1_2[1] = {1};
    int nums2_2[1] = {};
    merge(nums1_2, 1, 1, nums2_2, 0, 0);
    assert(nums1_2[0] == 1);

    // 測試 3：nums1 為空 (只有預留空間)
    int nums1_3[1] = {0};
    int nums2_3[1] = {1};
    merge(nums1_3, 1, 0, nums2_3, 1, 1);
    assert(nums1_3[0] == 1);

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討：
void MergeSortedArray_original(int *nums1, int m, int *nums2, int n)
{
    int i1 = 0;
    int i2 = n;
    int j = 0;
    
    // 💡 盲點 1：從「前面 (0)」往後排會把尚未比較的 nums1 元素給覆蓋打亂！
    // 例如 nums1[i2] = temp 會把數字丟到後面未排序的位置，導致後續順序錯亂。
    // 💡 正解：必須採用「倒序三指標法」，從最尾端 (m + n - 1) 往回填入最大的數字！
    while(i1 < m)
    {
        while(nums1[i1] < nums2[j])
        {
            i1++;
        }
        int temp = nums1[i1];
        nums1[i1] = nums2[j];
        nums1[i2] = temp;
        j++;
    }
}
*/