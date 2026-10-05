#include <stdio.h>
#include <assert.h>

// ============================================================================
// 寫法 1：標準陣列索引法 (最直覺滿分寫法)
// ============================================================================
int singleNumber(int* nums, int numsSize)
{
    int result = 0;

    // 依序將陣列中的每一個數字與 result 做 XOR 運算
    for (int i = 0; i < numsSize; i++)
    {
        result ^= nums[i]; // 出現兩次的數字會成對抵消 (x ^ x = 0)
    }

    return result; // 最終留下來的就是唯一落單的數字！
}

// ============================================================================
// 寫法 2：指標走訪法 (Pointer Walking - 順著你的原版指標思路)
// ============================================================================
int singleNumber_pointer(int* nums, int numsSize)
{
    int result = 0;

    while (numsSize > 0)
    {
        result ^= *nums; // 取出指標指向的數值 (*nums) 進行 XOR
        nums++;          // 指標往後移一格
        numsSize--;
    }

    return result;
}

// ============================================================================
// 測試主程式
// ============================================================================
int main()
{
    // 測試 1：[2, 2, 1] -> 唯一落單值為 1
    int arr1[] = {2, 2, 1};
    int ans1 = singleNumber(arr1, 3);
    printf("測試 1 -> 找到單一數值: %d (預期: 1)\n", ans1);
    assert(ans1 == 1);

    // 測試 2：[4, 1, 2, 1, 2] -> 唯一落單值為 4
    int arr2[] = {4, 1, 2, 1, 2};
    int ans2 = singleNumber(arr2, 5);
    printf("測試 2 -> 找到單一數值: %d (預期: 4)\n", ans2);
    assert(ans2 == 4);

    // 測試 3：[1] -> 唯一值為 1
    int arr3[] = {1};
    int ans3 = singleNumber(arr3, 1);
    printf("測試 3 -> 找到單一數值: %d (預期: 1)\n", ans3);
    assert(ans3 == 1);

    printf("\n🎉 所有測試案例全部 100%% 通過！\n");
    return 0;
}

/*
// ⚠️ 你的原始寫法對照與檢討（XOR 抵消核心概念 100% 正確！）：
int singleNumber_original(int* nums, int numsSize)
{
    int result = 0;

    // 💡 盲點 1：陣列不是鏈結串列，結尾不會是 NULL！
    // 必須使用 numsSize 來控制迴圈次數 (for 迴圈 或 while (numsSize--))，
    // 否則 nums 會一直往後狂走導致記憶體越界當機！
    while(nums != NULL)
    {
        // 💡 盲點 2：&nums 是「指標變數自己的地址」，要取陣列裡面的數值要用 *nums 或 nums[i]！
        result = result ^ &nums;
        nums++;
    }
    return result;
}
*/