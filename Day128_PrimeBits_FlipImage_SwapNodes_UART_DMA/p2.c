#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode #832 - Flipping an Image
 *
 * Description:
 * Given an n x n binary matrix image, flip the image horizontally, then invert it,
 * and return the resulting image.
 *
 * To flip an image horizontally means that each row of the image is reversed.
 * For example, flipping [1,1,0] horizontally results in [0,1,1].
 *
 * To invert an image means that each 0 is replaced by 1, and each 1 is replaced by 0.
 * For example, inverting [0,1,1] results in [1,0,0].
 *
 * Constraints:
 * n == image.length
 * n == image[i].length
 * 1 <= n <= 20
 * images[i][j] is either 0 or 1.
 *
 * Example 1:
 * Input: image = [[1,1,0],[1,0,1],[0,0,0]]
 * Output: [[1,0,0],[0,1,0],[1,1,1]]
 * Explanation:
 * First reverse each row: [[0,1,1],[1,0,1],[0,0,0]].
 * Then, invert the image: [[1,0,0],[0,1,0],[1,1,1]].
 *
 * Example 2:
 * Input: image = [[1,1,0,0],[1,0,0,1],[0,1,1,1],[1,0,1,0]]
 * Output: [[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]
 */

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */
int** flipAndInvertImage(int** image, int imageSize, int* imageColSize, int* returnSize, int** returnColumnSizes) {
    for(int i = 0; i < imageSize; i++)
    {
        int left = 0;
        int right = *imageColSize - 1;
        while(left < right)
        {
            int temp = image[i][left];
            image[i][left] = image[i][right];
            image[i][right] = temp;
            left++;
            right--;
        }
        for(int j = 0; j < *imageColSize; j++)
        {
            image[i][j] ^= 1;
        }
    }
    *returnSize = imageSize;
    *returnColumnSizes = imageColSize;
    return image;
}

static void printMatrix(int** mat, int rows, int* cols) {
    printf("[");
    for (int i = 0; i < rows; i++) {
        printf("[");
        for (int j = 0; j < cols[i]; j++) {
            printf("%d%s", mat[i][j], j < cols[i] - 1 ? "," : "");
        }
        printf("]%s", i < rows - 1 ? "," : "");
    }
    printf("]\n");
}

int main() {
    // Test 1: 3x3
    int r0[] = {1, 1, 0};
    int r1[] = {1, 0, 1};
    int r2[] = {0, 0, 0};
    int* img1[] = {r0, r1, r2};
    int colSize1[] = {3, 3, 3};
    int retSize1 = 0;
    int* retCols1 = NULL;
    int** res1 = flipAndInvertImage(img1, 3, colSize1, &retSize1, &retCols1);
    printf("Test 1 Result:   ");
    printMatrix(res1, retSize1, retCols1);
    printf("Expected:        [[1,0,0],[0,1,0],[1,1,1]]\n");

    // Test 2: 4x4
    int q0[] = {1, 1, 0, 0};
    int q1[] = {1, 0, 0, 1};
    int q2[] = {0, 1, 1, 1};
    int q3[] = {1, 0, 1, 0};
    int* img2[] = {q0, q1, q2, q3};
    int colSize2[] = {4, 4, 4, 4};
    int retSize2 = 0;
    int* retCols2 = NULL;
    int** res2 = flipAndInvertImage(img2, 4, colSize2, &retSize2, &retCols2);
    printf("Test 2 Result:   ");
    printMatrix(res2, retSize2, retCols2);
    printf("Expected:        [[1,1,0,0],[0,1,1,0],[0,0,0,1],[1,0,1,0]]\n");

    return 0;
}

/*
================================================================================
【原始白板代碼保留與盲點詳細剖析】
================================================================================
int** flipAndInvertImage(int** image, int imageSize, int* imageColSize, int* returnSize, int** returnColumnSizes) {
    for(int i = 0; i < imageSize; i++)
    {
        int left = 0;
        int right = *imageColSize - 1; // 建議寫 imageColSize[i] - 1 更具備通用性
        while(left < right)
        {
            int temp = image[i][left];
            image[i][left] = image[i][right];
            image[i][right] = temp;
            left++;
            right--;
        }
        for(int j = 0; j < *imageColSize; j++)
        {
            image[i][j] ^= 1;
        }
    }
    *returnSize = imageSize;
    // ❌ 盲點 1：returnColumnSizes 為 int**（二級指標），
    // 此時 *returnColumnSizes 尚未指向任何有效記憶體，直接雙重解引用 ** 會造成 NULL Dereference 崩潰！
    // 正確方式是將放有各列大小的陣列指標填入一級解引用：*returnColumnSizes = imageColSize;
    **returnColumnSizes = *imageColSize; 

    // ❌ 盲點 2：因為題目允許原地修改（In-Place），所以不需要另外 malloc 新陣列，
    // 最末端直接 return image; 即可，達成 0 ms / O(1) 空間極限效能！
}
================================================================================
*/
