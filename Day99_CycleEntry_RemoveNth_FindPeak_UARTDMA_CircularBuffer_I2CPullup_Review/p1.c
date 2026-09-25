#include <stdio.h>
#include <stdlib.h>

/**
 * Definition for singly-linked list.
 */
struct ListNode {
    int val;
    struct ListNode *next;
};

/* 
 * 題目 1: 環形鏈結串列入口點偵測 (LeetCode #142 - Linked List Cycle II)
 * 難度: 🟡 一線 IC 設計廠 (聯詠、瑞昱、聯發科) 快慢指針終極追問題
 * 
 * 題目說明:
 * 給定一個鏈結串列的頭節點 head，若串列中存在環，請回傳「進入環的第一個節點 (Entrance Node)」。
 * 若串列中不存在環，則回傳 NULL。
 * 
 * 限制條件:
 * - 空間複雜度要求: 嚴格 O(1) 額外空間 (禁止使用 Hash Table 記錄位址)
 * - 時間複雜度要求: O(N)
 * - 核心觀念: 運用昨天推導出的數學關係式 L = n*C - X，在相遇後將一指針放回起點，雙方同步單步前進！
 */
struct ListNode *detectCycle(struct ListNode *head) {
    struct ListNode *slow = head;
    struct ListNode *fast = head;

    // 階段 1: 判斷是否有環
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;

        // 若相遇，代表必定有環！
        if (slow == fast) {
            // 階段 2: 依數學關係式 L = n*C - X，將 slow 放回起點，雙方同步單步前進
            slow = head;
            while (slow != fast) {
                slow = slow->next;
                fast = fast->next;
            }
            return slow; // 再次相遇處即為進入環的第一個節點
        }
    }

    // 若 fast 走到盡頭 (NULL)，代表整條串列完全無環
    return NULL;
}

/*
================================================================================
【原始盲點剖析與使用者程式碼存檔】
================================================================================
使用者原始實作：
struct ListNode *detectCycle(struct ListNode *head) {
    struct ListNode *slow = head;
    struct ListNode *fast = head;
    while(fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast)
        {
            slow = head;
            break;
        }
    }
    while(slow != fast)
    {
        slow = slow->next;
        fast = fast->next;
    }

    return slow;
}

盲點 1：無環情況下的「邏輯穿透 (Fall-Through)」
- 第一個 while 迴圈有兩種退出方式：
  A. 有環：`slow == fast` 觸發 `break` 跳出。
  B. 無環：`fast == NULL` 或 `fast->next == NULL`，迴圈正常結束。
- 原始代碼在無環（情況 B）時，沒有立即回傳 NULL，而是直接往下「穿透」執行第二個 `while (slow != fast)` 迴圈：
  1. 在單節點無環串列 `[1]`：`fast->next == NULL`，第一個迴圈直接沒進去。
     此時 `slow == head`、`fast == head`，條件 `slow != fast` 為假直接跳過，
     最後直接執行 `return slow;` 回傳了節點 1，導致無環卻誤判有環！
  2. 在多節點無環串列 `[1, 2, 3]`：第一個迴圈結束時 `fast` 已經是 NULL，
     第二個迴圈執行 `fast = fast->next` 會直接觸發空指標解引用 (Null Pointer Dereference) 死機！

修正標準：
- 將第二階段尋找入口點的迴圈直接包在 `if (slow == fast)` 區塊內；
  外層迴圈一旦因遇到 NULL 結束，直接回傳 `NULL`。
================================================================================
*/

// 輔助函式: 建立鏈結串列並在指定 pos 處做環 (pos = -1 表示無環)
static struct ListNode* create_cycle_list(const int* arr, int size, int pos) {
    if (size == 0) return NULL;
    struct ListNode** nodes = (struct ListNode**)malloc(sizeof(struct ListNode*) * size);
    for (int i = 0; i < size; i++) {
        nodes[i] = (struct ListNode*)malloc(sizeof(struct ListNode));
        nodes[i]->val = arr[i];
        nodes[i]->next = NULL;
        if (i > 0) {
            nodes[i - 1]->next = nodes[i];
        }
    }
    if (pos >= 0 && pos < size) {
        nodes[size - 1]->next = nodes[pos];
    }
    struct ListNode* head = nodes[0];
    free(nodes);
    return head;
}

// 輔助函式: 釋放帶環鏈結串列
static void free_cycle_list(struct ListNode* head, int size, int pos) {
    if (!head || size == 0) return;
    struct ListNode* curr = head;
    for (int i = 0; i < size; i++) {
        struct ListNode* next = curr->next;
        free(curr);
        curr = next;
    }
}

int main(void) {
    printf("=== Day 99 - 白板題 1: 環形鏈結串列入口點偵測 (LeetCode #142) ===\n\n");

    // 測試案例 1: 3 -> 2 -> 0 -> -4 -> (環回 index 1: 2)
    int a1[] = {3, 2, 0, -4};
    struct ListNode* l1 = create_cycle_list(a1, 4, 1);
    struct ListNode* res1 = detectCycle(l1);
    printf("Test 1: [3, 2, 0, -4], pos = 1\n");
    printf("Result:   Node value = %s\n", res1 ? (res1->val == 2 ? "2 (正確)" : "錯誤數值") : "NULL");
    printf("Expected: Node value = 2\n\n");
    free_cycle_list(l1, 4, 1);

    // 測試案例 2: 1 -> 2 -> (環回 index 0: 1)
    int a2[] = {1, 2};
    struct ListNode* l2 = create_cycle_list(a2, 2, 0);
    struct ListNode* res2 = detectCycle(l2);
    printf("Test 2: [1, 2], pos = 0\n");
    printf("Result:   Node value = %s\n", res2 ? (res2->val == 1 ? "1 (正確)" : "錯誤數值") : "NULL");
    printf("Expected: Node value = 1\n\n");
    free_cycle_list(l2, 2, 0);

    // 測試案例 3: 單節點無環 [1]
    int a3[] = {1};
    struct ListNode* l3 = create_cycle_list(a3, 1, -1);
    struct ListNode* res3 = detectCycle(l3);
    printf("Test 3: [1], pos = -1 (無環)\n");
    printf("Result:   %s\n", res3 == NULL ? "NULL (正確)" : "錯誤有值");
    printf("Expected: NULL\n\n");
    free_cycle_list(l3, 1, -1);

    // 測試案例 4: 空串列
    struct ListNode* res4 = detectCycle(NULL);
    printf("Test 4: head = NULL\n");
    printf("Result:   %s\n", res4 == NULL ? "NULL (正確)" : "錯誤有值");
    printf("Expected: NULL\n\n");

    // 測試案例 5: 多節點無環串列 [1, 2, 3, 4]
    int a5[] = {1, 2, 3, 4};
    struct ListNode* l5 = create_cycle_list(a5, 4, -1);
    struct ListNode* res5 = detectCycle(l5);
    printf("Test 5: [1, 2, 3, 4], pos = -1 (多節點無環)\n");
    printf("Result:   %s\n", res5 == NULL ? "NULL (正確)" : "錯誤有值");
    printf("Expected: NULL\n");
    free_cycle_list(l5, 4, -1);

    return 0;
}
