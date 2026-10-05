#include <stdio.h>
#include <string.h>

// ============================================================================
// 題目 1【反轉字串 (Reverse String - LeetCode #344)】
// 難度：🟢 系統廠高頻基本題 | 出題頻率：🔥🔥🔥🔥🔥 (廣達/緯創/台達電/和碩白板手寫必備)
// 題目說明：編寫一個函式，其功能是將輸入的字串反轉過來。輸入字串以字元陣列 s 的形式給出。
//           你必須「原地 (in-place)」修改輸入陣列、使用 O(1) 的額外空間解決這一問題。
// 範例 1：
//    輸入：s = ["h","e","l","l","o"]
//    輸出：["o","l","l","e","h"]
// 範例 2：
//    輸入：s = ["H","a","n","n","a","h"]
//    輸出：["h","a","n","n","a","H"]
// 限制條件：
//    - 1 <= s.length <= 10^5
//    - s[i] 都是 ASCII 碼表中的可列印字元。
// ============================================================================

void reverseString(char* s, int sSize) {
    // TODO: 請在純白板模式下完成實作 (請勿尋求提示)
    if(sSize == 0 || sSize == 1)
    {
        return;
    }
    int left = 0;
    int right = sSize - 1;
    while(left < right)
    {
        char temp = s[left];
        s[left] = s[right];
        s[right] = temp;
        left++;
        right--;
    }
}

/*
 * ============================================================================
 * 🔍 原始盲點檢討 (Mistake Analysis Archive)
 * ============================================================================
 * 【原始寫法】：
 *     char temp = s[left];
 *     s[left] = s[right];
 *     s[right] = s[temp];   // ⚠️ 盲點：temp 已經是取出來的字元值 (例如 'h'，ASCII 為 104)
 *                          //        寫成 s[temp] 相當於去存取 s[104] (陣列越界 Out-of-bounds)！
 *                          //        若記憶體後面剛好為 0，就會把 '\0' 寫入 s[right]，導致字串被腰斬。
 * 【修正方式】：
 *     s[right] = temp;     // 直接將暫存的字元 temp 指派給 s[right]
 * ============================================================================
 */

int main(void) {
    /* 測試測資 1: "hello" -> "olleh" */
    char s1[] = "hello";
    int n1 = strlen(s1);
    reverseString(s1, n1);
    printf("測資 1 結果: \"%s\" (預期: \"olleh\")\n", s1);

    /* 測試測資 2: "Hannah" -> "hannaH" */
    char s2[] = "Hannah";
    int n2 = strlen(s2);
    reverseString(s2, n2);
    printf("測資 2 結果: \"%s\" (預期: \"hannaH\")\n", s2);

    /* 測試測資 3: 單一字元 "a" -> "a" */
    char s3[] = "a";
    int n3 = strlen(s3);
    reverseString(s3, n3);
    printf("測資 3 結果: \"%s\" (預期: \"a\")\n", s3);

    return 0;
}
