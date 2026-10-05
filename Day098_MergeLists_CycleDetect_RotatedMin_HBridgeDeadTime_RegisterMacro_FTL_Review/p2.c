#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Definition for singly-linked list.
 */
struct ListNode {
    int val;
    struct ListNode *next;
};

/* 
 * 題目 2: 環形鏈結串列偵測 (LeetCode #141 - Linked List Cycle)
 * 難度: 🟡 一線 IC 設計廠 (聯詠、瑞昱、聯發科) 經典快慢指針必考題
 * 
 * 題目說明:
 * 給定一個鏈結串列的頭節點 head，判斷鏈結串列中是否有環 (Cycle)。
 * 若串列中有某個節點可以透過連續追蹤 next 指針再次抵達，則表示有環。
 * 若有環回傳 true，否則回傳 false。
 * 
 * 限制條件:
 * - 空間複雜度要求: O(1) 額外空間 (嚴禁使用 Hash Table 或動態陣列存看過的位址)
 * - 時間複雜度要求: O(N)
 */
bool hasCycle(struct ListNode *head) {
    // === 請在此實作你的白板題解答 ===
    if(head == NULL)
    {
        return false;
    }
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while(fast != NULL && fast->next != NULL)
    {
        slow = slow->next;
        fast = fast->next->next;
        if(slow == fast)
        {
            return true;
        }
    }
    return false;
}

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

// 輔助函式: 釋放帶環鏈結串列 (需手動解除環避免無窮迴圈)
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
    printf("=== Day 98 - 白板題 2: 環形鏈結串列偵測 (LeetCode #141) ===\n\n");

    // 測試案例 1: 3 -> 2 -> 0 -> -4 -> (環回 index 1: 2)
    int a1[] = {3, 2, 0, -4};
    struct ListNode* l1 = create_cycle_list(a1, 4, 1);
    bool r1 = hasCycle(l1);
    printf("Test 1: [3, 2, 0, -4], pos = 1 (有環)\n");
    printf("Result:   %s\n", r1 ? "true (有環)" : "false (無環)");
    printf("Expected: true (有環)\n\n");
    free_cycle_list(l1, 4, 1);

    // 測試案例 2: 1 -> 2 -> (環回 index 0: 1)
    int a2[] = {1, 2};
    struct ListNode* l2 = create_cycle_list(a2, 2, 0);
    bool r2 = hasCycle(l2);
    printf("Test 2: [1, 2], pos = 0 (有環)\n");
    printf("Result:   %s\n", r2 ? "true (有環)" : "false (無環)");
    printf("Expected: true (有環)\n\n");
    free_cycle_list(l2, 2, 0);

    // 測試案例 3: 單節點無環 [1]
    int a3[] = {1};
    struct ListNode* l3 = create_cycle_list(a3, 1, -1);
    bool r3 = hasCycle(l3);
    printf("Test 3: [1], pos = -1 (單節點無環)\n");
    printf("Result:   %s\n", r3 ? "true (有環)" : "false (無環)");
    printf("Expected: false (無環)\n\n");
    free_cycle_list(l3, 1, -1);

    // 測試案例 4: 空串列 NULL
    bool r4 = hasCycle(NULL);
    printf("Test 4: head = NULL (空串列)\n");
    printf("Result:   %s\n", r4 ? "true (有環)" : "false (無環)");
    printf("Expected: false (無環)\n");

    return 0;
}
