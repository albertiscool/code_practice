#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// ============================================================================
// 題目 2【兩數之和 II - 輸入有序陣列 (Two Sum II - Input Array Is Sorted - LeetCode #167)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：有序陣列對撞雙指標 (Two-Pointer Sorted Convergence) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

int* twoSum(int* numbers, int numbersSize, int target, int* returnSize)
{
    // 防呆處理
    if (numbers == NULL || numbersSize < 2)
    {
        *returnSize = 0;
        return NULL;
    }

    *returnSize = 2;
    int* result = (int*)malloc(2 * sizeof(int));
    
    int left = 0;
    int right = numbersSize - 1;

    // 雙指標向中間收縮
    while (left < right)
    {
        int sum = numbers[left] + numbers[right];

        if (sum > target)
        {
            right--; // 和太大了，右指標往左移
        }
        else if (sum < target)
        {
            left++;  // 和太小了，左指標往右移
        }
        else
        {
            // 找到唯一解！(題目要求 1-indexed 回傳)
            result[0] = left + 1;
            result[1] = right + 1;
            break;   // 找到答案立即跳出迴圈！
        }
    }

    return result;
}

int main()
{
    // 測試 1: [2, 7, 11, 15], target = 9 -> [1, 2]
    int num1[4] = {2, 7, 11, 15};
    int size1 = 0;
    int* res1 = twoSum(num1, 4, 9, &size1);
    assert(size1 == 2);
    assert(res1[0] == 1 && res1[1] == 2);
    printf("測試 1 通過: [2, 7, 11, 15], target = 9 ➔ 索引 [%d, %d]\n", res1[0], res1[1]);
    free(res1);

    // 測試 2: [2, 3, 4], target = 6 -> [1, 3]
    int num2[3] = {2, 3, 4};
    int size2 = 0;
    int* res2 = twoSum(num2, 3, 6, &size2);
    assert(size2 == 2);
    assert(res2[0] == 1 && res2[1] == 3);
    printf("測試 2 通過: [2, 3, 4], target = 6 ➔ 索引 [%d, %d]\n", res2[0], res2[1]);
    free(res2);

    // 測試 3: [-1, 0], target = -1 -> [1, 2]
    int num3[2] = {-1, 0};
    int size3 = 0;
    int* res3 = twoSum(num3, 2, -1, &size3);
    assert(size3 == 2);
    assert(res3[0] == 1 && res3[1] == 2);
    printf("測試 3 通過: [-1, 0], target = -1 ➔ 索引 [%d, %d]\n", res3[0], res3[1]);
    free(res3);

    printf("\n🎉 p2.c 兩數之和 II (有序陣列對撞) 所有測試案例全數 100%% 通過！\n");
    return 0;
}
