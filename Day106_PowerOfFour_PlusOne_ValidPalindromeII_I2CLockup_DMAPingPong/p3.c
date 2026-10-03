#include <stdio.h>
#include <stdbool.h>
#include <string.h>

/**
 * ============================================================================
 * 【Day 106 - 題目三】驗證回文串 II (LeetCode #680 - Valid Palindrome II)
 * 【考試頻率】電子五哥 (廣達、緯創、和碩) / 系統廠 經典雙指標字串題
 * 【LeetCode 難度】Easy (初階精準題)
 * ============================================================================
 * 【題目說明】
 * 給定一個非空字串 s，請判斷是否能在「最多刪除一個字元」的前提下，將其變成一個回文字串。
 * 若可以，回傳 true；否則回傳 false。
 * 
 * 【範例 1】
 * 輸入: s = "aba"
 * 輸出: true
 * 
 * 【範例 2】
 * 輸入: s = "abca"
 * 輸出: true
 * 解釋: 可以刪除字元 'c'，使其變成 "aba"。
 * 
 * 【範例 3】
 * 輸入: s = "abc"
 * 輸出: false
 * 
 * 【限制條件】
 * 1. 1 <= s.length <= 10^5
 * 2. s 由小寫英文字母組成
 * ============================================================================
 */

// 輔助函式：判斷子字串 s[left ... right] 是否為純回文
bool isSubPalindrome(char* s, int left, int right) {
    while (left < right) {
        if (s[left] != s[right]) {
            return false;
        }
        left++;
        right--;
    }
    return true;
}

bool validPalindrome(char* s) {
    int left = 0;
    int right = strlen(s) - 1;

    while (left < right) {
        if (s[left] != s[right]) {
            // ⚠️ 關鍵分歧點：當遇到第一個不相等的字元時，我們擁有一次「刪除機會」
            // 此時不能貪心只往一邊走，必須做二選一驗證：
            // 分支 A：刪除左邊字元 -> 檢查剩餘的 s[left + 1 ... right] 是否為回文
            // 分支 B：刪除右邊字元 -> 檢查剩餘的 s[left ... right - 1] 是否為回文
            // 只要其中一條路徑成功，即可判定為 true！
            return isSubPalindrome(s, left + 1, right) || isSubPalindrome(s, left, right - 1);
        }
        left++;
        right--;
    }

    // 若完全沒有不相等字元，本身即為回文
    return true;
}

/*
===============================================================================
【原始程式碼盲點與改進分析】
-------------------------------------------------------------------------------
1. ⚠️ 貪心選擇盲目性（Greedy Ambiguity，本題最大陷阱！）：
   - 原寫法：
     ```c
     else if(s[left + 1] == s[right]) {
         left++;
     } else {
         right--;
     }
     ```
   - 致命盲點：
     當 `s[left] != s[right]` 時，可能「左邊跳過一個字元」與「右邊跳過一個字元」
     兩者看下一步「都剛好等於對向字元」！
     例如字串片段 "...cupuuf..." 與 "...upucul..."：
     如果無腦優先執行 `left++`，後面可能會在幾步之後撞牆失敗；
     但如果當初選擇 `right--`，整條字串其實是合法的回文！
   - 結論：
     遇到衝突時「不能只看下一步直接下死決定」，因為刪除機會只有一次，
     正確做法是把剩下的兩條路各自交給輔助函式驗證（`Branch A || Branch B`）。

2. 雙指標位移疊加（跳步 Bug）：
   - 原寫法在 `if` 區塊內執行了 `left++;`（第 49 行），
     但在 `if` 區塊外面第 57 行又無條件執行了一次 `left++;`！
   - 導致 `left` 一口氣前進了 2 步，跳過了原本應該拿來跟 `right` 比對的字元！

-------------------------------------------------------------------------------
【實測成績】
- 官方評判：Accepted (通過全部測資)
- 執行時間：0 ms (Beats 100.00% C submissions) ⚡
- 記憶體消耗：12.1 MB (Beats 100.00% C submissions) 🚀

-------------------------------------------------------------------------------
【原始程式碼存檔】
bool validPalindrome(char* s) {
    // 請在此處撰寫你的程式碼
    int left = 0;
    int right = strlen(s) - 1;
    int count = 0;
    while(left < right)
    {
        if(s[left] != s[right])
        {
            if(s[left + 1] != s[right] && s[left] != s[right - 1])
            {
                return false;
            }
            else if(s[left + 1] == s[right])
            {
                left++;
            }
            else
            {
                right--;
            }
            count++;
        }
        left++;
        right--;
    }
    
    return count <= 1;
}
===============================================================================
*/
