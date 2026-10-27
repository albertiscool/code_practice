#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * LeetCode #1356 - Sort Integers by The Number of 1 Bits
 *
 * Description:
 * You are given an integer array arr. Sort the integers in the array in ascending
 * order by the number of 1's in their binary representation and in case of two
 * or more integers have the same number of 1's you have to sort them in ascending order.
 *
 * Return the array after sorting it.
 *
 * Constraints:
 * 1 <= arr.length <= 500
 * 0 <= arr[i] <= 10^4
 *
 * Example 1:
 * Input: arr = [0,1,2,3,4,5,6,7,8]
 * Output: [0,1,2,4,8,3,5,6,7]
 *
 * Example 2:
 * Input: arr = [1024,512,256,128,64,32,16,8,4,2,1]
 * Output: [1,2,4,8,16,32,64,128,256,512,1024]
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
static int countBits(int n) {
    int count = 0;
    while (n != 0) {
        n = n & (n - 1);
        count++;
    }
    return count;
}

int cmp(const void *a, const void *b) {
    int val_a = *(const int*)a;
    int val_b = *(const int*)b;
    int count_a = countBits(val_a);
    int count_b = countBits(val_b);

    if (count_a == count_b) {
        return val_a - val_b;
    }
    return count_a - count_b;
}

int* sortByBits(int* arr, int arrSize, int* returnSize) {
    *returnSize = arrSize;
    int* result = (int*) malloc(arrSize * sizeof(int));
    if (!result) return NULL;
    memcpy(result, arr, arrSize * sizeof(int));
    qsort(result, arrSize, sizeof(int), cmp);
    return result;
}

/*
================================================================================
【原始白板代碼保留與盲點詳細剖析】
================================================================================
int cmp(const void *a, const void *b) {
    int count_a = 0;
    int count_b = 0;
    int aa = *(int*)a;
    int bb = *(int*)b;
    while(aa != 0)
    {
        aa = aa & (aa - 1);
        count_a++;
    }
    while(bb != 0)
    {
        bb = bb & (bb - 1);
        count_b++;
    }

    if(count_a == count_b)
    {
        return (aa - bb); // ❌ 盲點 1：aa 與 bb 已經在 while 迴圈被扣減至 0，
                          // 此處 (aa - bb) 永遠為 (0 - 0) = 0，導致無法按原數值大小排序！
    }
    else
    {
        return (count_a - count_b);
    }
}

int* sortByBits(int* arr, int arrSize, int* returnSize) {
    *returnSize = arrSize;
    int* result = (int*) malloc(arrSize * sizeof(int));
    memcpy(result, arr, arrSize); // ❌ 盲點 2：memcpy 第三個參數為 byte 數，
                                  // 傳入 arrSize 僅複製前 1/4 bytes，其餘為 heap 垃圾值！
                                  // 垃圾值引發溢位崩潰 (signed integer overflow)。
    qsort(result, arrSize, sizeof(int), cmp);
    return result;
}
================================================================================
*/

int main() {
    int arr1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8};
    int retSize1 = 0;
    int* res1 = sortByBits(arr1, 9, &retSize1);
    printf("Test 1 Result: ");
    for (int i = 0; i < retSize1; i++) {
        printf("%d ", res1[i]);
    }
    printf("\nExpected:      0 1 2 4 8 3 5 6 7\n");
    free(res1);

    int arr2[] = {1024, 512, 256, 128, 64, 32, 16, 8, 4, 2, 1};
    int retSize2 = 0;
    int* res2 = sortByBits(arr2, 11, &retSize2);
    printf("Test 2 Result: ");
    for (int i = 0; i < retSize2; i++) {
        printf("%d ", res2[i]);
    }
    printf("\nExpected:      1 2 4 8 16 32 64 128 256 512 1024\n");
    free(res2);

    return 0;
}
