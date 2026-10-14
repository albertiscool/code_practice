#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/**
 * 韌體面試經典白板考題：有序鏈結串列插入節點 (Sorted Linked List Insertion)
 * 
 * 題目描述：
 * 給定一個已按「非遞減順序（由小到大）」排列的單向鏈結串列頭節點 head，以及一個整數 val。
 * 請動態配置 (malloc) 一個新節點，並將其插入到鏈結串列中的正確位置，
 * 使得插入後的鏈結串列依然維持由小到大的非遞減排序，最後返回插入後的新頭節點。
 * 
 * 面試考察核心邊界：
 * 1. 串列為空 (head == NULL) 時插入。
 * 2. 插入在最前端 (val 比原本的 head->val 還小，新節點成為新 head)。
 * 3. 插入在串列中間。
 * 4. 插入在最尾端 (val 比所有節點都大，接在最後一個節點之後)。
 * 5. 存在相同數值節點時的穩定插入。
 * 
 * 挑戰目標：
 * 能否使用 Dummy Node（虛擬頭節點）或 Pointer-to-Pointer（雙重指標 struct ListNode**）
 * 寫出完全零邊界特判、代碼極度精簡的滿分解法？
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* insertSorted(struct ListNode* head, int val) {
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* pre = &dummy;
    struct ListNode* curr = head;

    // 關鍵修復：必須加上 curr != NULL 檢查，防範空串列與尾部插入時解引用 NULL！
    while (curr != NULL && curr->val <= val) {
        pre = pre->next;
        curr = curr->next;
    }

    struct ListNode* new = (struct ListNode*)malloc(sizeof(struct ListNode));
    new->val = val;

    // 核心精華：Dummy Node 的真正威力！
    // 不論是插在頭部、中間還是尾部，這兩行代碼完全通吃，完全無需 if (curr == head) 特判！
    pre->next = new;
    new->next = curr;

    return dummy.next;
}

/*
 * =====================================================================================
 * 【面試盲點剖析 & 節點插入精髓】 (Sorted Linked List Insertion)
 * =====================================================================================
 * 原始盲點剖析：
 * 1. 致命缺少 `curr != NULL` 檢查 (導致 Segmentation Fault / HardFault)：
 *    原迴圈寫 `while (curr->val <= val)`。
 *    - 當 head == NULL (空串列) 時，curr 為 NULL，執行 curr->val 立刻引發程式崩潰！
 *    - 當插入數值大於所有節點 (尾部插入，如 [1, 2, 3] 插入 9) 時，curr 走到底變成 NULL，
 *      下一輪迴圈執行 curr->val 也立刻死機！
 *    - 必須加上短路保護：`while (curr != NULL && curr->val <= val)`。
 * 
 * 2. 多寫了冗餘的 `if (curr == head)` (沒有徹底發揮 Dummy Node 的威力)：
 *    當插在最開頭時，curr == head，此時 pre 剛好就是 &dummy！
 *    因此執行：
 *      pre->next = new; (相當於 dummy.next = new)
 *      new->next = curr; (相當於 new->next = head)
 *    你看！它跟一般情況的寫法一模一樣！這正是宣告 Dummy Node 的真正目的——消除所有頭部特判！
 * 
 * 3. 進階神技 (Linus Torvalds "Good Taste" 雙重指標解法，完全不需 Dummy Node)：
 *    struct ListNode** pp = &head;
 *    while (*pp != NULL && (*pp)->val <= val) {
 *        pp = &((*pp)->next);
 *    }
 *    struct ListNode* new = (struct ListNode*)malloc(sizeof(struct ListNode));
 *    new->val = val;
 *    new->next = *pp;
 *    *pp = new;
 *    return head;
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
    printf("=== Test P4: Sorted Linked List Insertion ===\n");

    // 情境 1: 空串列插入 -> [5]
    struct ListNode* l1 = insertSorted(NULL, 5);
    int exp1[] = {5};
    assert(compareList(l1, exp1, 1));
    freeList(l1);
    printf("Test 1 Passed: Empty list insert 5 -> [5]\n");

    // 情境 2: 插入至最前端 (成為新 head) -> [1, 2, 4] 插入 0 -> [0, 1, 2, 4]
    int arr2[] = {1, 2, 4};
    struct ListNode* l2 = createList(arr2, 3);
    l2 = insertSorted(l2, 0);
    int exp2[] = {0, 1, 2, 4};
    assert(compareList(l2, exp2, 4));
    freeList(l2);
    printf("Test 2 Passed: Insert at head (val=0) -> [0, 1, 2, 4]\n");

    // 情境 3: 插入至中間 -> [1, 2, 4, 5] 插入 3 -> [1, 2, 3, 4, 5]
    int arr3[] = {1, 2, 4, 5};
    struct ListNode* l3 = createList(arr3, 4);
    l3 = insertSorted(l3, 3);
    int exp3[] = {1, 2, 3, 4, 5};
    assert(compareList(l3, exp3, 5));
    freeList(l3);
    printf("Test 3 Passed: Insert in middle (val=3) -> [1, 2, 3, 4, 5]\n");

    // 情境 4: 插入至最尾端 -> [1, 2, 3] 插入 9 -> [1, 2, 3, 9]
    int arr4[] = {1, 2, 3};
    struct ListNode* l4 = createList(arr4, 3);
    l4 = insertSorted(l4, 9);
    int exp4[] = {1, 2, 3, 9};
    assert(compareList(l4, exp4, 4));
    freeList(l4);
    printf("Test 4 Passed: Insert at tail (val=9) -> [1, 2, 3, 9]\n");

    // 情境 5: 插入重複數值 -> [1, 3, 5] 插入 3 -> [1, 3, 3, 5]
    int arr5[] = {1, 3, 5};
    struct ListNode* l5 = createList(arr5, 3);
    l5 = insertSorted(l5, 3);
    int exp5[] = {1, 3, 3, 5};
    assert(compareList(l5, exp5, 4));
    freeList(l5);
    printf("Test 5 Passed: Insert duplicate (val=3) -> [1, 3, 3, 5]\n");

    printf("\nAll P4 Local Tests Passed! 恭喜全數通過！\n");
    return 0;
}
