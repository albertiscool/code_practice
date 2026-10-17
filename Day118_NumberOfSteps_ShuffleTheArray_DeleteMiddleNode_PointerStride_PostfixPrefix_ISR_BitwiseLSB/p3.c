#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/**
 * LeetCode #2095: Delete the Middle Node of a Linked List
 * (群聯 HackMD 考古題原題：鏈結串列快慢指標、中點定位與刪除操作)
 * 
 * 題目描述：
 * 給定一個鏈結串列的頭節點 head。請刪除該鏈結串列的「中間節點」，並返回修改後的頭節點 head。
 * 
 * 定義：
 * - 長度為 n 的鏈結串列，中間節點的 0-indexed 索引為 ⌊ n / 2 ⌋。
 *   例如：
 *   - n = 1: 索引 0 (唯一節點被刪除，返回 NULL)
 *   - n = 2: 索引 1 (刪除第 2 個節點)
 *   - n = 3: 索引 1 (刪除第 2 個節點)
 *   - n = 4: 索引 2 (刪除第 3 個節點)
 *   - n = 5: 索引 2 (刪除第 3 個節點)
 * 
 * 限制條件：
 * - 鏈結串列中的節點數在範圍 [1, 10^5] 內
 * - 1 <= Node.val <= 10^5
 * - 挑戰目標：你能否在單次遍歷 (One-pass) 內完成？且需注意主動釋放被刪除節點的記憶體 (free)。
 * 
 * 範例 1:
 *   輸入: head = [1,3,4,7,1,2,6]
 *   輸出: [1,3,4,1,2,6]
 *   解釋: n = 7，中間節點為索引 3 (值為 7 的節點)，刪除後返回 [1,3,4,1,2,6]。
 * 
 * 範例 2:
 *   輸入: head = [1,2,3,4]
 *   輸出: [1,2,4]
 *   解釋: n = 4，中間節點為索引 2 (值為 3 的節點)，刪除後返回 [1,2,4]。
 * 
 * 範例 3:
 *   輸入: head = [2,1]
 *   輸出: [2]
 *   解釋: n = 2，中間節點為索引 1 (值為 1 的節點)，刪除後返回 [2]。
 * 
 * 範例 4 (關鍵邊界):
 *   輸入: head = [1]
 *   輸出: [] (即 NULL)
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* deleteMiddle(struct ListNode* head) {
    // 【使用者原始白板實作】(Accepted 2 ms / Beats 95%+)
    if(head == NULL || head->next == NULL) return NULL;

    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* pre = &dummy;
    struct ListNode* slow = head;
    struct ListNode* fast = head;
    while(fast != NULL && fast->next != NULL)
    {
        pre = pre->next;
        slow = slow->next;
        fast = fast->next->next;
    }
    pre->next = slow->next;
    free(slow);
    return head;
}

/*
 * =====================================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * =====================================================================================
 * struct ListNode* deleteMiddle(struct ListNode* head) {
 *     if(head == NULL || head->next == NULL) return NULL;
 * 
 *     struct ListNode dummy;
 *     dummy.next = head;
 *     struct ListNode* pre = &dummy;
 *     struct ListNode* slow = head;
 *     struct ListNode* fast = head;
 *     while(fast != NULL && fast->next != NULL)
 *     {
 *         pre = pre->next;
 *         slow = slow->next;
 *         fast = fast->next->next;
 *     }
 *     pre->next = slow->next;
 *     free(slow);
 *     return head;
 * }
 * =====================================================================================
 * 【面試亮點剖析 & 快慢雙指標工程師素養】 (LeetCode #2095: Delete Middle Node)
 * =====================================================================================
 * 
 * 1. 滿分解法亮點 (一次 AC，完美工程素養)：
 *    - 極限邊界特判：開頭精準捕捉 `head->next == NULL`，單節點刪除後直接返回 NULL。
 *    - Stack Dummy Node (虛擬頭節點)：
 *      使用區域變數 `struct ListNode dummy` 放在 Stack 上，0 額外 malloc 開銷，
 *      讓 `pre` 指標能無縫處理「頭節點後繼」的縫合問題。
 *    - 快慢指標單趟定位 (One-pass O(N))：
 *      `fast` 一次走 2 步，`slow` 一次走 1 步，迴圈結束瞬間 `slow` 必然精準停在正中間！
 *    - 專業韌體防洩漏細節：
 *      在斷開 `pre->next = slow->next` 之後，主動呼叫 `free(slow)` 釋放節點記憶體，
 *      完全展現出 C 語言底層工程師杜絕 Memory Leak 的高度專業素養！
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

static bool compareList(const struct ListNode* head, const int* arr, int size) {
    const struct ListNode* curr = head;
    for (int i = 0; i < size; i++) {
        if (!curr || curr->val != arr[i]) return false;
        curr = curr->next;
    }
    return curr == NULL;
}

static void freeList(struct ListNode* head) {
    while (head) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    printf("=== Test P3: Delete the Middle Node of a Linked List ===\n");

    // Test 1: [1, 3, 4, 7, 1, 2, 6] -> [1, 3, 4, 1, 2, 6]
    int a1[] = {1, 3, 4, 7, 1, 2, 6};
    int exp1[] = {1, 3, 4, 1, 2, 6};
    struct ListNode* l1 = createList(a1, 7);
    l1 = deleteMiddle(l1);
    assert(compareList(l1, exp1, 6));
    freeList(l1);
    printf("Test 1 Passed: [1,3,4,7,1,2,6] -> [1,3,4,1,2,6]\n");

    // Test 2: [1, 2, 3, 4] -> [1, 2, 4]
    int a2[] = {1, 2, 3, 4};
    int exp2[] = {1, 2, 4};
    struct ListNode* l2 = createList(a2, 4);
    l2 = deleteMiddle(l2);
    assert(compareList(l2, exp2, 3));
    freeList(l2);
    printf("Test 2 Passed: [1,2,3,4] -> [1,2,4]\n");

    // Test 3: [2, 1] -> [2]
    int a3[] = {2, 1};
    int exp3[] = {2};
    struct ListNode* l3 = createList(a3, 2);
    l3 = deleteMiddle(l3);
    assert(compareList(l3, exp3, 1));
    freeList(l3);
    printf("Test 3 Passed: [2,1] -> [2]\n");

    // Test 4: [1] -> [] (NULL)
    int a4[] = {1};
    struct ListNode* l4 = createList(a4, 1);
    l4 = deleteMiddle(l4);
    assert(l4 == NULL);
    printf("Test 4 Passed: [1] -> NULL\n");

    printf("All P3 Local Tests Passed!\n");
    return 0;
}
