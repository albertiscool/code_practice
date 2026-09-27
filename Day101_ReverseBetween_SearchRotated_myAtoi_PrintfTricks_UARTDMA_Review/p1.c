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
 * 題目 1: 反轉指定區間鏈結串列 (LeetCode #92 - Reverse Linked List II)
 * 難度: 🟡 一線 IC 設計廠 (聯發科、瑞昱、聯詠) 雙指標與節點重新穿線高頻必考題
 * 
 * 題目說明:
 * 給定單向鏈結串列的頭節點 head 以及兩個整數 left 和 right (1-indexed，滿足 1 <= left <= right <= 串列長度)。
 * 請在原地 (In-place) 一次遍歷 (One-pass) 中，將位置從 left 到 right 之間的節點反轉，並回傳反轉後的頭節點。
 * 
 * 範例 1:
 *   輸入: head = [1, 2, 3, 4, 5], left = 2, right = 4
 *   輸出: [1, 4, 3, 2, 5]
 * 
 * 範例 2:
 *   輸入: head = [5], left = 1, right = 1
 *   輸出: [5]
 * 
 * 限制條件:
 * - 時間複雜度要求: O(N) 一次遍歷完成
 * - 空間複雜度要求: 嚴格 O(1) 額外空間 (不得建立新節點，只能修改指標方向)
 */
struct ListNode* reverseBetween(struct ListNode* head, int left, int right) {
    if (head == NULL || left == right) {
        return head;
    }

    // 建立 Dummy Node (虛擬頭節點)，解決 left = 1 時 head 會被替換的邊界問題
    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = head;

    // 1. 走到反轉區間的前一個節點 (pre)
    struct ListNode* pre = &dummy;
    for (int i = 0; i < left - 1; i++) {
        pre = pre->next;
    }

    // 2. curr 為反轉區間的第一個節點 (反轉完成後它會變成此區間的尾巴)
    struct ListNode* curr = pre->next;

    // 3. 頭插法 (Head Insertion)：依序將 curr 後面的節點拔下來，插到 pre 的正後方
    for (int i = 0; i < right - left; i++) {
        struct ListNode* next_node = curr->next;
        curr->next = next_node->next;
        next_node->next = pre->next;
        pre->next = next_node;
    }

    return dummy.next;
}

/* 
 * ============================================================================
 * 🔍【原始代碼盲點檢視與解析】
 * ============================================================================
 * 你原先寫的代碼：
 * 
 * struct ListNode* reverseBetween(struct ListNode* head, int left, int right) {
 *     if(head == NULL) {
 *         return NULL:  // 盲點 1: 筆誤，分號打成冒號 ':'
 *     }
 *     if(left == right) {
 *         return head;
 *     }
 * 
 *     struct ListNode* pre = NULL;
 *     struct ListNode* curr = head;
 *     for(int i = 0; i < left - 1; i++) {
 *         curr = curr->next; // 盲點 2: pre 沒有跟著走，pre 依然停在 NULL
 *     }
 * 
 *     for(int j = left; j < right; j++) {
 *         struct ListNode* next_node = curr->next;
 *         curr->next = pre;
 *         curr = next_node;
 *         pre = curr;  // 盲點 3: pre = curr 導致 pre 與 curr 變成同一個節點！
 *                      // 下一輪 curr->next = pre 會導致節點自己指向自己產生 Cycle！
 *     }
 *     
 *     return head; // 盲點 4: 若 left = 1，真正的頭節點已經換人，回傳 head 會出錯；
 *                  // 且反轉區間沒有與前後節點 (left-1 和 right+1) 重新縫合接回。
 * }
 * 
 * 💡【白板題解題心法 - 頭插法 (Head Insertion)】
 * 在單向鏈結串列中反轉 [left, right] 區間，最不易出錯的做法是「頭插法」：
 * 假設 pre = 1, curr = 2: 串列為 1 -> 2 -> 3 -> 4 -> 5
 * 第一步：把 3 拔下來插到 pre 後面 -> 1 -> 3 -> 2 -> 4 -> 5
 * 第二步：把 4 拔下來插到 pre 後面 -> 1 -> 4 -> 3 -> 2 -> 5
 * 只要做 (right - left) 次，curr 會自然退到尾巴，整個區間就原地完成反轉！
 * ============================================================================
 */

// 輔助函式: 建立鏈結串列節點
static struct ListNode* create_node(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

// 輔助函式: 印出鏈結串列
static void print_list(struct ListNode* head) {
    printf("[");
    struct ListNode* curr = head;
    while (curr) {
        printf("%d%s", curr->val, curr->next ? " -> " : "");
        curr = curr->next;
    }
    printf("]\n");
}

int main(void) {
    printf("=== Day 101 - 白板題 1: 反轉指定區間鏈結串列 (LeetCode #92) ===\n\n");

    // 測試案例 1: 1 -> 2 -> 3 -> 4 -> 5, left = 2, right = 4
    struct ListNode* head1 = create_node(1);
    head1->next = create_node(2);
    head1->next->next = create_node(3);
    head1->next->next->next = create_node(4);
    head1->next->next->next->next = create_node(5);

    printf("Test 1 原串列: ");
    print_list(head1);
    printf("預期反轉 (left=2, right=4): [1 -> 4 -> 3 -> 2 -> 5]\n");
    head1 = reverseBetween(head1, 2, 4);
    printf("實際輸出: ");
    print_list(head1);
    printf("\n");

    // 測試案例 2: 單一節點 [5], left = 1, right = 1
    struct ListNode* head2 = create_node(5);
    printf("Test 2 原串列: ");
    print_list(head2);
    printf("預期反轉 (left=1, right=1): [5]\n");
    head2 = reverseBetween(head2, 1, 1);
    printf("實際輸出: ");
    print_list(head2);
    printf("\n");

    // 測試案例 3: 反轉包含頭節點 [3, 5], left = 1, right = 2
    struct ListNode* head3 = create_node(3);
    head3->next = create_node(5);
    printf("Test 3 原串列: ");
    print_list(head3);
    printf("預期反轉 (left=1, right=2): [5 -> 3]\n");
    head3 = reverseBetween(head3, 1, 2);
    printf("實際輸出: ");
    print_list(head3);

    return 0;
}
