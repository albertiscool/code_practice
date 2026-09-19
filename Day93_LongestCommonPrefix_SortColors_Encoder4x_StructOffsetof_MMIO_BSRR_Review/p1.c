#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ============================================================================
// 題目 1【最長公共前綴 (Longest Common Prefix - LeetCode #14)】
// 難度：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠/IC設計廠白板字串高頻基本題)
// 題目說明：編寫一個函式來尋找字串陣列中的最長公共前綴。
//           如果不存在公共前綴，返回空字串 ""。
// 範例 1：
//    輸入：strs = ["flower","flow","flight"]
//    輸出："fl"
// 範例 2：
//    輸入：strs = ["dog","racecar","car"]
//    輸出：""
//    解釋：輸入不存在公共前綴。
// 限制條件：
//    - 1 <= strs.length <= 200
//    - 0 <= strs[i].length <= 200
//    - strs[i] 僅由小寫英文字母組成。
//    - 回傳值需為動態配置之字串 (可由呼叫者 free)。
// ============================================================================

char* longestCommonPrefix(char** strs, int strsSize) {
    /*
     * 正確解答 (縱向掃描法 Vertical Scanning - 時間 O(S)、額外空間 O(1)):
     * 1. 以第一個字串 strs[0] 的每個字元位置 i 為基準。
     * 2. 依序檢查其他字串 strs[j] 在位置 i 的字元：
     *    - 若 strs[j] 已經到達結尾 (strs[j][i] == '\0')，或字元不符合 (strs[j][i] != c)，
     *      則代表公共前綴在長度 i 結束。
     *    - 動態配置 malloc(i + 1) 並填入前綴，務必補上 '\0' 字串結尾符！
     * 3. 若整個 strs[0] 順利比對完畢，則 strs[0] 本身即為最長公共前綴。
     */
    if (strs == NULL || strsSize == 0) {
        char* empty = (char*)malloc(1);
        empty[0] = '\0';
        return empty;
    }

    int len0 = strlen(strs[0]);
    for (int i = 0; i < len0; i++) {
        char c = strs[0][i];
        for (int j = 1; j < strsSize; j++) {
            if (strs[j][i] == '\0' || strs[j][i] != c) {
                char* result = (char*)malloc(i + 1);
                strncpy(result, strs[0], i);
                result[i] = '\0'; // 務必手動補上字串結尾符 '\0'
                return result;
            }
        }
    }

    // 若全部通過迴圈，代表 strs[0] 本身就是最長公共前綴
    char* result = (char*)malloc(len0 + 1);
    strcpy(result, strs[0]);
    return result;
}

/* ============================================================================
 * ❌ 原始實作盲點分析 (已保留供複習檢討):
 * ============================================================================
 * char* longestCommonPrefix_wrong(char** strs, int strsSize) {
 *     if(strsSize == 0) return "";
 * 
 *     // ⚠️ 盲點 1: 回傳值規定為動態配置 (需能被呼叫者 free)
 *     //           直接 return strs[0] (字串常值/靜態位址) 會在 main 呼叫 free() 時引發崩潰！
 *     if(strsSize == 1) return strs[0];
 * 
 *     int len0 = strlen(strs[0]);
 *     for(int i = 0; i < len0; i++)
 *     {
 *         char s = strs[0][i];
 *         for(int j = 0; j < strsSize; j++)
 *         {
 *             if(strs[j][i] != s)
 *             {
 *                 // ⚠️ 盲點 2 (編譯錯誤): malloc 只有一個參數，寫成 malloc(i+1, sizeof(char)) 語法錯誤
 *                 char* result = (char*) malloc(i+1,sizeof(char));
 *                 for(int z = 0; z < i; z++)
 *                 {
 *                     result[z] = strs[0][z];
 *                 }
 *                 // ⚠️ 盲點 3 (C 字串致命漏洞): 沒有補上 '\0'！
 *                 //           C 語言字串結尾必須有 '\0'，否則 printf 會讀取未定義記憶體直到亂碼或崩潰！
 *                 //           應補上: result[i] = '\0';
 *                 return result;
 *             }
 *         }
 *     }
 *     // ⚠️ 盲點 4: 若整個 strs[0] 都是公共前綴 (例如 ["ab", "ab"])，迴圈跑完應該回傳 strs[0]，
 *     //           而不是回傳空字串 ""！
 *     return "";
 * }
 * ============================================================================ */

int main(void) {
    /* 測試測資 1: ["flower","flow","flight"] -> "fl" */
    char* strs1[] = {"flower", "flow", "flight"};
    char* ans1 = longestCommonPrefix(strs1, 3);
    printf("測資 1 結果: \"%s\" (預期: \"fl\")\n", ans1);
    if (ans1 && strlen(ans1) > 0) free(ans1);

    /* 測試測資 2: ["dog","racecar","car"] -> "" */
    char* strs2[] = {"dog", "racecar", "car"};
    char* ans2 = longestCommonPrefix(strs2, 3);
    printf("測資 2 結果: \"%s\" (預期: \"\")\n", ans2);
    if (ans2 && strlen(ans2) > 0) free(ans2);

    /* 測試測資 3: 單一字串 ["a"] -> "a" */
    char* strs3[] = {"a"};
    char* ans3 = longestCommonPrefix(strs3, 1);
    printf("測資 3 結果: \"%s\" (預期: \"a\")\n", ans3);
    if (ans3 && strlen(ans3) > 0) free(ans3);

    return 0;
}
