#include <stdio.h>
#include <stdlib.h>

/* 
 * 題目 3: 兩數之和 II - 輸入有序陣列 (LeetCode #167 - Two Sum II - Input Array Is Sorted)
 * 難度: 🟡 一線 IC 設計廠 (聯發科、聯詠、群聯) 雙指針邊界夾擊必考題
 * 
 * 題目說明:
 * 給定一個已按照「非遞減順序 (升序)」排列的整數陣列 numbers，請你從陣列中找出滿足相加之和等於目標數 target 的兩個數。
 * 函式應以長度為 2 的整數陣列形式回傳這兩個數的「1-indexed 索引值」[index1, index2]，其中 1 <= index1 < index2 <= numbers.length。
 * 假設每個輸入剛好只有一個有效答案，且不可重複使用同一個元素。
 * 
 * 限制條件:
 * - 時間複雜度要求: O(N)
 * - 空間複雜度要求: O(1) 額外空間 (嚴禁使用 Hash Table)
 * 
 * 範例說明:
 * - 範例 1: numbers = [2, 7, 11, 15], target = 9
 *   輸出: [1, 2]
 *   解釋: numbers[0] + numbers[1] = 2 + 7 = 9。轉為 1-based index 即為 [1, 2]。
 * 
 * - 範例 2: numbers = [2, 3, 4], target = 6
 *   輸出: [1, 3]
 *   解釋: numbers[0] + numbers[2] = 2 + 4 = 6。轉為 1-based index 即為 [1, 3]。
 * 
 * - 範例 3: numbers = [-1, 0], target = -1
 *   輸出: [1, 2]
 *   解釋: numbers[0] + numbers[1] = -1 + 0 = -1。轉為 1-based index 即為 [1, 2]。
 */
int* twoSum(int* numbers, int numbersSize, int target, int* returnSize) {
    // === 請在此實作你的白板題解答 ===
    int left = 0;
    int right = numbersSize - 1;
    while(numbers[left] + numbers[right] != target)
    {
        if(numbers[left] + numbers[right] < target)
        {
            left++;
        }
        else
        {
            right--;
        }
    }
    *returnSize = 2;
    int *result = (int*) malloc(2*sizeof(int));
    result[0] = left+1;
    result[1] = right+1;
    return result;
}

int main(void) {
    printf("=== Day 100 - 白板題 3: 兩數之和 II - 輸入有序陣列 (LeetCode #167) ===\n\n");

    // 測試案例 1
    int t1[] = {2, 7, 11, 15};
    int retSize1 = 0;
    int* res1 = twoSum(t1, 4, 9, &retSize1);
    printf("Test 1: [2, 7, 11, 15], target = 9\n");
    if (res1 && retSize1 == 2) {
        printf("Result:   [%d, %d]\n", res1[0], res1[1]);
        free(res1);
    } else {
        printf("Result:   NULL\n");
    }
    printf("Expected: [1, 2]\n\n");

    // 測試案例 2
    int t2[] = {2, 3, 4};
    int retSize2 = 0;
    int* res2 = twoSum(t2, 3, 6, &retSize2);
    printf("Test 2: [2, 3, 4], target = 6\n");
    if (res2 && retSize2 == 2) {
        printf("Result:   [%d, %d]\n", res2[0], res2[1]);
        free(res2);
    } else {
        printf("Result:   NULL\n");
    }
    printf("Expected: [1, 3]\n\n");

    // 測試案例 3: 包含負數
    int t3[] = {-1, 0};
    int retSize3 = 0;
    int* res3 = twoSum(t3, 2, -1, &retSize3);
    printf("Test 3: [-1, 0], target = -1\n");
    if (res3 && retSize3 == 2) {
        printf("Result:   [%d, %d]\n", res3[0], res3[1]);
        free(res3);
    } else {
        printf("Result:   NULL\n");
    }
    printf("Expected: [1, 2]\n");

    return 0;
}
