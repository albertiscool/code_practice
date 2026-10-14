#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/**
 * LeetCode #234: Palindrome Linked List
 * 
 * 題目描述：
 * 給定一個單向鏈結串列的頭節點 head，請判斷該鏈結串列是否為「回文鏈結串列 (Palindrome)」。
 * 如果是，返回 true；否則返回 false。
 * 
 * 限制條件：
 * - 鏈結串列中的節點數在範圍 [1, 10^5] 內
 * - 0 <= Node.val <= 9
 * - 進階挑戰：你能否在時間複雜度 O(N) 且額外空間複雜度 O(1) 的條件下解決此問題？
 * 
 * 範例 1:
 *   輸入: head = [1,2,2,1]
 *   輸出: true
 * 
 * 範例 2:
 *   輸入: head = [1,2]
 *   輸出: false
 * 
 * 範例 3:
 *   輸入: head = [1]
 *   輸出: true
 * 
 * 範例 4:
 *   輸入: head = [1,2,3,2,1]
 *   輸出: true
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

bool isPalindrome(struct ListNode* head) {
    if (head == NULL || head->next == NULL) {
        return true;
    }

    // 第一階段：快慢指標尋找前半段的尾節點 (slow 停在前半段結尾)
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 第二階段：使用「頭插法 (Head-Insertion)」原地反轉後半段
    // slow 扮演 pre 的角色，鎖定在後半段的前驅守門人
    struct ListNode* pre = slow;
    struct ListNode* curr = pre->next;

    while (curr != NULL && curr->next != NULL) {
        struct ListNode* next_node = curr->next;
        curr->next = next_node->next;  // 1. curr 跳過 next_node，接上後面
        next_node->next = pre->next;   // 2. next_node 插到 pre 後面 (接上當前頭部)
        pre->next = next_node;        // 3. pre 接上最新插進來的頭部 next_node
    }

    // 第三階段：雙指標同步比對前半段與後半段
    struct ListNode* p1 = head;
    struct ListNode* p2 = slow->next; // pre->next (即 slow->next) 正是頭插法反轉後的後半段新頭部！
    bool is_palin = true;
    while (p2 != NULL) {
        if (p1->val != p2->val) {
            is_palin = false;
            break;
        }
        p1 = p1->next;
        p2 = p2->next;
    }

    return is_palin;
}

/*
 * =====================================================================================
 * 【面試盲點剖析 & 邊界除錯精華】 (LeetCode #234: Palindrome Linked List)
 * =====================================================================================
 * 原始盲點剖析：
 * 1. 反轉鏈結串列邏輯混亂引發 Null Pointer 解引用與自環死鎖 (Self-cycle)：
 *    原程式碼在反轉時寫了：
 *    ```c
 *    struct ListNode* next_node = curr->next;
 *    curr = next_node->next;      // ⚠️ 盲點 1：當 curr 是最後節點時，next_node 為 NULL，
 *                                 // 執行 next_node->next 立刻觸發 NULL 解引用 (Runtime Error)！
 *    pre->next = next_node;
 *    next_node->next = pre->next; // ⚠️ 盲點 2：上一行剛令 pre->next = next_node，
 *                                 // 這裡又令 next_node->next = pre->next，形成自己指自己的死環！
 *    ```
 * 
 * 2. 回歸 Day 111 練習過的標準「三指標反轉範式 (Classic 3-Pointer In-place Reverse)」：
 *    反轉單向鏈結串列只需經典的三指標移動，絕對不要發明複雜的跳接語法：
 *    ```c
 *    struct ListNode* prev = NULL;
 *    struct ListNode* curr = slow->next;
 *    while (curr != NULL) {
 *        struct ListNode* next_node = curr->next;
 *        curr->next = prev;
 *        prev = curr;
 *        curr = next_node;
 *    }
 *    ```
 * 
 * 3. 快慢指標終止條件細節 (fast->next && fast->next->next)：
 *    - 偶數長度 (如 1->2->2->1)：fast 停在第二個節點，slow 停在第一個 2 (前半段尾端)，
 *      slow->next 剛好是後半段的開頭 (2->1)。
 *    - 奇數長度 (如 1->2->3->2->1)：fast 停在尾節點 1，slow 停在正中間的 3，
 *      slow->next 剛好是後半段的開頭 (2->1)。
 *    - 這樣設計能保證 slow->next 永遠精準切齊後半段！
 * =====================================================================================
 */

// === 測試輔助函式 ===
static struct ListNode* createList(const int* arr, int size) {
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

static void freeList(struct ListNode* head) {
    while (head) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    printf("=== Test P3: Palindrome Linked List ===\n");

    // Test 1: [1, 2, 2, 1] -> true
    int arr1[] = {1, 2, 2, 1};
    struct ListNode* l1 = createList(arr1, 4);
    assert(isPalindrome(l1) == true);
    freeList(l1);
    printf("Test 1 Passed: [1, 2, 2, 1] -> true\n");

    // Test 2: [1, 2] -> false
    int arr2[] = {1, 2};
    struct ListNode* l2 = createList(arr2, 2);
    assert(isPalindrome(l2) == false);
    freeList(l2);
    printf("Test 2 Passed: [1, 2] -> false\n");

    // Test 3: [1] -> true
    int arr3[] = {1};
    struct ListNode* l3 = createList(arr3, 1);
    assert(isPalindrome(l3) == true);
    freeList(l3);
    printf("Test 3 Passed: [1] -> true\n");

    // Test 4: [1, 2, 3, 2, 1] -> true
    int arr4[] = {1, 2, 3, 2, 1};
    struct ListNode* l4 = createList(arr4, 5);
    assert(isPalindrome(l4) == true);
    freeList(l4);
    printf("Test 4 Passed: [1, 2, 3, 2, 1] -> true\n");

    printf("All P3 Local Tests Passed!\n");
    return 0;
}
