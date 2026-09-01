#include <stdio.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 題目 3【無重複字元的最長子字串 (Longest Substring Without Repeating Characters - LeetCode #3)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥 (全球大廠面試 Top 3 超級神題)
// 演算法：滑動窗口 + 上次出現位置陣列 (Sliding Window) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

int lengthOfLongestSubstring(char* s)
{
    if (s == NULL)
    {
        return 0;
    }

    // 1. 記錄 128 個 ASCII 字元上次出現的位置，初始全設為 -1
    int last_pos[128];
    for (int i = 0; i < 128; i++)
    {
        last_pos[i] = -1;
    }

    int max_len = 0;
    int left = 0;
    int len = strlen(s);

    // 2. 右邊界 right 逐一向右擴展窗口
    for (int right = 0; right < len; right++)
    {
        unsigned char c = (unsigned char)s[right];

        // 3. 若字元 c 之前出現過且在當前窗口內，left 瞬間跳躍！
        if (last_pos[c] >= left)
        {
            left = last_pos[c] + 1;
        }

        // 4. 更新字元 c 最新出現的索引位置
        last_pos[c] = right;

        // 5. 計算當前窗口長度並更新最大值
        int current_len = right - left + 1;
        if (current_len > max_len)
        {
            max_len = current_len;
        }
    }

    return max_len;
}

int main()
{
    // 測試 1: "abcabcbb" -> 3
    char s1[] = "abcabcbb";
    int ans1 = lengthOfLongestSubstring(s1);
    printf("測試 1 通過: \"abcabcbb\" 最長無重複長度: %d (預期: 3)\n", ans1);
    assert(ans1 == 3);

    // 測試 2: "bbbbb" -> 1
    char s2[] = "bbbbb";
    int ans2 = lengthOfLongestSubstring(s2);
    printf("測試 2 通過: \"bbbbb\" 最長無重複長度: %d (預期: 1)\n", ans2);
    assert(ans2 == 1);

    // 測試 3: "pwwkew" -> 3
    char s3[] = "pwwkew";
    int ans3 = lengthOfLongestSubstring(s3);
    printf("測試 3 通過: \"pwwkew\" 最長無重複長度: %d (預期: 3)\n", ans3);
    assert(ans3 == 3);

    // 測試 4: "" -> 0
    char s4[] = "";
    int ans4 = lengthOfLongestSubstring(s4);
    printf("測試 4 通過: \"\" 最長無重複長度: %d (預期: 0)\n", ans4);
    assert(ans4 == 0);

    printf("\n🎉 p3.c 無重複字元的最長子字串所有測試案例全數 100%% 通過！\n");
    return 0;
}
