#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 2【最大子陣列和 (Maximum Subarray - LeetCode #53)】
// 演算法：Kadane 演算法 (貪心/動態規劃) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================
int maxSubArray(int* nums, int numsSize)
{
    // 邊界防禦
    if (numsSize == 0) return 0;

    int max_sum = nums[0]; // 記錄迄今為止的歷史最大和 (預設為第一個元素，可支援全負數陣列)
    int current_sum = 0;   // 記錄當前連續子陣列的累積和

    for (int i = 0; i < numsSize; i++)
    {
        current_sum += nums[i]; // 將當前數字加入累積和

        // 1. 如果當前累積和打破歷史紀錄，更新最大和
        if (current_sum > max_sum)
        {
            max_sum = current_sum;
        }

        // 2. 核心貪心思維（丟棄負包袱）：
        // 如果累積和跌落成負數 (current_sum < 0)，
        // 帶著負數往後加只會拖累後面的數字，所以立刻果斷歸零，從下一個數字重新開始計算！
        if (current_sum < 0)
        {
            current_sum = 0;
        }
    }

    return max_sum;
}

int main()
{
    // 測試 1: [-2, 1, -3, 4, -1, 2, 1, -5, 4] -> 預期: 6 ([4, -1, 2, 1])
    int nums1[9] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
    int ans1 = maxSubArray(nums1, 9);
    printf("測試 1 最大和: %d (預期: 6)\n", ans1);
    assert(ans1 == 6);

    // 測試 2: [1] -> 預期: 1
    int nums2[1] = {1};
    int ans2 = maxSubArray(nums2, 1);
    printf("測試 2 最大和: %d (預期: 1)\n", ans2);
    assert(ans2 == 1);

    // 測試 3: 全負數 [-5, -2, -8, -1] -> 預期: -1 (取最大的單個負數)
    int nums3[4] = {-5, -2, -8, -1};
    int ans3 = maxSubArray(nums3, 4);
    printf("測試 3 最大和: %d (預期: -1)\n", ans3);
    assert(ans3 == -1);

    printf("\n🎉 p2.c 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討：
int maxSubArray_original(int* nums, int numsSize)
{
    if(numsSize == 0) return 0;

    int sum = 0;
    int max = nums[0];
    for(int fast = 0 ; fast < numsSize ; fast++)
    {
        if(sum + nums[fast] >= max)
        {
            max = sum + nums[fast];
        }
        else
        {
            sum = 0;
        }
        // 💡 盲點 1：return 放在迴圈內部，導致第一輪 fast=0 跑完就直接提前結束了！
        return max; 
    }
    
    // 💡 盲點 2（何時歸零？）：
    // 當前累積和 sum 必須在「小於 0」時才需要歸零（丟掉負包袱），
    // 不能只因為沒有大於 max 就把 sum 歸零，因為例如 [4, -1, 2]，-1 雖然讓和變小，但總和 3 依然是正數，對後面的 2 還有正貢獻！
    return 0;
}
*/
