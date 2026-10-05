#include <stdio.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 題目 2【字串中的第一個唯一字元 (First Unique Character - LeetCode #387)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：頻率統計陣列 + 兩遍走訪 (Two-Pass Frequency Array) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

int firstUniqChar(char* s)
{
    // 防呆處理
    if (s == NULL || *s == '\0')
    {
        return -1;
    }

    // 1. 先計算並快取字串長度，避免在 for 迴圈條件中重複呼叫 strlen 造成 O(N^2)
    int len = strlen(s);

    // 2. 建立 26 個英文字母頻率統計陣列
    int count[26] = {0};

    // 3. 第一遍走訪：統計每個字元出現的次數
    for (int i = 0; i < len; i++)
    {
        count[s[i] - 'a']++;
    }

    // 4. 第二遍走訪：依字串原本順序，找出第一個計數為 1 的字元
    for (int j = 0; j < len; j++)
    {
        if (count[s[j] - 'a'] == 1)
        {
            return j; // 命中！立即回傳索引
        }
    }

    // 5. 若皆無唯一字元，回傳 -1
    return -1;
}

int main()
{
    // 測試 1: "leetcode" -> 0
    char s1[] = "leetcode";
    int ans1 = firstUniqChar(s1);
    printf("測試 1 通過: \"leetcode\" 第一個唯一字元索引: %d (預期: 0)\n", ans1);
    assert(ans1 == 0);

    // 測試 2: "loveleetcode" -> 2 ('v')
    char s2[] = "loveleetcode";
    int ans2 = firstUniqChar(s2);
    printf("測試 2 通過: \"loveleetcode\" 第一個唯一字元索引: %d (預期: 2)\n", ans2);
    assert(ans2 == 2);

    // 測試 3: "aabb" -> -1
    char s3[] = "aabb";
    int ans3 = firstUniqChar(s3);
    printf("測試 3 通過: \"aabb\" 無唯一字元: %d (預期: -1)\n", ans3);
    assert(ans3 == -1);

    printf("\n🎉 p2.c 第一個唯一字元所有測試案例全數 100%% 通過！\n");
    return 0;
}
