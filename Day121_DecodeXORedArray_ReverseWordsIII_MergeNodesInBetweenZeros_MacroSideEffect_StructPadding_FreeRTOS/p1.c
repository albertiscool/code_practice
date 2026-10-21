#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #1720 - Decode XORed Array
 * 
 * 題目描述：
 * 未知的整數陣列 arr 包含 n 個非負整數。
 * 我們將 arr 編碼成另一個長度為 n - 1 的整數陣列 encoded，
 * 其中 encoded[i] = arr[i] ^ arr[i + 1]（^ 為位元 XOR 運算）。
 * 例如：若 arr = [1, 0, 2, 1]，則 encoded = [1^0, 0^2, 2^1] = [1, 2, 3]。
 * 
 * 給定編碼後的陣列 encoded 以及原始陣列的第一個元素 first（即 arr[0] = first）。
 * 請解碼並回傳原始的整數陣列 arr。
 * 
 * 範例 1：
 * 輸入：encoded = [1, 2, 3], first = 1
 * 輸出：[1, 0, 2, 1]
 * 解釋：arr[0] = 1, arr[1] = 1^1 = 0, arr[2] = 0^2 = 2, arr[3] = 2^3 = 1
 * 
 * 範例 2：
 * 輸入：encoded = [6, 2, 7, 3], first = 4
 * 輸出：[4, 2, 0, 7, 4]
 * 
 * 限制條件：
 * 2 <= n <= 10^4
 * encoded.length == n - 1
 * 0 <= encoded[i] <= 10^5
 * 0 <= first <= 10^5
 */

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本 - 一次 AC / 0 ms)】
 * -------------------------------------------------------------------------
 * int* decode(int* encoded, int encodedSize, int first, int* returnSize) {
 *     *returnSize = encodedSize + 1;
 *     int* arr = (int*) malloc(*returnSize * sizeof(int));
 *     arr[0] = first;
 *     arr[1] = first ^ encoded[0];
 *     for(int i = 1; i < encodedSize; i++)
 *     {
 *         arr[i+1] = arr[i] ^ encoded[i];
 *     }
 *     
 *     return arr;
 * }
 * -------------------------------------------------------------------------
 * 【設計亮點剖析】：
 * 1. 異或代數逆運算自反性：
 *    - 若 encoded[i] = arr[i] ^ arr[i+1]，兩邊同時 XOR arr[i]：
 *      arr[i] ^ encoded[i] = arr[i] ^ arr[i] ^ arr[i+1] = 0 ^ arr[i+1] = arr[i+1]。
 *    - 遞迴關係式：arr[i+1] = arr[i] ^ encoded[i]。
 * 2. 邊界與記憶體安全：
 *    - 陣列大小精準配置為 encodedSize + 1，並正確賦值 *returnSize。
 * 3. 程式碼微調小技巧：
 *    - 迴圈可直接從 i = 0 開始（因為 i=0 時 arr[1] = arr[0] ^ encoded[0] = first ^ encoded[0]），
 *      直接省去第 40 行的單獨賦值！
 * 4. 遠端實測：76/76 全測資 Accepted，執行時間 0 ms (Beats 100%)！
 * =========================================================================
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* decode(int* encoded, int encodedSize, int first, int* returnSize) {
    *returnSize = encodedSize + 1;
    int* arr = (int*)malloc(*returnSize * sizeof(int));
    arr[0] = first;
    
    // 從 i = 0 開始一路異或推進
    for (int i = 0; i < encodedSize; i++) {
        arr[i + 1] = arr[i] ^ encoded[i];
    }
    
    return arr;
}

int main() {
    int enc1[] = {1, 2, 3};
    int size1 = 0;
    int* res1 = decode(enc1, 3, 1, &size1);
    printf("Test 1: [");
    for (int i = 0; i < size1; i++) {
        printf("%d%s", res1[i], (i == size1 - 1) ? "" : ", ");
    }
    printf("] (Expected: [1, 0, 2, 1])\n");
    free(res1);

    int enc2[] = {6, 2, 7, 3};
    int size2 = 0;
    int* res2 = decode(enc2, 4, 4, &size2);
    printf("Test 2: [");
    for (int i = 0; i < size2; i++) {
        printf("%d%s", res2[i], (i == size2 - 1) ? "" : ", ");
    }
    printf("] (Expected: [4, 2, 0, 7, 4])\n");
    free(res2);

    return 0;
}
