#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #203: Remove Linked List Elements
 * 
 * 題目描述：
 * 給定一個單向鏈結串列的頭節點 head 和一個整數 val，
 * 請刪除鏈結串列中所有滿足 Node.val == val 的節點，並返回新的頭節點。
 * 
 * 限制條件：
 * - 串列中的節點數在範圍 [0, 10^4] 內
 * - 1 <= Node.val <= 50
 * - 0 <= val <= 50
 * 
 * 範例 1:
 *   輸入: head = [1,2,6,3,4,5,6], val = 6
 *   輸出: [1,2,3,4,5]
 * 
 * 範例 2:
 *   輸入: head = [], val = 1
 *   輸出: []
 * 
 * 範例 3:
 *   輸入: head = [7,7,7,7], val = 7
 *   輸出: []
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* removeElements(struct ListNode* head, int val) {
    // 請在此處實現純白板程式碼
    if(head == NULL) return head;

    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* pre = &dummy;
    struct ListNode* curr = head;

    while(curr != NULL)
    {
        if(curr->val == val)
        {
            struct ListNode* next_node = curr->next;
            pre->next = next_node;
            free(curr);
            curr = next_node;
        }
        else
        {
            pre = pre->next;
            curr = curr->next;
        }
    }
    return dummy.next;
}

/*
 * =====================================================================================
 * 【面試亮點剖析 & 韌體記憶體管理】 (LeetCode #203: Remove Linked List Elements)
 * =====================================================================================
 * 滿分解法三大亮點：
 * 1. 記憶體防護意識頂尖 (free(curr) 零記憶體洩漏)：
 *    在 LeetCode 與一般白板中，很多面試者只修改 pre->next 就草草了事。
 *    你在斷鏈後「主動釋放節點記憶體 free(curr)」，完全體現 C 語言韌體工程師該有的嚴謹品格！
 * 
 * 2. 指針不變量維護得宜 (連續重複節點安全刪除)：
 *    當命中目標進行刪除時，你「只移動 curr，不移動 pre」：
 *    - 這保證了若下一個節點數值依然是 val（如 [1, 2, 2, 1] 刪除 2），pre 依然能在前哨站精準縫合！
 *    - 只有在不等於 val 時才同步 pre = pre->next 與 curr = curr->next，指標節奏絲毫不亂。
 * 
 * 3. 虛擬頭節點 (Dummy Node Pattern)：
 *    利用 Stack 上宣告的 struct ListNode dummy，一舉解決「頭節點連續命中需要刪除」的特判，
 *    無需 malloc 額外空間，乾淨俐落！
 * 
 * 進階神技補充 (Linus Torvalds Double Pointer 解法)：
 *   struct ListNode** curr = &head;
 *   while (*curr) {
 *       if ((*curr)->val == val) {
 *           struct ListNode* temp = *curr;
 *           *curr = (*curr)->next;
 *           free(temp);
 *       } else {
 *           curr = &(*curr)->next;
 *       }
 *   }
 *   return head;
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

static int compareList(struct ListNode* head, const int* arr, int size) {
    int i = 0;
    while (head && i < size) {
        if (head->val != arr[i]) return 0;
        head = head->next;
        i++;
    }
    return (head == NULL && i == size);
}

int main(void) {
    printf("=== Test P3: Remove Linked List Elements ===\n");

    // Test 1: [1,2,6,3,4,5,6], val = 6 -> [1,2,3,4,5]
    int arr1[] = {1, 2, 6, 3, 4, 5, 6};
    int exp1[] = {1, 2, 3, 4, 5};
    struct ListNode* l1 = createList(arr1, 7);
    l1 = removeElements(l1, 6);
    assert(compareList(l1, exp1, 5));
    freeList(l1);
    printf("Test 1 Passed: [1,2,6,3,4,5,6], val = 6 -> [1,2,3,4,5]\n");

    // Test 2: [], val = 1 -> []
    struct ListNode* l2 = createList(NULL, 0);
    l2 = removeElements(l2, 1);
    assert(compareList(l2, NULL, 0));
    freeList(l2);
    printf("Test 2 Passed: [], val = 1 -> []\n");

    // Test 3: [7,7,7,7], val = 7 -> []
    int arr3[] = {7, 7, 7, 7};
    struct ListNode* l3 = createList(arr3, 4);
    l3 = removeElements(l3, 7);
    assert(compareList(l3, NULL, 0));
    freeList(l3);
    printf("Test 3 Passed: [7,7,7,7], val = 7 -> []\n");

    // Test 4: [1,2,2,1], val = 2 -> [1,1]
    int arr4[] = {1, 2, 2, 1};
    int exp4[] = {1, 1};
    struct ListNode* l4 = createList(arr4, 4);
    l4 = removeElements(l4, 2);
    assert(compareList(l4, exp4, 2));
    freeList(l4);
    printf("Test 4 Passed: [1,2,2,1], val = 2 -> [1,1]\n");

    printf("All P3 Local Tests Passed!\n");
    return 0;
}
