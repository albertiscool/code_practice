#include <stdio.h>
#include <stdbool.h>

/**
 * ============================================================================
 * 【Day 105 - 題目一】贖金信 (LeetCode #383 - Ransom Note)
 * 【考試頻率】電子五哥 (廣達、緯創、和碩) 經典字元計數與記憶體查表題
 * 【LeetCode 難度】Easy (初階精準題)
 * ============================================================================
 * 【題目說明】
 * 給定兩個字串 ransomNote 和 magazine，如果可以由 magazine 裡面的字元構成 ransomNote，
 * 則回傳 true；否則回傳 false。
 * 
 * ⚠️ 限制條件：
 * magazine 中的每個字元在 ransomNote 中只能使用一次。
 * 
 * 【範例 1】
 * 輸入: ransomNote = "a", magazine = "b"
 * 輸出: false
 * 
 * 【範例 2】
 * 輸入: ransomNote = "aa", magazine = "ab"
 * 輸出: false
 * 
 * 【範例 3】
 * 輸入: ransomNote = "aa", magazine = "aab"
 * 輸出: true
 * 
 * 【限制條件】
 * 1. 1 <= ransomNote.length, magazine.length <= 10^5
 * 2. ransomNote 和 magazine 由小寫英文字母組成
 * ============================================================================
 */

bool canConstruct(char* ransomNote, char* magazine) {
    int count[26] = {0};

    // 1. 遍歷 magazine，統計每個字母的可用數量
    for (int i = 0; magazine[i] != '\0'; i++) {
        count[magazine[i] - 'a']++;
    }

    // 2. 遍歷 ransomNote，扣除對應字母；若透支 (< 0) 立即早退 (Early Return)
    for (int i = 0; ransomNote[i] != '\0'; i++) {
        count[ransomNote[i] - 'a']--;
        if (count[ransomNote[i] - 'a'] < 0) {
            return false;
        }
    }

    return true;
}

/*
===============================================================================
【原始程式碼盲點與改進分析】
-------------------------------------------------------------------------------
1. ⚠️ C 語言指標與 sizeof 致命陷阱（面試高頻陷阱！）：
   - 原寫法：
     `int len_a = sizeof(ransomNote) / sizeof(char);`
     `int len_b = sizeof(magazine) / sizeof(char);`
   - 致命盲點：
     函式參數 `char* ransomNote` 是一個「指標（Pointer）」！
     在 64 位元架構中（如 LeetCode 評判伺服器與現代電腦），`sizeof(指標)` 的回傳值「永遠固定是 8」！
     `sizeof(char)` 是 1，因此 `len_a` 和 `len_b` 的數值被鎖死成了「8」！
   - 嚴重後果：
     - 若字串長度小於 8（如 "a"），迴圈會硬跑 8 次，讀取到超出字串結尾的隨機垃圾記憶體（Buffer Overflow）！
     - 若字串長度大於 8（題目可達 10^5），迴圈只檢查了前 8 個字母，後面的字元全部被忽略！
   - 正確作法：
     - 使用 `<string.h>` 的 `strlen()` 獲取字串長度。
     - 或直接利用 C 語言字串以空字元結尾的特性：`for (int i = 0; s[i] != '\0'; i++)`。

2. 效能優化（Early Return 早退機制）：
   - 原寫法跑了 3 次迴圈（第 1 次數 magazine、第 2 次扣 ransomNote、第 3 次檢查 26 個字母）。
   - 優化作法：在第 2 次扣除字母的當下，若發現該字母餘額小於 0（`count[x] < 0`），
     代表字母已經不夠用，直接 `return false`！完全不需要第 3 個迴圈，達到 0ms 極速。

-------------------------------------------------------------------------------
【原始程式碼存檔】
bool canConstruct(char* ransomNote, char* magazine) {
    // 請在此處撰寫你的程式碼
    int count[26] = {0};
    int len_a = sizeof(ransomNote) / sizeof(char);
    int len_b = sizeof(magazine) / sizeof(char);

    if(len_b < len_a)
    {
        return false;
    }
    
    for(int j = 0; j < len_b; j++)
    {
        count[magazine[j] - 'a']++;
    }

    for(int i =0; i < len_a; i++)
    {
        count[ransomNote[i] - 'a']--;
    }

    for(int k = 0; k < 26; k++)
    {
        if(count[k] < 0)
        {
            return false;
        }
    }

    return true;

}
===============================================================================
*/
