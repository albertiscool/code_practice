#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * LeetCode #24: Swap Nodes in Pairs
 * 
 * 題目描述：
 * 給定一個單向鏈結串列，請交換每兩個相鄰的節點，並返回交換後的鏈結串列頭節點。
 * 你必須在不修改節點內部數值 (val) 的情況下解決問題（即只能直接修改節點指標 next 的指向）。
 * 
 * 限制條件：
 * - 鏈結串列中的節點數量在範圍 [0, 100] 內
 * - 0 <= Node.val <= 100
 * 
 * 範例 1:
 *   輸入: head = [1,2,3,4]
 *   輸出: [2,1,4,3]
 * 
 * 範例 2:
 *   輸入: head = []
 *   輸出: []
 * 
 * 範例 3:
 *   輸入: head = [1]
 *   輸出: [1]
 * 
 * 範例 4:
 *   輸入: head = [1,2,3]
 *   輸出: [2,1,3]
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* swapPairs(struct ListNode* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = head;

    struct ListNode* prev = &dummy;

    while (prev->next != NULL && prev->next->next != NULL) {
        struct ListNode* first = prev->next;
        struct ListNode* second = prev->next->next;

        // 三步指標重接 (Pointer Rewiring)
        first->next = second->next;  // 1. first 指向下一組 pair (或 NULL)
        second->next = first;        // 2. second 指向 first (完成兩兩相鄰反轉)
        prev->next = second;         // 3. 上一組的尾端 (或 dummy) 接上新的 pair 頭部 (second)

        // 前進到下一組 pair 的前驅節點
        prev = first;
    }

    return dummy.next;
}

/*
 * =====================================================================================
 * 【面試盲點剖析 & 指標精密重組】 (LeetCode #24: Swap Nodes in Pairs)
 * =====================================================================================
 * 原始盲點剖析：
 * 1. 迴圈終止條件過度嚴苛 (curr2->next != NULL)：
 *    原程式碼寫 `while(curr2 != NULL && curr2->next != NULL)`。
 *    - 當串列只有 2 個節點 [1, 2] 時，curr2 為 2，其 next 為 NULL，導致迴圈「一次都沒執行」！
 *    - 當串列為 4 個節點 [1, 2, 3, 4] 時，第一輪換完後，第二輪 curr2 為 4，其 next 為 NULL，
 *      導致第二組 (3, 4) 完全沒被交換就提早退出！
 * 
 * 2. 缺少跨 Pair 的前驅指標 (prev 指標遺失)：
 *    在鏈結串列中，要交換兩個相鄰節點 (first, second)，必須有第三個指標 prev：
 *    - 因為交換後 second 變成該 Pair 的新頭部，上一組 Pair 的尾端必須更新其 next 指向 second！
 *    - 沒有 prev，就無法在換完 (3, 4) 變成 (4, 3) 時，讓第 1 個節點從指向 3 改指向 4！
 * 
 * 標準解題範式 (Dummy Node Pattern)：
 *   設 dummy -> first -> second -> next_pair
 *   1. first->next = second->next; (first 接上下一段)
 *   2. second->next = first;       (second 接上 first)
 *   3. prev->next = second;        (prev 接上 second)
 *   4. prev = first;               (prev 移動到新尾端，準備處理下一組)
 * 
 * 進階神技 (Linus Torvalds Double Pointer 解法)：
 *   若不想宣告 dummy node，可使用指標的指標 struct ListNode** pp = &head;
 *   while (*pp && (*pp)->next) {
 *       struct ListNode *a = *pp, *b = a->next;
 *       a->next = b->next; b->next = a;
 *       *pp = b; pp = &(a->next);
 *   }
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
    printf("=== Test P3: Swap Nodes in Pairs ===\n");

    // Test 1: [1, 2, 3, 4] -> [2, 1, 4, 3]
    int arr1[] = {1, 2, 3, 4};
    int exp1[] = {2, 1, 4, 3};
    struct ListNode* l1 = createList(arr1, 4);
    l1 = swapPairs(l1);
    assert(compareList(l1, exp1, 4));
    freeList(l1);
    printf("Test 1 Passed: [1, 2, 3, 4] -> [2, 1, 4, 3]\n");

    // Test 2: [] -> []
    struct ListNode* l2 = createList(NULL, 0);
    l2 = swapPairs(l2);
    assert(compareList(l2, NULL, 0));
    freeList(l2);
    printf("Test 2 Passed: [] -> []\n");

    // Test 3: [1] -> [1]
    int arr3[] = {1};
    int exp3[] = {1};
    struct ListNode* l3 = createList(arr3, 1);
    l3 = swapPairs(l3);
    assert(compareList(l3, exp3, 1));
    freeList(l3);
    printf("Test 3 Passed: [1] -> [1]\n");

    // Test 4: [1, 2, 3] -> [2, 1, 3]
    int arr4[] = {1, 2, 3};
    int exp4[] = {2, 1, 3};
    struct ListNode* l4 = createList(arr4, 3);
    l4 = swapPairs(l4);
    assert(compareList(l4, exp4, 3));
    freeList(l4);
    printf("Test 4 Passed: [1, 2, 3] -> [2, 1, 3]\n");

    printf("All P3 Local Tests Passed!\n");
    return 0;
}
