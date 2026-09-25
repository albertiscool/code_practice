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
 * 題目 2: 刪除鏈結串列的倒數第 N 個節點 (LeetCode #19 - Remove Nth Node From End of List)
 * 難度: 🟡 一線 IC 設計廠與系統廠 雙指針動態操作必考題
 * 
 * 題目說明:
 * 給定一個鏈結串列的頭節點 head 以及一個整數 n，請刪除鏈結串列中「倒數第 n 個節點」，
 * 並回傳修改後的鏈結串列頭節點。
 * 
 * 限制條件:
 * - 能否在「只遍歷一次串列 (One-pass)」的情況下完成？
 * - 空間複雜度: O(1)
 */
struct ListNode* removeNthFromEnd(struct ListNode* head, int n) {
    // === 請在此實作你的白板題解答 ===
    if(head == NULL)
    {
        return NULL;
    }
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* slow = &dummy;
    struct ListNode* fast = head;
    for(int i = 0; i < n; i++)
    {
        fast = fast->next;
    }
    while(fast != NULL)
    {
        slow = slow->next;
        fast = fast->next;
    }
    slow->next = slow->next->next;
    return dummy.next;
}

// 輔助函式: 由陣列建立鏈結串列
static struct ListNode* create_list(const int* arr, int size) {
    if (size == 0) return NULL;
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;
    struct ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        curr = curr->next;
        curr->val = arr[i];
        curr->next = NULL;
    }
    return head;
}

// 輔助函式: 印出串列
static void print_list(const struct ListNode* head) {
    printf("[");
    const struct ListNode* curr = head;
    while (curr != NULL) {
        printf("%d%s", curr->val, curr->next ? " -> " : "");
        curr = curr->next;
    }
    printf("]\n");
}

// 輔助函式: 釋放串列
static void free_list(struct ListNode* head) {
    while (head != NULL) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    printf("=== Day 99 - 白板題 2: 刪除鏈結串列倒數第 N 個節點 (LeetCode #19) ===\n\n");

    // 測試案例 1: 刪除中間節點 [1, 2, 3, 4, 5], n = 2 (刪除 4)
    int a1[] = {1, 2, 3, 4, 5};
    struct ListNode* l1 = create_list(a1, 5);
    printf("Test 1 Input:    ");
    print_list(l1);
    struct ListNode* res1 = removeNthFromEnd(l1, 2);
    printf("Test 1 Result:   ");
    print_list(res1);
    printf("Expected:        [1 -> 2 -> 3 -> 5]\n\n");
    free_list(res1);

    // 測試案例 2: 刪除頭節點 [1, 2], n = 2 (刪除 1)
    int a2[] = {1, 2};
    struct ListNode* l2 = create_list(a2, 2);
    printf("Test 2 Input:    ");
    print_list(l2);
    struct ListNode* res2 = removeNthFromEnd(l2, 2);
    printf("Test 2 Result:   ");
    print_list(res2);
    printf("Expected:        [2]\n\n");
    free_list(res2);

    // 測試案例 3: 單節點刪除 [1], n = 1 (刪除後變空串列)
    int a3[] = {1};
    struct ListNode* l3 = create_list(a3, 1);
    printf("Test 3 Input:    ");
    print_list(l3);
    struct ListNode* res3 = removeNthFromEnd(l3, 1);
    printf("Test 3 Result:   ");
    print_list(res3);
    printf("Expected:        []\n");
    free_list(res3);

    return 0;
}
