#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * 【群聯面試 Q3 續集】靜態陣列鏈結串列有序插入 (Phison Q3 - Sorted Insert)
 *
 * 題目背景與情境：
 * 群聯嵌入式韌體常利用預先分配的陣列建立靜態鏈結串列（Static Linked List in Array）。
 * 給定結構體與全域變數定義如下：
 *
 * #define NULL_PTR 0xFFFF
 *
 * struct ListStruct {
 *     unsigned int DataH;
 *     unsigned int DataL;
 *     unsigned int NextPtr;
 * };
 *
 * struct ListStruct ListArray[1000];
 * unsigned int ListHead;
 *
 * 鏈結串列特性：
 * 1. ListHead 為整條鏈結串列的第一個節點在 ListArray 中的 index。若為空串列則 ListHead == NULL_PTR。
 * 2. NextPtr 為下一個節點在 ListArray 中的 index；串列結尾節點的 NextPtr 為 NULL_PTR (0xFFFF)。
 * 3. 節點的合成資料值定義為 ((unsigned long long)DataH << 32) | DataL。
 * 4. 關鍵單調性：整條鏈結串列的資料嚴格由小到大遞增。
 *
 * 任務需求：
 * 請實作函式 insertListEntry(DATA_A, DATA_B)：
 * - 將新節點 (DATA_A, DATA_B) 插入到 ListArray 鏈結串列中。
 * - 插入後，整條鏈結串列必須依然維持由小到大嚴格遞增的順序。
 * - 插入成功時，回傳該新節點在 ListArray 中的 index。
 * - 若發生以下情況，拒絕插入並回傳 -1：
 *     1. 串列已滿（無閒置空間可用）。
 *     2. 串列中已存在相同數值 (DataH == DATA_A && DataL == DATA_B) 的節點。
 */

#define NULL_PTR 0xFFFF

struct ListStruct {
    unsigned int DataH;
    unsigned int DataL;
    unsigned int NextPtr;
};

struct ListStruct ListArray[1000];
unsigned int ListHead = NULL_PTR;
static bool isUsed[1000] = {false};

// 輔助函式：尋找 ListArray 中未被使用的 entry index，若已滿則回傳 -1
int allocateEntry(void) {
    for (int i = 0; i < 1000; i++) {
        if (!isUsed[i]) {
            isUsed[i] = true;
            return i;
        }
    }
    return -1;
}

/*
 * =============================================================================
 * 【版本一：使用者思路訂正版（保持 if(ListHead==NULL_PTR) 分支與 temp 交換邏輯）】
 * =============================================================================
 */
int insertListEntry(unsigned int DATA_A, unsigned int DATA_B) {
    // 1. 空串列情況（沿用您的處理）
    if (ListHead == NULL_PTR)
    {
        int new_idx = allocateEntry();
        if (new_idx == -1) return -1;
        ListHead = new_idx;
        ListArray[ListHead].DataH = DATA_A;
        ListArray[ListHead].DataL = DATA_B;
        ListArray[ListHead].NextPtr = NULL_PTR;
        return ListHead;
    }
    // 2. 非空串列插入
    else
    {
        struct ListStruct curr;
        unsigned int pre = NULL_PTR;
        unsigned int now = ListHead;

        // 【訂正 1】：比對當前節點，若已大於等於目標則中斷
        while (now != NULL_PTR && now < 1000)
        {
            curr = ListArray[now];
            if (curr.DataH > DATA_A || (curr.DataH == DATA_A && curr.DataL >= DATA_B))
            {
                break;
            }
            pre = now;
            now = curr.NextPtr;
        }

        // 【訂正 2】：重複數值檢查（需先確認 now != NULL_PTR）
        if (now != NULL_PTR && DATA_A == curr.DataH && DATA_B == curr.DataL)
        {
            return -1;
        }

        // 【訂正 3】：改用 new_entry 變數，避免覆蓋記錄著下一個節點 index 的 now
        int new_entry = allocateEntry();
        if (new_entry == -1) return -1;

        // 沿用您的頭插法
        if (pre == NULL_PTR)
        {
            unsigned int temp = ListHead;
            ListHead = new_entry;
            ListArray[new_entry].DataH = DATA_A;
            ListArray[new_entry].DataL = DATA_B;
            ListArray[new_entry].NextPtr = temp;
            return new_entry;
        }
        // 沿用您的中間與尾部插入法
        else
        {
            unsigned int temp = ListArray[pre].NextPtr;
            ListArray[pre].NextPtr = new_entry;
            ListArray[new_entry].DataH = DATA_A;
            ListArray[new_entry].DataL = DATA_B;
            ListArray[new_entry].NextPtr = temp;
            return new_entry;
        }
    }
}

