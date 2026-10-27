#include <stdio.h>
#include <stdlib.h>

/*
 * 【群聯面試經典題之三】靜態陣列鏈結串列搜尋 (Phison Q3)
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
 * 1. ListHead 為整條鏈結串列的第一個節點在 ListArray 中的 index。
 * 2. NextPtr 為下一個節點在 ListArray 中的 index；串列結尾節點的 NextPtr 為 NULL_PTR (0xFFFF)。
 * 3. 節點的合成資料值定義為 ((unsigned long long)DataH << 32) | DataL。
 * 4. 關鍵單調性：整條鏈結串列的資料嚴格由小到大遞增（即 NextPtr 所指向之節點數值必定大於前一節點）。
 *
 * 任務需求：
 * 請實作函式 searchListArray(DATA_A, DATA_B, pre_entry)：
 * - 在 ListArray 鏈結串列中搜尋符合 DataH == DATA_A 且 DataL == DATA_B 的節點。
 * - 若找到目標節點：
 *     - 將 *pre_entry 設為前一個節點的 index（若找到的是頭節點 ListHead，則設為 NULL_PTR）。
 *     - 回傳該目標節點的 index (found_entry)。
 * - 若未找到目標節點：
 *     - 回傳 NULL_PTR (0xFFFF)。
 *
 * 效率與安全考量：
 * 1. 善用「遞增有序」之特性，進行最佳化提早終止搜尋。
 * 2. 避免 NextPtr 越界（如超出 0~999）或空串列無效存取。
 */

#define NULL_PTR 0xFFFF

struct ListStruct {
    unsigned int DataH;
    unsigned int DataL;
    unsigned int NextPtr;
};

struct ListStruct ListArray[1000];
unsigned int ListHead = NULL_PTR;

unsigned int searchListArray(unsigned int DATA_A, unsigned int DATA_B, unsigned int *pre_entry) {
    if(ListHead == NULL_PTR) return NULL_PTR;
    struct ListStruct curr = ListArray[ListHead];
    unsigned int curr_idx = ListHead;
    unsigned int pre = NULL_PTR;
    if(curr.DataH == DATA_A && curr.DataL == DATA_B)
    {
        *pre_entry = NULL_PTR;
        return ListHead;
    }
    while(curr.DataH != DATA_A || curr.DataL != DATA_B)
    {
        if(curr.DataH > DATA_A)
        {
            return NULL_PTR;
        }
        else
        {
            if(curr.DataH == DATA_A && curr.DataL > DATA_B)
            {
                return NULL_PTR;
            }
        }

        if(curr.NextPtr >= 0 && curr.NextPtr <= 999)
        {
            pre = curr_idx;
            curr_idx = curr.NextPtr;
            curr = ListArray[curr.NextPtr];
        }
        else
        {
            return NULL_PTR;
        }
    }

    *pre_entry = pre;
    return curr_idx;


}

int main() {
    // 建立一條有序鏈結串列: Node 3 -> Node 1 -> Node 7 -> NULL
    // Node 3 (index 3): (DataH=1, DataL=10)
    // Node 1 (index 1): (DataH=1, DataL=20)
    // Node 7 (index 7): (DataH=2, DataL=5)
    ListHead = 3;
    ListArray[3].DataH = 1; ListArray[3].DataL = 10; ListArray[3].NextPtr = 1;
    ListArray[1].DataH = 1; ListArray[1].DataL = 20; ListArray[1].NextPtr = 7;
    ListArray[7].DataH = 2; ListArray[7].DataL = 5;  ListArray[7].NextPtr = NULL_PTR;

    unsigned int pre = 0;
    unsigned int found = 0;

    // Test 1: 尋找頭節點 (1, 10)
    printf("=== Test 1: Search Head (1, 10) ===\n");
    found = searchListArray(1, 10, &pre);
    if (found != NULL_PTR) {
        printf("Found Entry: %u, Pre Entry: 0x%X\n", found, pre);
        printf("Expected:    Found Entry: 3, Pre Entry: 0xFFFF\n");
    } else {
        printf("not found.\n");
    }

    // Test 2: 尋找中間節點 (1, 20)
    printf("\n=== Test 2: Search Middle (1, 20) ===\n");
    found = searchListArray(1, 20, &pre);
    if (found != NULL_PTR) {
        printf("Found Entry: %u, Pre Entry: %u\n", found, pre);
        printf("Expected:    Found Entry: 1, Pre Entry: 3\n");
    } else {
        printf("not found.\n");
    }

    // Test 3: 尋找不存在之較大數值 (2, 100) -> 應提早中斷並回傳 not found
    printf("\n=== Test 3: Search Non-existing (2, 100) ===\n");
    found = searchListArray(2, 100, &pre);
    if (found != NULL_PTR) {
        printf("Found Entry: %u, Pre Entry: %u\n", found, pre);
    } else {
        printf("not found.\n");
        printf("Expected:    not found.\n");
    }

    return 0;
}
