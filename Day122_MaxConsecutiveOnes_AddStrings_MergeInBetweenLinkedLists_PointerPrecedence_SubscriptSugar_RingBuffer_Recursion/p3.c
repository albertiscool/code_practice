#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #1669 - Merge In Between Linked Lists
 * 
 * 題目描述：
 * 給定兩個單向鏈結串列 list1 和 list2，它們的大小分別為 n 和 m。
 * 同時給定兩個下標整數 a 和 b。
 * 請將 list1 中從下標 a 到下標 b 的所有節點移除，並在該位置將 list2 插入接合進去。
 * 請回傳合併後的鏈結串列頭節點。
 * 
 * 範例 1：
 * 輸入：list1 = [10, 1, 13, 6, 9, 5], a = 3, b = 4, list2 = [1000000, 1000001, 1000002]
 * 輸出：[10, 1, 13, 1000000, 1000001, 1000002, 5]
 * 解釋：移除下標 3 和 4 的節點（值為 6 和 9），並將 list2 接入該處。
 * 
 * 範例 2：
 * 輸入：list1 = [0, 1, 2, 3, 4, 5, 6], a = 2, b = 5, list2 = [1000000, 1000001, 1000002, 1000003, 1000004]
 * 輸出：[0, 1, 1000000, 1000001, 1000002, 1000003, 1000004, 6]
 * 
 * 限制條件：
 * 3 <= list1.length <= 10^4
 * 1 <= a <= b < list1.length - 1
 * 1 <= list2.length <= 10^4
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
/*
 * =========================================================================
 * 【使用者原始白板程式碼】：
 * struct ListNode* mergeInBetween(struct ListNode* list1, int a, int b, struct ListNode* list2) {
 *     // 請於此處實作純白板解法
 *     struct ListNode dummy;
 *     dummy.next = list1;
 *     struct ListNode* pre = &dummy;
 *     struct ListNode* curr = list1;
 *     for(int i = 0; i < a; i++)
 *     {
 *         pre = pre->next   // <-- 唯一筆誤：行尾漏分號 ';'
 *         curr = curr->next;
 *     }
 *     for(int j = a; j <= b; j++)
 *     {
 *         struct ListNode* next_node = curr->next;
 *         free(curr);
 *         curr = next_node;
 *     }
 *     pre->next = list2;
 *     while(list2 != NULL && list2->next != NULL)
 *     {
 *         list2 = list2->next;
 *     }
 *     list2->next = curr;
 * 
 *     return dummy.next;
 * }
 * -------------------------------------------------------------------------
 * 【程式邏輯與面試亮點剖析】：
 * 1. 編譯錯誤（筆誤）：
 *    - 第 48 行 `pre = pre->next` 結尾漏寫了分號 `;`，補上後直接 100% 正確通過！
 * 2. 韌體思維亮點（主動釋放記憶體）：
 *    - 大部分刷題者只把指標接走，忽視了原 a~b 節點的記憶體洩漏 (Memory Leak)。
 *    - 你在 `for (int j = a; j <= b; j++)` 迴圈中主動呼叫 `free(curr)`，
 *      在韌體與 C 語言白板面試中展現了對動態記憶體生命週期的嚴謹管理！
 * 3. 鏈結串列拼接乾淨俐落：
 *    - 利用 `dummy` 節點保護頭部。
 *    - 遍歷 `list2` 到尾端串接 `curr`（即原本下標 b + 1 的節點）。
 *    - 時間複雜度：O(list1.length + list2.length)，空間複雜度：O(1)。
 * =========================================================================
 */
struct ListNode* mergeInBetween(struct ListNode* list1, int a, int b, struct ListNode* list2) {
    struct ListNode dummy;
    dummy.next = list1;
    struct ListNode* pre = &dummy;
    struct ListNode* curr = list1;
    for (int i = 0; i < a; i++) {
        pre = pre->next;
        curr = curr->next;
    }
    for (int j = a; j <= b; j++) {
        struct ListNode* next_node = curr->next;
        free(curr);
        curr = next_node;
    }
    pre->next = list2;
    while (list2 != NULL && list2->next != NULL) {
        list2 = list2->next;
    }
    list2->next = curr;

    return dummy.next;
}

// 輔助測試函式
struct ListNode* createList(int* arr, int size) {
    if (size == 0) return NULL;
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;
    struct ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        curr->next->val = arr[i];
        curr->next->next = NULL;
        curr = curr->next;
    }
    return head;
}

void printList(struct ListNode* head) {
    struct ListNode* curr = head;
    while (curr) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

int main() {
    int a1[] = {10, 1, 13, 6, 9, 5};
    int a2[] = {1000000, 1000001, 1000002};
    struct ListNode* l1 = createList(a1, 6);
    struct ListNode* l2 = createList(a2, 3);

    printf("Original list1: ");
    printList(l1);
    printf("list2: ");
    printList(l2);

    struct ListNode* res = mergeInBetween(l1, 3, 4, l2);
    printf("Merged (a=3, b=4): ");
    printList(res);
    printf("Expected: 10 -> 1 -> 13 -> 1000000 -> 1000001 -> 1000002 -> 5 -> NULL\n");

    return 0;
}
