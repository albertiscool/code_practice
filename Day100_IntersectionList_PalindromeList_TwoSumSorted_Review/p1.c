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
 * 題目 1: 相交鏈結串列 (LeetCode #160 - Intersection of Two Linked Lists)
 * 難度: 🟡 一線 IC 設計廠 (聯詠、瑞昱、聯發科) 雙指標經典高頻題
 * 
 * 題目說明:
 * 給定兩個單向鏈結串列的頭節點 headA 和 headB，請找出並回傳兩個單向鏈結串列相交的起始節點。
 * 如果兩個鏈結串列完全沒有重疊交點，則回傳 NULL。
 * 
 * 限制條件:
 * - 時間複雜度要求: O(M + N)
 * - 空間複雜度要求: 嚴格 O(1) 額外空間 (嚴禁使用 Hash Table 或修改原本節點的結構與數值)
 */
struct ListNode *getIntersectionNode(struct ListNode *headA, struct ListNode *headB) {
    // === 請在此實作你的白板題解答 ===
    if(headA == NULL || headB == NULL)
    {
        return NULL;
    }
    
    struct ListNode* currA = headA;
    struct ListNode* currB = headB;

    while(currA != currB)
    {
        currA = (currA == NULL) ? headB : currA->next;
        currB = (currB == NULL) ? headA : currB->next;
    }

    return currA;
}

// 輔助函式: 建立鏈結串列
static struct ListNode* create_node(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

int main(void) {
    printf("=== Day 100 - 白板題 1: 相交鏈結串列 (LeetCode #160) ===\n\n");

    // 測試案例 1: 有交點
    // Common: 8 -> 4 -> 5
    // ListA: 4 -> 1 -> (8 -> 4 -> 5)
    // ListB: 5 -> 6 -> 1 -> (8 -> 4 -> 5)
    struct ListNode* c1 = create_node(8);
    c1->next = create_node(4);
    c1->next->next = create_node(5);

    struct ListNode* a1 = create_node(4);
    a1->next = create_node(1);
    a1->next->next = c1;

    struct ListNode* b1 = create_node(5);
    b1->next = create_node(6);
    b1->next->next = create_node(1);
    b1->next->next->next = c1;

    struct ListNode* res1 = getIntersectionNode(a1, b1);
    printf("Test 1: ListA & ListB 相交於節點 8\n");
    printf("Result:   Intersected at %s\n", res1 ? (res1->val == 8 ? "Node with val 8 (正確)" : "錯誤數值") : "NULL");
    printf("Expected: Intersected at Node with val 8\n\n");

    // 測試案例 2: 完全無交點
    // ListA: 2 -> 6 -> 4
    // ListB: 1 -> 5
    struct ListNode* a2 = create_node(2);
    a2->next = create_node(6);
    a2->next->next = create_node(4);

    struct ListNode* b2 = create_node(1);
    b2->next = create_node(5);

    struct ListNode* res2 = getIntersectionNode(a2, b2);
    printf("Test 2: 完全無相交\n");
    printf("Result:   %s\n", res2 == NULL ? "NULL (正確)" : "錯誤有值");
    printf("Expected: NULL\n");

    // 釋放記憶體
    free(a1->next); free(a1);
    free(b1->next->next); free(b1->next); free(b1);
    free(c1->next->next); free(c1->next); free(c1);
    free(a2->next->next); free(a2->next); free(a2);
    free(b2->next); free(b2);

    return 0;
}
