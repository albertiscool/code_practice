#include <stdio.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 題目 2【字串中的第一個唯一字元 (First Unique Character in a String - LeetCode #387)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥
// 演算法：頻率統計陣列法 (Frequency Array / Hash Table) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================
int firstUniqChar(char* s)
{
    // 防呆處理
    if (s == NULL) {
        return -1;
    }

    // 1. 初始化長度為 26 的字元出現次數統計陣列 (空間複雜度 O(1))
    int count[26] = {0};

    // 2. 第 1 次走訪字串：統計每一個字母出現的總次數 (時間複雜度 O(N))
    for (int i = 0; s[i] != '\0'; i++)
    {
        count[s[i] - 'a']++;
    }

    // 3. 第 2 次走訪字串：找到第一個出現次數恰好為 1 的字母
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (count[s[i] - 'a'] == 1)
        {
            return i; // 找到第一個不重複字母，立刻回傳 index
        }
    }

    // 4. 若全都有重複，回傳 -1
    return -1;
}

int main()
{
    // 測試 1: "leetcode" -> 0
    char s1[] = "leetcode";
    int ans1 = firstUniqChar(s1);
    printf("測試 1 索引: %d (預期: 0)\n", ans1);
    assert(ans1 == 0);

    // 測試 2: "loveleetcode" -> 2
    char s2[] = "loveleetcode";
    int ans2 = firstUniqChar(s2);
    printf("測試 2 索引: %d (預期: 2)\n", ans2);
    assert(ans2 == 2);

    // 測試 3: "aabb" -> -1
    char s3[] = "aabb";
    int ans3 = firstUniqChar(s3);
    printf("測試 3 索引: %d (預期: -1)\n", ans3);
    assert(ans3 == -1);

    printf("\n🎉 p2.c 第一個唯一字元 所有測試案例全數 100%% 通過！\n");
    return 0;
}
