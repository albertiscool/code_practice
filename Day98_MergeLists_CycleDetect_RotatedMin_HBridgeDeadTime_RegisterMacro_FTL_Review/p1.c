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
 * 題目 1: 合併兩個已排序串列 (LeetCode #21 - Merge Two Sorted Lists)
 * 難度: 🟡 一線 IC 設計廠 / 系統廠 鏈結串列第一高頻題
 * 
 * 題目說明:
 * 將兩個升序鏈結串列 list1 與 list2 合併為一個新的升序鏈結串列並回傳。
 * 新鏈結串列必須由兩個既有串列的節點原地拼接而成 (In-place splice)。
 * 
 * 限制條件:
 * - 空間複雜度要求: O(1) 原地拼接，嚴禁為新串列重新 malloc 每個節點！
 * - 時間複雜度要求: O(M + N)
 */
struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    // === 請在此實作你的白板題解答 ===
    if(list1 == NULL && list2 == NULL)
    {
        return NULL;
    }
    if(list1 == NULL)
    {
        return list2;
    }
    if(list2 == NULL)
    {
        return list1;
    }

    struct ListNode dummy;
    dummy.next = NULL;
    struct ListNode* tail = &dummy;
    while(list1 != NULL && list2 != NULL)
    {
        if(list1->val < list2->val)
        {
            tail->next = list1;
            list1 = list1->next;
        }
        else
        {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }
    if(list1 == NULL)
    {
        tail->next = list2;
    }
    if(list2 == NULL)
    {
        tail->next = list1;
    }
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

// 輔助函式: 釋放串列記憶體
static void free_list(struct ListNode* head) {
    while (head != NULL) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    printf("=== Day 98 - 白板題 1: 合併兩個已排序串列 (LeetCode #21) ===\n\n");

    // 測試案例 1: 兩個標準非空串列
    int a1[] = {1, 2, 4};
    int b1[] = {1, 3, 4};
    struct ListNode* l1 = create_list(a1, 3);
    struct ListNode* l2 = create_list(b1, 3);
    printf("Test 1 Input: List1 = ");
    print_list(l1);
    printf("             List2 = ");
    print_list(l2);
    struct ListNode* res1 = mergeTwoLists(l1, l2);
    printf("Test 1 Result:       ");
    print_list(res1);
    printf("Expected:            [1 -> 1 -> 2 -> 3 -> 4 -> 4]\n\n");
    free_list(res1);

    // 測試案例 2: 其中一個為空串列
    int a2[] = {2, 5, 8};
    struct ListNode* l3 = create_list(a2, 3);
    struct ListNode* l4 = NULL;
    printf("Test 2 Input: List1 = ");
    print_list(l3);
    printf("             List2 = []\n");
    struct ListNode* res2 = mergeTwoLists(l3, l4);
    printf("Test 2 Result:       ");
    print_list(res2);
    printf("Expected:            [2 -> 5 -> 8]\n\n");
    free_list(res2);

    // 測試案例 3: 兩個皆為空串列
    struct ListNode* res3 = mergeTwoLists(NULL, NULL);
    printf("Test 3 Input: List1 = [], List2 = []\n");
    printf("Test 3 Result:       ");
    print_list(res3);
    printf("Expected:            []\n");

    return 0;
}
