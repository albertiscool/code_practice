#include <stdio.h>
#include <assert.h>

// ============================================================================
// 標準滿分解答：移動零 (Move Zeroes) - 快慢雙指標原地壓實法 ($O(N)$ 時間, $O(1)$ 空間)
// ============================================================================
void moveZeroes(int* nums, int numsSize)
{
    int slow = 0;

    // 階段 1：將所有非零元素由左往右依序填入 slow 位置
    for (int fast = 0; fast < numsSize; fast++)
    {
        if (nums[fast] != 0)
        {
            nums[slow++] = nums[fast];
        }
    }

    // 階段 2：將 slow 後面剩餘的所有格子補零
    while (slow < numsSize)
    {
        nums[slow++] = 0;
    }
}

// ============================================================================
// 寫法 2：雙指標單次遍歷交換法 (One-Pass Swap)
// ============================================================================
void moveZeroes_swap(int* nums, int numsSize)
{
    int slow = 0;
    for (int fast = 0; fast < numsSize; fast++)
    {
        if (nums[fast] != 0)
        {
            int temp = nums[slow];
            nums[slow] = nums[fast];
            nums[fast] = temp;
            slow++;
        }
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
    // 測試 1：[0, 1, 0, 3, 12] -> 預期: [1, 3, 12, 0, 0]
    int arr1[] = {0, 1, 0, 3, 12};
    printf("原陣列 1: ");
    printArray(arr1, 5);
    moveZeroes(arr1, 5);
    printf("移動後 1: ");
    printArray(arr1, 5);
    assert(arr1[0] == 1 && arr1[1] == 3 && arr1[2] == 12 && arr1[3] == 0 && arr1[4] == 0);

    // 測試 2：[0] -> 預期: [0]
    int arr2[] = {0};
    moveZeroes(arr2, 1);
    assert(arr2[0] == 0);

    // 測試 3：全部非零 [2, 1] -> 預期: [2, 1]
    int arr3[] = {2, 1};
    moveZeroes(arr3, 2);
    assert(arr3[0] == 2 && arr3[1] == 1);

    // 測試 4：全部為零 [0, 0, 0] -> 預期: [0, 0, 0]
    int arr4[] = {0, 0, 0};
    moveZeroes(arr4, 3);
    assert(arr4[0] == 0 && arr4[1] == 0 && arr4[2] == 0);

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}