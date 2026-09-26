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
 * 題目 2: 回文鏈結串列 (LeetCode #234 - Palindrome Linked List)
 * 難度: 🟡 一線 IC 設計廠與系統廠 動態指標綜合考查必考題
 * 
 * 題目說明:
 * 給定一個單向鏈結串列的頭節點 head，請判斷該鏈結串列是否為「回文串列 (Palindrome)」。
 * 如果是，回傳 true；否則回傳 false。
 * 
 * 限制條件:
 * - 時間複雜度要求: O(N)
 * - 空間複雜度要求: 嚴格 O(1) 額外空間 (嚴禁複製整個串列到陣列中或使用額外容器)
 */
bool isPalindrome(struct ListNode* head) {
    if (head == NULL || head->next == NULL) {
        return true;
    }

    // 步驟 1: 快慢指針找到中點
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 步驟 2: 原地反轉後半段串列
    struct ListNode* pre = NULL;
    while (slow != NULL) {
        struct ListNode* next_node = slow->next;
        slow->next = pre;
        pre = slow;
        slow = next_node;
    }

    // 步驟 3: 雙指針從兩端同步比對 (pre 為反轉後的後半段新頭節點)
    struct ListNode* p1 = head;
    struct ListNode* p2 = pre;
    while (p2 != NULL) {
        if (p1->val != p2->val) {
            return false;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    return true;
}

/*
================================================================================
【原始盲點剖析與使用者程式碼存檔】
================================================================================
使用者原始實作：
bool isPalindrome(struct ListNode* head) {
    if(head == NULL || head->next == NULL)
    {
        return true;
    }
    struct ListNode* p1 = head;
    struct ListNode* p2 = head;
    while(p2 != NULL && p2->next != NULL)
    {
        p1 = p1->next;
        p2 = p2->next->next;
    }

    struct ListNode* pre = NULL;
    while(p1 != NULL)
    {
        struct ListNode* next_node = p1->next;
        p1->next = pre;
        pre = p1;
        p1 = next_node;
    }

    struct ListNode* p3 = head;
    while(p1 != NULL) // ❌ 致命盲點：p1 在反轉後已經走到了 NULL！
    {
        if(p1->val != p3->val)
        {
            return false;
        }
        p1 = p1->next;
        p3 = p3->next;
    }
    return true;
}

盲點剖析：指針名稱筆誤導致比對迴圈「完全未執行」
- 在步驟 2 的反轉迴圈中：
  `while(p1 != NULL) { ... p1 = next_node; }`
  當該迴圈結束時，`p1` 已經一路走到鏈結串列末端的 `NULL`，
  而反轉後的後半段串列的「真正頭節點」其實保存在 `pre` 當中！
- 原始程式碼在步驟 3 進行回文比對時，依然寫了 `while (p1 != NULL)`：
  由於 `p1` 此刻就是 `NULL`，這個比對迴圈根本連一次都沒有進入，
  直接跳過並執行 `return true;`！
  這導致所有輸入（即使是非回文的 [1, 2]）都會被一律誤判為 true！
- 修正方式：
  比對時應使用 `pre` 指向後半段（`while (p2 != NULL)`），依序與前半段 `head` 逐一節點核對。
================================================================================
*/

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

// 輔助函式: 釋放串列記憶體
static void free_list(struct ListNode* head) {
    while (head != NULL) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    printf("=== Day 100 - 白板題 2: 回文鏈結串列 (LeetCode #234) ===\n\n");

    // 測試案例 1: 偶數節點回文 [1, 2, 2, 1]
    int a1[] = {1, 2, 2, 1};
    struct ListNode* l1 = create_list(a1, 4);
    bool r1 = isPalindrome(l1);
    printf("Test 1: [1, 2, 2, 1]\n");
    printf("Result:   %s\n", r1 ? "true (回文)" : "false (非回文)");
    printf("Expected: true (回文)\n\n");
    free_list(l1);

    // 測試案例 2: 奇數節點回文 [1, 2, 3, 2, 1]
    int a2[] = {1, 2, 3, 2, 1};
    struct ListNode* l2 = create_list(a2, 5);
    bool r2 = isPalindrome(l2);
    printf("Test 2: [1, 2, 3, 2, 1]\n");
    printf("Result:   %s\n", r2 ? "true (回文)" : "false (非回文)");
    printf("Expected: true (回文)\n\n");
    free_list(l2);

    // 測試案例 3: 非回文 [1, 2]
    int a3[] = {1, 2};
    struct ListNode* l3 = create_list(a3, 2);
    bool r3 = isPalindrome(l3);
    printf("Test 3: [1, 2]\n");
    printf("Result:   %s\n", r3 ? "true (回文)" : "false (非回文)");
    printf("Expected: false (非回文)\n\n");
    free_list(l3);

    // 測試案例 4: 單節點 [1]
    int a4[] = {1};
    struct ListNode* l4 = create_list(a4, 1);
    bool r4 = isPalindrome(l4);
    printf("Test 4: [1]\n");
    printf("Result:   %s\n", r4 ? "true (回文)" : "false (非回文)");
    printf("Expected: true (回文)\n");
    free_list(l4);

    return 0;
}