/*
 * =============================================================================
 * 【版本二：極簡標準參考解法（Unified Loop，不分空串列特例，統一指針串接）】
 * =============================================================================
int insertListEntry_Optimal(unsigned int DATA_A, unsigned int DATA_B) {
    unsigned int pre = NULL_PTR;
    unsigned int now = ListHead;

    while (now != NULL_PTR && now < 1000) {
        struct ListStruct *curr = &ListArray[now];
        if (curr->DataH == DATA_A && curr->DataL == DATA_B) return -1; // 重複
        if (curr->DataH > DATA_A || (curr->DataH == DATA_A && curr->DataL > DATA_B)) break;
        pre = now;
        now = curr->NextPtr;
    }

    int new_idx = allocateEntry();
    if (new_idx == -1) return -1;

    ListArray[new_idx].DataH = DATA_A;
    ListArray[new_idx].DataL = DATA_B;
    ListArray[new_idx].NextPtr = now;

    if (pre == NULL_PTR) ListHead = new_idx;
    else ListArray[pre].NextPtr = new_idx;

    return new_idx;
}
 * =============================================================================
 */

/*
================================================================================
【原始白板代碼保留與盲點詳細剖析】
================================================================================
int insertListEntry_Original(unsigned int DATA_A, unsigned int DATA_B) {
    if(ListHead == NULL_PTR)
    {
        ListHead = allocateEntry();
        ListArray[ListHead].DataH = DATA_A;
        ListArray[ListHead].DataL = DATA_B;
        ListArray[ListHead].NextPtr = NULL_PTR;
        return ListHead;
    }
    else
    {
        struct ListStruct curr = ListArray[ListHead];
        unsigned int pre = NULL_PTR;
        unsigned int now = ListHead;
        
        // ❌ 盲點 1（語法錯誤）：curr 為結構體 (struct ListStruct)，
        // 不能直接與整數 NULL_PTR (0xFFFF) 比對 !=。
        while(curr != NULL_PTR && now != NULL_PTR && DATA_A > curr.DataH)
        {
            pre = now;
            now = curr.NextPtr;
            curr = ListArray[curr.NextPtr];
        }
        
        // ❌ 盲點 2（演算法致命錯誤）：拆成兩個 while 迴圈比大小會破壞有序性！
        // 若當前節點 DataH 已大於目標，但在第二個迴圈中 DataB > curr.DataL 成立，
        // 指標會錯誤地繼續往後衝，導致新節點被插在比它更大的節點後面！
        while(curr != NULL_PTR && now != NULL_PTR && DATA_B > curr.DataL)
        {
            pre = now;
            now = curr.NextPtr;
            curr = ListArray[curr.NextPtr];
        }
        if(DATA_A == curr.DataH && DATA_B == curr.DataL) return -1;
        now = allocateEntry(); // ❌ 盲點 3：now 本來記錄著下一個節點，被 allocateEntry() 覆蓋了
        if(now == -1) return -1;
        if(pre == NULL_PTR)
        {
            unsigned int temp = ListHead;
            ListHead = now;
            ListArray[now].DataH = DATA_A;
            ListArray[now].DataL = DATA_B;
            ListArray[now].NextPtr = temp;
            return now;
        }
        if(now != -1)
        {
            unsigned int temp = ListArray[pre].NextPtr;
            ListArray[pre].NextPtr = now;
            ListArray[now].DataH = DATA_A;
            ListArray[now].DataL = DATA_B;
            ListArray[now].NextPtr = temp;
            return now;
        }
    }
}
================================================================================
*/

static void printEntireList(void) {
    unsigned int curr = ListHead;
    printf("List Content: ");
    if (curr == NULL_PTR) {
        printf("[Empty List]\n");
        return;
    }
    while (curr != NULL_PTR && curr < 1000) {
        printf("[%u: (%u, %u)] -> ", curr, ListArray[curr].DataH, ListArray[curr].DataL);
        curr = ListArray[curr].NextPtr;
    }
    printf("NULL\n");
}

int main() {
    printf("=== Test 1: Insert into empty list (1, 20) ===\n");
    int idx1 = insertListEntry(1, 20);
    printf("Inserted at index: %d\n", idx1);
    printEntireList();

    printf("\n=== Test 2: Insert at Head (smaller value: (1, 10)) ===\n");
    int idx2 = insertListEntry(1, 10);
    printf("Inserted at index: %d\n", idx2);
    printEntireList();

    printf("\n=== Test 3: Insert at Tail (largest value: (2, 5)) ===\n");
    int idx3 = insertListEntry(2, 5);
    printf("Inserted at index: %d\n", idx3);
    printEntireList();

    printf("\n=== Test 4: Insert in Middle (1, 15) ===\n");
    int idx4 = insertListEntry(1, 15);
    printf("Inserted at index: %d\n", idx4);
    printEntireList();

    printf("\n=== Test 5: Insert Duplicate (1, 15) -> Should Fail ===\n");
    int idx5 = insertListEntry(1, 15);
    printf("Inserted at index: %d (Expected: -1)\n", idx5);
    printEntireList();

    return 0;
}
