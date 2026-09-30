#include <stdio.h>
#include <stdlib.h>

/**
 * ============================================================================
 * 【Day 103 - 題目一】每日溫度 (LeetCode #739 - Daily Temperatures)
 * 【LeetCode 難度】Medium (中等)
 * ============================================================================
 * 【題目說明】
 * 給定一個整數陣列 temperatures 表示每天的溫度。
 * 請回傳一個陣列 answer，其中 answer[i] 是指對於第 i 天，
 * 下一個更高溫度的天數出現在幾天後。如果之後都沒有更高的溫度，請填入 0。
 * 
 * 【範例 1】
 * 輸入: temperatures = [73, 74, 75, 71, 69, 72, 76, 73]
 * 輸出: [1, 1, 4, 2, 1, 1, 0, 0]
 * 
 * 【範例 2】
 * 輸入: temperatures = [30, 40, 50, 60]
 * 輸出: [1, 1, 1, 0]
 * 
 * 【範例 3】
 * 輸入: temperatures = [30, 60, 90]
 * 輸出: [1, 1, 0]
 * 
 * 【限制條件】
 * 1. 1 <= temperatures.length <= 10^5
 * 2. 30 <= temperatures[i] <= 100
 * 3. 請注意時間複雜度，暴力法 O(N^2) 會超時 (TLE)，建議達到 O(N)。
 * ============================================================================
 */

int* dailyTemperatures(int* temperatures, int temperaturesSize, int* returnSize) {
    *returnSize = temperaturesSize;
    if (temperaturesSize == 0) {
        return NULL;
    }

    // 盲點 1 修正：使用 calloc 直接將配置的整座陣列記憶體歸零 (避免 sizeof(pointer) 踩雷)
    int* result = (int*)calloc(temperaturesSize, sizeof(int));
    if (result == NULL) {
        return NULL;
    }

    // 核心資料結構：單調遞減堆疊 (Monotonic Decreasing Stack)
    // 陣列模擬 Stack，裡面儲存天數的「下標 (index)」
    int* stack = (int*)malloc(temperaturesSize * sizeof(int));
    int top = -1; // 堆疊頂端指標，-1 表示空堆疊

    // O(N) 時間複雜度：每個元素最多入棧一次、出棧一次
    for (int i = 0; i < temperaturesSize; i++) {
        // 當堆疊不為空，且當前溫度大於棧頂那一天的溫度時：
        // 代表第 i 天就是棧頂那一天「等待的下一個更高溫度」！
        while (top >= 0 && temperatures[i] > temperatures[stack[top]]) {
            int prevDay = stack[top--];       // 彈出棧頂天數
            result[prevDay] = i - prevDay;   // 計算相隔天數
        }
        // 將當前這一天壓入堆疊，等待未來更熱的天數來喚醒它
        stack[++top] = i;
    }

    // 釋放輔助堆疊空間
    free(stack);
    return result;
}

/* ================= 測試輔助函式 ================= */
static void printArray(int* arr, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d%s", arr[i], i < size - 1 ? ", " : "");
    }
    printf("]\n");
}

int main(void) {
    printf("=== Day 103 P1: Daily Temperatures 驗證 ===\n");

    // 測試 1: [73, 74, 75, 71, 69, 72, 76, 73]
    int t1[] = {73, 74, 75, 71, 69, 72, 76, 73};
    int retSize1 = 0;
    int* res1 = dailyTemperatures(t1, 8, &retSize1);
    printf("測試 1 結果: "); printArray(res1, retSize1);
    printf("測試 1 預期: [1, 1, 4, 2, 1, 1, 0, 0]\n\n");
    free(res1);

    // 測試 2: [30, 40, 50, 60]
    int t2[] = {30, 40, 50, 60};
    int retSize2 = 0;
    int* res2 = dailyTemperatures(t2, 4, &retSize2);
    printf("測試 2 結果: "); printArray(res2, retSize2);
    printf("測試 2 預期: [1, 1, 1, 0]\n\n");
    free(res2);

    // 測試 3: [30, 60, 90]
    int t3[] = {30, 60, 90};
    int retSize3 = 0;
    int* res3 = dailyTemperatures(t3, 3, &retSize3);
    printf("測試 3 結果: "); printArray(res3, retSize3);
    printf("測試 3 預期: [1, 1, 0]\n\n");
    free(res3);

    return 0;
}

/*
 * ============================================================================
 * 🔍 使用者原始白板程式碼存檔 (Original Implementation Archive)
 * ============================================================================
 * 【原始完整程式碼】：
 * int* dailyTemperatures(int* temperatures, int temperaturesSize, int* returnSize) {
 *     int slow = 0;
 *     int fast = 0;
 *     *returnSize = temperaturesSize;
 *     int* result = (int*) malloc(temperaturesSize * sizeof(int));
 *     memset(result, 0, sizeof(result));
 *     while(slow < temperaturesSize && fast < temperaturesSize)
 *     {
 *         fast = slow + 1;
 *         if(temperatures[slow] < temperatures[fast])
 *         {
 *             result[slow] = fast - slow;
 *             slow++;
 *         }
 *         else
 *         {
 *             fast++;
 *         }
 *     }
 *     return result;
 * }
 * 
 * ============================================================================
 * 【原始白板盲點複盤 (Original Blind Spots)】
 * ============================================================================
 * 盲點 1：sizeof(指標) 記憶體歸零未清空 (Fatal Memory Bug)
 *   原始寫法：
 *     int* result = (int*) malloc(temperaturesSize * sizeof(int));
 *     memset(result, 0, sizeof(result));
 *   問題：
 *     result 是一個指標變數 (int*)，在 64-bit 系統上 sizeof(result) 永遠是 8 Bytes！
 *     這導致 memset 永遠只把前 2 個 int 清成 0，後面的數千個整數全是未初始化的隨機垃圾值。
 *   正解：
 *     使用 `calloc(temperaturesSize, sizeof(int))` 一步到位，或
 *     `memset(result, 0, temperaturesSize * sizeof(int))`。
 * 
 * 盲點 2：fast = slow + 1 導致的無窮迴圈 (Infinite Loop)
 *   原始寫法：
 *     while (slow < temperaturesSize && fast < temperaturesSize) {
 *         fast = slow + 1; // 致命錯誤！
 *         if (temperatures[slow] < temperatures[fast]) { slow++; }
 *         else { fast++; }
 *     }
 *   問題：
 *     在 else 分支中 fast++ 往後走，但在下一輪迴圈開頭又被強行執行 `fast = slow + 1` 打回原形！
 *     當 temperatures[slow] >= temperatures[slow+1] 時，fast 永遠在原地踏步，程式陷入死迴圈 (Hang)！
 * 
 * 盲點 3：演算法複雜度陷阱 (O(N^2) vs O(N))
 *   本題 N 最大可達 10^5。若使用雙指針或雙層迴圈枚舉，最差情況時間複雜度高達 O(N^2) = 10^10 次操作，
 *   在 LeetCode 或面試評測中會直接觸發 TLE (Time Limit Exceeded)。
 *   標準正解必須使用「單調堆疊 (Monotonic Stack)」：
 *   堆疊內儲存「還沒等到更熱天氣的天數下標」，維持溫度遞減。
 *   每次遇到新溫度，一口氣將所有比它矮的舊天數全部結算，整體複雜度精確收斂為 O(N)！
 * ============================================================================
 */
