#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 1【盛最多水的容器 (Container With Most Water - LeetCode #11)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：對撞雙指標貪心法 (Two-Pointer Greedy) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

// 輔助函式：取得兩數較小值
static inline int get_min(int a, int b) {
    return (a < b) ? a : b;
}

int maxArea(int* height, int heightSize)
{
    // 防呆處理
    if (height == NULL || heightSize <= 1)
    {
        return 0;
    }

    int left = 0;
    int right = heightSize - 1;
    int max_area = 0;

    // 雙向對撞走訪
    while (left < right)
    {
        // 當前面積 = 較短的一側高度 * 寬度
        int current_height = get_min(height[left], height[right]);
        int current_width = right - left;
        int area = current_height * current_width;

        if (area > max_area)
        {
            max_area = area;
        }

        // 貪心策略：移動「較矮的那一側」指標，爭取遇到更高的柱子！
        if (height[left] < height[right])
        {
            left++;
        }
        else
        {
            right--;
        }
    }

    return max_area;
}

int main()
{
    // 測試 1: [1, 8, 6, 2, 5, 4, 8, 3, 7] -> 49
    int h1[9] = {1, 8, 6, 2, 5, 4, 8, 3, 7};
    int ans1 = maxArea(h1, 9);
    printf("測試 1 最大盛水量: %d (預期: 49)\n", ans1);
    assert(ans1 == 49);

    // 測試 2: [1, 1] -> 1
    int h2[2] = {1, 1};
    int ans2 = maxArea(h2, 2);
    printf("測試 2 最大盛水量: %d (預期: 1)\n", ans2);
    assert(ans2 == 1);

    // 測試 3: [4, 3, 2, 1, 4] -> 16
    int h3[5] = {4, 3, 2, 1, 4};
    int ans3 = maxArea(h3, 5);
    printf("測試 3 最大盛水量: %d (預期: 16)\n", ans3);
    assert(ans3 == 16);

    printf("\n🎉 p1.c 盛最多水的容器所有測試案例全數 100%% 通過！\n");
    return 0;
}
