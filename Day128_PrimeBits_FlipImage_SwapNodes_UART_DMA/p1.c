#include <stdio.h>
#include <stdbool.h>

/*
 * LeetCode #762 - Prime Number of Set Bits in Binary Representation
 *
 * Description:
 * Given two integers left and right, return the count of numbers in the inclusive
 * range [left, right] having a prime number of set bits in their binary representation.
 *
 * Recall that the number of set bits an integer has is the number of 1's present
 * when written in binary.
 *
 * Constraints:
 * 1 <= left <= right <= 10^6
 * 0 <= right - left <= 10^4
 *
 * Example 1:
 * Input: left = 6, right = 10
 * Output: 4
 * Explanation:
 * 6  -> 110 (2 set bits, 2 is prime)
 * 7  -> 111 (3 set bits, 3 is prime)
 * 8  -> 1000 (1 set bit, 1 is not prime)
 * 9  -> 1001 (2 set bits, 2 is prime)
 * 10 -> 1010 (2 set bits, 2 is prime)
 * 4 numbers have a prime number of set bits.
 *
 * Example 2:
 * Input: left = 10, right = 15
 * Output: 5
 * Explanation:
 * 10 -> 1010 (2 set bits, 2 is prime)
 * 11 -> 1011 (3 set bits, 3 is prime)
 * 12 -> 1100 (2 set bits, 2 is prime)
 * 13 -> 1101 (3 set bits, 3 is prime)
 * 14 -> 1110 (3 set bits, 3 is prime)
 * 15 -> 1111 (4 set bits, 4 is not prime)
 * 5 numbers have a prime number of set bits.
 */

int countPrimeSetBits(int left, int right) {
    int result = 0;
    for(int i = left; i <= right; i++)
    {
        int curr = i;
        int count = 0;
        while(curr > 0)
        {
            curr = curr & (curr - 1);
            count++;
        }
        if(count == 2 || count == 3 || count == 5 || count == 7 || count == 11 || count == 13 || count == 17 || count == 19)
        {
            result++;
        }
    }
    return result;
}

int main() {
    printf("Test 1: %d (Expected: 4)\n", countPrimeSetBits(6, 10));
    printf("Test 2: %d (Expected: 5)\n", countPrimeSetBits(10, 15));
    printf("Test 3: %d (Expected: 23)\n", countPrimeSetBits(842, 888));
    return 0;
}

/*
================================================================================
【原始白板代碼保留與盲點詳細剖析】
================================================================================
int countPrimeSetBits(int left, int right) {
    int result = 0;
    for(int i = left; i <= right; i++)
    {
        int count = 0;
        // ❌ 原始盲點：直接在內部 while 迴圈修改了外層迴圈變數 i！
        // while 結束後 i 變成 0，接著 for 迴圈 i++ 變成 1，
        // 導致 i 永遠在 0 和 1 之間死循環，引發致命無窮迴圈（Time Limit Exceeded）。
        while(i > 0)
        {
            i = i & (i - 1);
            count++;
        }
        if(count == 2 || count == 3 || count == 5 || count == 7 || 
           count == 11 || count == 13 || count == 17 || count == 19)
        {
            result++;
        }
    }
    return result;
}
================================================================================
*/
