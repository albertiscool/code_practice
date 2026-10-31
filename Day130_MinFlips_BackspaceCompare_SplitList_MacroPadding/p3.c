#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode #725 - Split Linked List in Parts
 *
 * Description:
 * Given the head of a singly linked list and an integer k, split the linked list into k consecutive linked list parts.
 *
 * The length of each part should be as equal as possible: no two parts should have a size differing by more than one.
 * This may lead to some parts being null.
 *
 * The parts should be in the order of occurrence in the input list, and parts occurring earlier
 * should always have a size greater than or equal to parts occurring later.
 *
 * Return an array of the k parts.
 *
 * Constraints:
 * The number of nodes in the list is in the range [0, 1000].
 * 0 <= Node.val <= 1000
 * 1 <= k <= 50
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
struct ListNode** splitListToParts(struct ListNode* head, int k, int* returnSize) {
    *returnSize = k;
    // 配置長度為 k 的指標陣列（注意：是存放 struct ListNode* 指標的陣列）
    struct ListNode** result = (struct ListNode**) calloc(k, sizeof(struct ListNode*));

    // 1. 計算總長度
    struct ListNode* curr = head;
    int count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }

    // 2. 計算基準長度與前幾段多 1 的名額
    int size = count / k;
    int plus = count % k; // 共有 plus 個段落要分配到 (size + 1) 個節點

    // 3. 原地切割鏈結串列（直接複用原有節點，毋須另建節點或複製陣列）
    struct ListNode* p = head;
    for (int j = 0; j < k && p != NULL; j++) {
        result[j] = p; // 當前段落的頭節點就是 p

        // 計算當前段落的長度：前 plus 段為 size + 1，其餘為 size
        int cur_part_size = size + (j < plus ? 1 : 0);

        // 往前走 cur_part_size - 1 步，抵達當前段落的尾端節點
        for (int i = 0; i < cur_part_size - 1; i++) {
            p = p->next;
        }

        // 關鍵斷鏈技巧：記錄下一段開頭，並將當前段尾端切斷指向 NULL
        struct ListNode* next_head = p->next;
        p->next = NULL;
        p = next_head;
    }

    return result;
}

/*
 * =========================================================================
 * 原始白板實作與盲點深度剖析 (Preserved User's Original Code & Blind Spots):
 * =========================================================================
 * 原始程式碼：
 * struct ListNode** splitListToParts(struct ListNode* head, int k, int* returnSize) {
 *     struct ListNode* result = (struct ListNode*) malloc(k * sizeof(struct ListNode));
 *     struct ListNode* curr = head;
 *     int count = 0;
 *     while(curr != NULL)
 *     {
 *         count++;
 *         curr = curr->next;
 *     }
 *     int size = count / k;
 *     int plus = count % k;
 *     struct ListNode* p = head;
 *     int j = 0;
 *     while(p != NULL)
 *     {
 *         if(plus > 0)
 *         {
 *             int result2[size + plus];
 *             plus--;
 *             for(int i = 0; i < size + plus; i++)
 *             {
 *                 result2[i] = p->val;
 *                 p = p->next;
 *             }
 *             result[j] = (struct ListNode**) malloc((size + plus) * sizeof(struct ListNode*));
 *             sprintf(result, );
 *         }
 *         else
 *         {
 *             int result2[size];
 *             for(int i = 0; i < size + plus; i++)
 *             {
 *                 result2[i] = p->val;
 *                 p = p->next;
 *             }
 *         }
 *     }
 *     *returnSize = k;
 *     return NULL;
 * }
 *
 * 盲點深度剖析：
 * 1. 指標陣列型別與記憶體配置認知偏差：
 *    - 函式回傳型別是 struct ListNode**（存放各段鏈結串列「頭指標」的陣列）。
 *    - 原始寫法配置：(struct ListNode*) malloc(k * sizeof(struct ListNode));
 *      這是配置了 k 個「節點實體」，而非 k 個「節點指標」。
 *    - 正確配置：(struct ListNode**) calloc(k, sizeof(struct ListNode*));
 *      使用 calloc 還能自動將所有元素初始化為 NULL，兼顧了節點數不足 k 時後續為 NULL 的要求。
 *
 * 2. 複製資料 vs 原地指標操作（In-place Slicing）：
 *    - 原始想法嘗試宣告一個 int result2[] 陣列，把節點值撈出來，再打算透過 malloc 或 sprintf 重建。
 *      在韌體與 LeetCode 實務中，這是常見的直覺盲區（將鏈結操作當成字串/陣列操作）。
 *    - 事實上完全「不需要」額外配置新節點！原有鏈結串列已經把節點連好了，
 *      我們只需找到每段的尾端節點，把尾端的 next 設為 NULL（斷鏈），
 *      並把各段的頭指標存入 result[j] 即可，時間複雜度 O(N)，空間複雜度 O(1)（不計回傳值）。
 *
 * 3. 餘數分配數學邏輯偏差：
 *    - plus = count % k 代表的是「共有 plus 個段落要多分配 1 個節點」。
 *    - 原始寫法寫成 size + plus，若 count % k = 3，第一段就會分到 size + 3 個，而非 size + 1！
 *    - 正確分配為：前 plus 段每段長度為 size + 1，其餘每段為 size。
 * =========================================================================
 */

static struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

static struct ListNode* createList(int arr[], int size) {
    if (size == 0) return NULL;
    struct ListNode* head = createNode(arr[0]);
    struct ListNode* curr = head;
    for (int i = 1; i < size; i++) {
        curr->next = createNode(arr[i]);
        curr = curr->next;
    }
    return head;
}

static void printPart(struct ListNode* head) {
    printf("[");
    while (head != NULL) {
        printf("%d", head->val);
        if (head->next != NULL) printf(" -> ");
        head = head->next;
    }
    printf("]");
}

static void printParts(struct ListNode** parts, int k) {
    printf("[\n");
    for (int i = 0; i < k; i++) {
        printf("  Part %d: ", i);
        printPart(parts[i]);
        printf("\n");
    }
    printf("]\n");
}

int main(void) {
    // Test Case 1: head = [1, 2, 3], k = 5
    // Output: [[1], [2], [3], [], []]
    int arr1[] = {1, 2, 3};
    struct ListNode* head1 = createList(arr1, 3);
    int returnSize1 = 0;
    struct ListNode** res1 = splitListToParts(head1, 5, &returnSize1);
    printf("Test 1 (List [1,2,3], k=5):\n");
    if (res1) {
        printParts(res1, returnSize1);
        free(res1);
    } else {
        printf("Returned NULL\n");
    }
    printf("\n");

    // Test Case 2: head = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10], k = 3
    // Output: [[1, 2, 3, 4], [5, 6, 7], [8, 9, 10]]
    int arr2[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    struct ListNode* head2 = createList(arr2, 10);
    int returnSize2 = 0;
    struct ListNode** res2 = splitListToParts(head2, 3, &returnSize2);
    printf("Test 2 (List 1..10, k=3):\n");
    if (res2) {
        printParts(res2, returnSize2);
        free(res2);
    } else {
        printf("Returned NULL\n");
    }

    return 0;
}
