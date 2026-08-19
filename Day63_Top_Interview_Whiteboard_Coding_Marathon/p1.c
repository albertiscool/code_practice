#include <stdio.h>
#include <assert.h>

// ============================================================================
// 標準滿分解答：二分搜尋法 (Binary Search) - 100% 防溢位滿分寫法！
// ============================================================================
int binarySearch(int arr[], int size, int target)
{
    int left = 0;
    int right = size - 1;

    // 邊界條件：left <= right
    while (left <= right)
    {
        // 🌟 頂級防溢位寫法：避免 (left + right) 超出 32-bit INT_MAX
        int mid = left + (right - left) / 2;

        if (arr[mid] == target)
        {
            return mid; // 命中目標，回傳索引
        }
        else if (arr[mid] > target)
        {
            right = mid - 1; // 目標在左半邊
        }
        else
        {
            left = mid + 1;  // 目標在右半邊
        }
    }

    return -1; // 找不到目標
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    int arr[] = {-1, 0, 3, 5, 9, 12};
    int size = sizeof(arr) / sizeof(arr[0]);

    // 測試 1：尋找存在的數值 9 (預期索引: 4)
    int idx1 = binarySearch(arr, size, 9);
    printf("搜尋 9 -> 索引: %d (預期: 4)\n", idx1);
    assert(idx1 == 4);

    // 測試 2：尋找不存在的數值 2 (預期索引: -1)
    int idx2 = binarySearch(arr, size, 2);
    printf("搜尋 2 -> 索引: %d (預期: -1)\n", idx2);
    assert(idx2 == -1);

    // 測試 3：邊界測試 - 尋找最左邊 -1 (預期索引: 0)
    int idx3 = binarySearch(arr, size, -1);
    printf("搜尋 -1 -> 索引: %d (預期: 0)\n", idx3);
    assert(idx3 == 0);

    // 測試 4：邊界測試 - 尋找最右邊 12 (預期索引: 5)
    int idx4 = binarySearch(arr, size, 12);
    printf("搜尋 12 -> 索引: %d (預期: 5)\n", idx4);
    assert(idx4 == 5);

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}