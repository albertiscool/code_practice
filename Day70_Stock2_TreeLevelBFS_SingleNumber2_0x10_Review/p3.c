#include <stdio.h>
#include <assert.h>

// ============================================================================
// 題目 3【只出現一次的數字 II (Single Number II - LeetCode #137)】
// 難度：🟡 Medium (中等題) | 出題頻率：🔥🔥🔥🔥🔥 (IC 廠高頻位元神題)
// 演算法：32-Bit 位元統計法 (Bit-Counting Method) - $O(N)$ 時間, $O(1)$ 空間
// ============================================================================
int singleNumber(int* nums, int numsSize)
{
    unsigned int result = 0;

    // 1. 外層迴圈：遍歷 32 個 bit (i 從 0 走到 31)
    for (int i = 0; i < 32; i++)
    {
        int sum = 0;

        // 2. 內層迴圈：統計陣列中「所有數字」的第 i 個 bit 是否為 1
        for (int j = 0; j < numsSize; j++)
        {
            // 將 nums[j] 右移 i 位並取得最低 bit
            sum += (((unsigned int)nums[j] >> i) & 1U);
        }

        // 3. 若該 bit 的 1 總數不能被 3 整除，說明目標數字在第 i bit 必為 1
        if (sum % 3 != 0)
        {
            result |= (1U << i); // 將第 i 個 bit 拼進 result
        }
    }

    return (int)result;
}

int main()
{
    // 測試 1: [2, 2, 3, 2] -> 3
    int nums1[4] = {2, 2, 3, 2};
    int ans1 = singleNumber(nums1, 4);
    printf("測試 1 獨一無二的數字: %d (預期: 3)\n", ans1);
    assert(ans1 == 3);

    // 測試 2: [0, 1, 0, 1, 0, 1, 99] -> 99
    int nums2[7] = {0, 1, 0, 1, 0, 1, 99};
    int ans2 = singleNumber(nums2, 7);
    printf("測試 2 獨一無二的數字: %d (預期: 99)\n", ans2);
    assert(ans2 == 99);

    // 測試 3: [-2, -2, 1, -2] -> 1
    int nums3[4] = {-2, -2, 1, -2};
    int ans3 = singleNumber(nums3, 4);
    printf("測試 3 獨一無二的數字: %d (預期: 1)\n", ans3);
    assert(ans3 == 1);

    printf("\n🎉 p3.c 只出現一次的數字 II 所有測試案例全數 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（32-bit 統計與 result |= (1U << i) 思路 100% 正確！）：
int singleNumber_original(int* nums, int numsSize)
{
    int bit_sum[32] = {0};
    
    // 💡 盲點 1（雙重迴圈維度）：
    // 原版只用了一個 i 從 0 到 31，但陣列裡面有 numsSize 個數字！
    // 必須使用雙重迴圈：外層走 32 個 bit (i)，內層走訪陣列的所有數字 (j 從 0 到 numsSize-1)。
    
    // 💡 盲點 2（C 語言語法）：
    // C 語言中不能直接對整個陣列做 bit_sum = bit_sum % 3;
    // 只要在每次統計完第 i bit 後，直接檢查 if (sum % 3 != 0) 即可！
    
    // 💡 盲點 3（運算子優先級）：
    // if (nums[i] & 1 == 1) 在 C 語言中 == 的優先級高於 &，會被解讀為 nums[i] & (1 == 1)。
    // 寫法應加上括號：((nums[j] >> i) & 1)。
}
*/
