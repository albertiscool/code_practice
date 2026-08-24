#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 1【反轉字串 (Reverse String - LeetCode #344)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥
// 演算法：雙指標對撞反轉法 ($O(N)$ 時間, $O(1)$ 空間)
// ============================================================================
void reverseString(char* s, int sSize)
{
    char *head = s;
    char *tail = s + sSize - 1;

    // 只要 head 還在 tail 左邊，就進行字元交換
    while (head < tail)
    {
        char temp = *head;
        *head = *tail;
        *tail = temp;

        // 🌟 關鍵修正：指針移動！head 往右移、tail 往左移
        head++;
        tail--;
    }
}

int main()
{
    // 測試 1: ["h","e","l","l","o"] -> ["o","l","l","e","h"]
    char s1[] = {'h', 'e', 'l', 'l', 'o', '\0'};
    reverseString(s1, 5);
    printf("測試 1 反轉後: %s (預期: olleh)\n", s1);
    assert(s1[0] == 'o' && s1[1] == 'l' && s1[2] == 'l' && s1[3] == 'e' && s1[4] == 'h');

    // 測試 2: ["H","a","n","n","a","h"] -> ["h","a","n","n","a","H"]
    char s2[] = {'H', 'a', 'n', 'n', 'a', 'h', '\0'};
    reverseString(s2, 6);
    printf("測試 2 反轉後: %s (預期: hannah)\n", s2);
    assert(s2[0] == 'h' && s2[5] == 'H');

    printf("\n🎉 p1.c 反轉字串 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討：
void reverseString_original(char* s, int sSize)
{
    char *head = s;
    char *tail = s + sSize - 1;
    while(head <= tail)
    {
        char temp = *head;
        *head = *tail;
        *tail = temp;

        // 💡 致命問題（死迴圈 Infinite Loop）：
        // 交換完資料後，忘記將指標移動了！(少了 head++ 與 tail--)
        // 這會導致 head <= tail 永遠成立，程式會無窮迴圈卡死在那裡！
    }
}
*/
