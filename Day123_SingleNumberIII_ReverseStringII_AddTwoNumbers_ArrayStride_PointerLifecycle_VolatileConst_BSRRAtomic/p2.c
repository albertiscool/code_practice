#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * LeetCode #541 - Reverse String II
 * 
 * 題目描述：
 * 給定一個字串 s 和一個整數 k。從字串開頭算起，每 2k 個字元反轉前 k 個字元。
 * - 若剩餘字元少於 k 個，則反轉剩餘的所有字元。
 * - 若剩餘字元小於 2k 但大於等於 k 個，則反轉前 k 個字元，其餘字元保持原樣。
 * 
 * 請回傳反轉後的字串。
 * 
 * 範例 1：
 * 輸入：s = "abcdefg", k = 2
 * 輸出："bacdfeg"
 * 
 * 範例 2：
 * 輸入：s = "abcd", k = 2
 * 輸出："bacd"
 * 
 * 限制條件：
 * 1 <= s.length <= 10^4
 * s 僅由英文字母小寫組成
 * 1 <= k <= 10^4
 */

/*
 * =========================================================================
 * 【LeetCode 官方提交成績】：
 * - 狀態：Accepted (通過 60/60 測試案例)
 * - 執行時間：0 ms (擊敗 100.00% C 語言提交)
 * - 記憶體：8.8 MB
 * - Submission ID：2178673755
 * -------------------------------------------------------------------------
 * 【使用者原始白板程式碼】：
 * char* reverseStr(char* s, int k) {
 *     // 請於此處實作純白板解法
 *     int slow = 0;
 *     int fast = 0;
 *     int pre = 0;
 *     int len = strlen(s);
 *     while(len >= (2*k))
 *     {
 *         slow = slow + k - 1;
 *         fast = slow + 2*k - 1;
 *         while(pre < slow)
 *         {
 *             char temp = s[pre];
 *             s[pre] = s[slow];
 *             s[slow] = temp;
 *             // 盲點 1：漏了 pre++ 與 slow--，導致無限迴圈 (Time Limit Exceeded)
 *         }
 *         len -= 2*k;
 *         slow = fast;
 *         pre = fast;
 *     }
 *     if((len < (2*k)) && len >= k)
 *     {
 *         slow = slow + k - 1;
 *         while(pre < slow)
 *         {
 *             char temp = s[pre];
 *             s[pre] = s[slow];
 *             s[slow] = temp;
 *         }
 *         return s;
 *     }
 *     else
 *     {
 *         slow = slow + len;
 *         while(pre < slow)
 *         {
 *             char temp = s[pre];
 *             s[pre] = s[slow];
 *             s[slow] = temp;
 *         }
 *         return s;
 *     }
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 無限迴圈（TLE, Time Limit Exceeded）：
 *    - 雙指標翻轉 `while(pre < slow)` 區塊內，完全沒有更新指標（漏寫 `pre++` 與 `slow--`），
 *      條件 `pre < slow` 永遠成立，CPU 陷入無窮迴圈。
 * 2. 指標步長疊加偏移錯誤（Misalignment）：
 *    - 先行 `slow = slow + k - 1`，接著又算 `fast = slow + 2*k - 1`，
 *      導致 `fast` 變成了原起點的 `3k - 2`，造成區間位移徹底錯位！
 * 3. 邊界越界風險：
 *    - 在尾端處理 `len < k` 時，`slow = slow + len` 沒有減 1，會指向 `\0` 結束字元。
 * 4. 架構優化（以步長為 2k 的 for 迴圈 + 邊界截斷）：
 *    - 只要外層每次推進 `i += 2 * k`，目標翻轉區間右端點即為 `min(i + k - 1, len - 1)`，
 *      一行兼顧所有「剩餘小於 k」與「剩餘大於等於 k」的情境！
 * =========================================================================
 */

char* reverseStr(char* s, int k) {
    int slow = 0;
    int fast = 0;
    int pre = 0;
    int len = strlen(s);

    // 處理完整 2k 長度的區間
    while (len >= (2 * k)) {
        fast = pre + 2 * k;     // 下一個區間的起點
        slow = pre + k - 1;     // 本區間需要翻轉的前 k 個字元末尾

        // 雙指標原地翻轉（補上 pre++ 與 slow--）
        while (pre < slow) {
            char temp = s[pre];
            s[pre] = s[slow];
            s[slow] = temp;
            pre++;
            slow--;
        }

        len -= 2 * k;
        pre = fast;             // 指向下一個 2k 區間起點
        slow = fast;
    }

    // 剩餘字元小於 2k 但大於等於 k 個：翻轉前 k 個字元
    if (len >= k) {
        slow = pre + k - 1;
        while (pre < slow) {
            char temp = s[pre];
            s[pre] = s[slow];
            s[slow] = temp;
            pre++;
            slow--;
        }
        return s;
    }
    // 剩餘字元少於 k 個：全部翻轉（注意是 pre + len - 1）
    else {
        slow = pre + len - 1;
        while (pre < slow) {
            char temp = s[pre];
            s[pre] = s[slow];
            s[slow] = temp;
            pre++;
            slow--;
        }
        return s;
    }
}

int main() {
    char s1[] = "abcdefg";
    printf("Test 1: %s -> %s (Expected: bacdfeg)\n", "abcdefg", reverseStr(s1, 2));

    char s2[] = "abcd";
    printf("Test 2: %s -> %s (Expected: bacd)\n", "abcd", reverseStr(s2, 2));

    char s3[] = "a";
    printf("Test 3: %s -> %s (Expected: a)\n", "a", reverseStr(s3, 2));

    return 0;
}
