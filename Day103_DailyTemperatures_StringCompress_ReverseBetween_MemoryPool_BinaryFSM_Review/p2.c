#include <stdio.h>
#include <string.h>

/**
 * ============================================================================
 * 【Day 103 - 題目二】壓縮字串 (LeetCode #443 - String Compression)
 * 【LeetCode 難度】Medium (中等)
 * ============================================================================
 * 【題目說明】
 * 給定一個字元陣列 chars，請使用以下演算法對其進行「原地 (In-place)」壓縮：
 * 
 * 從一個空的字串 s 開始。對於 chars 中每一組「連續重複的字元」：
 * 1. 如果該字元的群組長度為 1，將該字元加入 s。
 * 2. 否則，將該字元加入 s，接著加入表示該群組長度的字元數字。
 * 
 * 壓縮後的字串 s 不應單獨回傳，而是必須「直接修改原本的輸入陣列 chars」存回。
 * 
 * 請回傳修改後 chars 陣列的新長度。
 * 必須使用 O(1) 的額外空間完成。
 * 
 * 【範例 1】
 * 輸入: chars = ["a","a","b","b","c","c","c"]
 * 輸出: 回傳 6，且 chars 前 6 個元素被修改為 ["a","2","b","2","c","3"]
 * 
 * 【範例 2】
 * 輸入: chars = ["a"]
 * 輸出: 回傳 1，且 chars 為 ["a"]
 * 
 * 【範例 3】
 * 輸入: chars = ["a","b","b","b","b","b","b","b","b","b","b","b","b"] (12 個 'b')
 * 輸出: 回傳 4，且 chars 前 4 個元素被修改為 ["a","b","1","2"]
 * 解釋: 長度 12 拆成字元 '1' 和 '2'。
 * 
 * 【限制條件】
 * 1. 1 <= chars.length <= 2000
 * 2. chars[i] 可以是小寫英文字母、大寫英文字母、數字或符號。
 * 3. 空間複雜度必須為 O(1)。
 * ============================================================================
 */

int compress(char* chars, int charsSize) {
    if (chars == NULL || charsSize <= 0) {
        return 0;
    }

    int write = 0; // 寫入指針 (最終回傳的壓縮長度)
    int read = 0;  // 讀取探針

    while (read < charsSize) {
        char curr = chars[read];
        int count = 0;

        // 計算當前連續相同字元的數量 (注意邊界保護 read < charsSize)
        while (read < charsSize && chars[read] == curr) {
            read++;
            count++;
        }

        // 1. 原地寫入當前字元
        chars[write++] = curr;

        // 2. 若重複次數大於 1，寫入連續次數的 ASCII 數字
        if (count > 1) {
            int start = write; // 記錄數字開始寫入的位置

            // 逐位拆解數字寫入 (由低位數到高位數，例如 12 會先寫 '2' 再寫 '1')
            while (count > 0) {
                chars[write++] = (count % 10) + '0'; // 盲點修正：務必加上 '0' 轉為 ASCII
                count /= 10;
            }

            // 原地翻轉剛剛寫入的數字部分 (將 "21" 翻轉為 "12")
            int end = write - 1;
            while (start < end) {
                char temp = chars[start];
                chars[start] = chars[end];
                chars[end] = temp;
                start++;
                end--;
            }
        }
    }

    return write;
}

/* ================= 測試輔助函式 ================= */
static void printChars(char* chars, int len) {
    printf("[");
    for (int i = 0; i < len; i++) {
        printf("\"%c\"%s", chars[i], i < len - 1 ? ", " : "");
    }
    printf("]\n");
}

int main(void) {
    printf("=== Day 103 P2: String Compression 驗證 ===\n");

    // 測試 1: ["a","a","b","b","c","c","c"] -> 6, ["a","2","b","2","c","3"]
    char t1[] = {'a','a','b','b','c','c','c'};
    int len1 = compress(t1, 7);
    printf("測試 1 結果 (長度 %d): ", len1); printChars(t1, len1);
    printf("測試 1 預期 (長度 6): [\"a\", \"2\", \"b\", \"2\", \"c\", \"3\"]\n\n");

    // 測試 2: ["a"] -> 1, ["a"]
    char t2[] = {'a'};
    int len2 = compress(t2, 1);
    printf("測試 2 結果 (長度 %d): ", len2); printChars(t2, len2);
    printf("測試 2 預期 (長度 1): [\"a\"]\n\n");

    // 測試 3: 12 個 'b' -> 4, ["a","b","1","2"]
    char t3[] = {'a','b','b','b','b','b','b','b','b','b','b','b','b'};
    int len3 = compress(t3, 13);
    printf("測試 3 結果 (長度 %d): ", len3); printChars(t3, len3);
    printf("測試 3 預期 (長度 4): [\"a\", \"b\", \"1\", \"2\"]\n\n");

    return 0;
}

