#include <stdio.h>
#include <string.h>
#include <assert.h>

// ============================================================================
// 題目 1【手寫字串原地反轉 (In-Place String Reversal - 韌體經典題)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (系統廠與韌體面試必考底層題)
// 演算法：首尾雙指標原地對撞法 - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================

void my_strrev(char* s)
{
    // 1. 防呆處理：若是 NULL 指標或空字串，直接返回
    if (s == NULL || *s == '\0')
    {
        return;
    }

    // 2. 自力計算字串長度 (純 C 手寫不依賴庫函式)
    int len = 0;
    while (s[len] != '\0')
    {
        len++;
    }

    // 3. 首尾雙指標向中間收縮
    int left = 0;
    int right = len - 1;

    while (left < right)
    {
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;
        left++;
        right--;
    }
}

int main()
{
    // 測試 1: "hello" -> "olleh"
    char s1[] = "hello";
    my_strrev(s1);
    assert(strcmp(s1, "olleh") == 0);
    printf("測試 1 通過: \"hello\" 反轉為 \"%s\"\n", s1);

    // 測試 2: "Taiwan" -> "nawiaT"
    char s2[] = "Taiwan";
    my_strrev(s2);
    assert(strcmp(s2, "nawiaT") == 0);
    printf("測試 2 通過: \"Taiwan\" 反轉為 \"%s\"\n", s2);

    // 測試 3: "A" -> "A"
    char s3[] = "A";
    my_strrev(s3);
    assert(strcmp(s3, "A") == 0);
    printf("測試 3 通過: \"A\" 反轉為 \"%s\"\n", s3);

    // 測試 4: "" -> ""
    char s4[] = "";
    my_strrev(s4);
    assert(strcmp(s4, "") == 0);
    printf("測試 4 通過: 空字串反轉為 \"%s\"\n", s4);

    // 測試 5: NULL 防呆測試
    my_strrev(NULL);
    printf("測試 5 通過: NULL 指標防呆保護正常\n");

    printf("\n🎉 p1.c 手寫字串原地反轉所有測試案例全數 100%% 通過！\n");
    return 0;
}