/*
 * ============================================================================
 * 🔍 使用者原始白板程式碼存檔 (Original Implementation Archive)
 * ============================================================================
 * 【原始完整程式碼】：
 * int compress(char* chars, int charsSize) {
 *     int count[256] = {0};
 *     int slow = 0;
 *     int head = 0;
 *     int fast = 0;
 *     while(head < charsSize)
 *     {
 *         if(chars[head] == chars[fast])
 *         {
 *             count[chars[head]]++;
 *             fast++;
 *         }
 *         else
 *         {
 *             if(count[chars[head]] - 1 == 0)
 *             {
 *                 slow++;
 *             }
 *             else
 *             {
 *                 if(count[chars[head]] >= 1000)
 *                 {
 *                     char a = (char) (count[chars[head]] / 1000);
 *                     char b = (char) (count[chars[head]] % 1000 / 100);
 *                     char c = (char) (count[chars[head]] % 100 / 10);
 *                     char d = (char) (count[chars[head]] % 10);
 *                     chars[++slow] = a;
 *                     chars[++slow] = b;
 *                     chars[++slow] = c;
 *                     chars[++slow] = d;
 *                 }
 *                 else if(count[chars[head]] >= 100)
 *                 {
 *                     char b = (char) (count[chars[head]] % 1000 / 100);
 *                     char c = (char) (count[chars[head]] % 100 / 10);
 *                     char d = (char) (count[chars[head]] % 10);
 *                     chars[++slow] = b;
 *                     chars[++slow] = c;
 *                     chars[++slow] = d;
 *                 }
 *                 else if(count[chars[head]] >= 10)
 *                 {
 *                     char c = (char) (count[chars[head]] % 100 / 10);
 *                     char d = (char) (count[chars[head]] % 10);
 *                     chars[++slow] = c;
 *                     chars[++slow] = d;
 *                 }
 *                 else
 *                 {
 *                     char d = (char) (count[chars[head]] % 10);
 *                     chars[++slow] = d;
 *                 }
 *             }
 *             count[chars[head]] = 0;
 *             head = fast;
 *         }
 *     }
 *     return slow;
 * }
 * 
 * ============================================================================
 * 【原始白板盲點複盤 (Original Blind Spots)】
 * ============================================================================
 * 盲點 1：ASCII 數值轉字元偏移量遺失 (Char Offset Bug)
 *   原始寫法：
 *     char a = (char) (count[chars[head]] / 1000);
 *     char d = (char) (count[chars[head]] % 10);
 *   問題：
 *     在 C 語言中，整數數值 1 轉型為 char，其值是 ASCII 0x01 (不可見控制字元 SOH)，
 *     並非字元 '1' (ASCII 49)！必須加上 '0' (即 48) 的偏移量：`(count % 10) + '0'`。
 * 
 * 盲點 2：陣列越界存取與結尾區段吞掉 (Buffer Overrun & Missing Tail)
 *   原始寫法：
 *     while (head < charsSize) {
 *         if (chars[head] == chars[fast]) { fast++; }
 *         else { ... }
 *     }
 *   問題：
 *     - fast 沒有邊界保護，當 fast >= charsSize 時依然執行 `chars[head] == chars[fast]`，
 *       讀取越界記憶體導致未定義行為 (UB)。
 *     - 若字串結尾是連續重複字元 (如 ["a", "a"])，fast 一路加到底，永遠進不去 else，
 *       導致最後一組字元完全沒有被結算輸出。
 * 
 * 盲點 3：全域記帳 vs 連續壓縮 (RLE 語義混淆)
 *   原始寫法宣告了 `int count[256] = {0};` 做字元記帳。
 *   但本題是「遊程編碼 (Run-Length Encoding, RLE)」——只壓縮「連續相同」的字元！
 *   若輸入為 ["a", "b", "a"]，使用全域陣列會錯誤記成 2 個 'a'，壓縮成錯誤的結果。
 *   正解為原地「快慢雙指標 (Read / Write)」，空間嚴格 O(1)。
 * ============================================================================
 */
