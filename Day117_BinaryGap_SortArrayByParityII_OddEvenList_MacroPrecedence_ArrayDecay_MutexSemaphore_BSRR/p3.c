#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>

/**
 * LeetCode #328: Odd Even Linked List
 * 
 * 題目描述：
 * 給定一個單向鏈結串列的頭節點 head，請將所有「奇數節點 (odd index)」排在前面，
 * 緊接著所有「偶數節點 (even index)」，並返回重新排列後的鏈結串列。
 * 
 * 注意：
 * - 這裡指的是「節點的序號 (1-indexed)」，第 1 個節點為奇數節點，第 2 個節點為偶數節點，以此類推。
 *   （並非指節點內 val 的數值奇偶性！）
 * - 奇數節點內部與偶數節點內部的「相對順序」必須保持與輸入時完全一致。
 * - 你必須在「時間複雜度 O(N)」且「額外空間複雜度 O(1)」的條件下完成原地重新串接。
 * 
 * 限制條件：
 * - 節點數在範圍 [0, 10^4] 內
 * - -10^6 <= Node.val <= 10^6
 * 
 * 範例 1:
 *   輸入: head = [1,2,3,4,5]
 *   輸出: [1,3,5,2,4]
 * 
 * 範例 2:
 *   輸入: head = [2,1,3,5,6,4,7]
 *   輸出: [2,3,6,7,1,5,4]
 * 
 * 範例 3:
 *   輸入: head = []
 *   輸出: []
 * 
 * 範例 4:
 *   輸入: head = [1]
 *   輸出: [1]
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* oddEvenList(struct ListNode* head) {
    // 邊界特判：空鏈結或僅有單一節點，無須重排
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // 【雙鏈分離跳步縫合法 (Leapfrog Split & Stitch)】
    // odd 指向奇數鏈起點 (第 1 節點)
    // even 指向偶數鏈起點 (第 2 節點)
    // evenHead 保留偶數鏈頭，供最後拼接使用
    struct ListNode* odd = head;
    struct ListNode* even = head->next;
    struct ListNode* evenHead = even;

    while (even != NULL && even->next != NULL) {
        odd->next = even->next;   // 奇數節點跳接下一個奇數節點
        odd = odd->next;          // 推進奇數指標
        even->next = odd->next;   // 偶數節點跳接下一個偶數節點
        even = even->next;        // 推進偶數指標
    }

    // 將整條偶數鏈結無縫縫合在奇數鏈結的尾端
    odd->next = evenHead;

    return head;
}

/*
 * =====================================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * =====================================================================================
 * struct ListNode* oddEvenList(struct ListNode* head) {
 *     if(head == NULL) return head;
 *     struct ListNode* tail = head;
 *     while(tail->next != NULL)
 *     {
 *         tail = tail->next;
 *     }
 *     struct ListNode* pre = head;
 *     struct ListNode* curr = head->next;
 *     while(curr != NULL && curr->next != NULL)
 *     {
 *         struct ListNode* next_node = curr->next;
 *         tail->next = curr;
 *         pre->next = next_node;
 *         curr = next_node->next;
 *         pre = next_node;
 *     }
 *     return head;
 * }
 * =====================================================================================
 * 【面試盲點剖析 & 鏈結串列環狀死結精華】 (LeetCode #328: Odd Even Linked List)
 * =====================================================================================
 * 
 * 1. 致命環狀死結與無窮迴圈 (Cycle Formation / Infinite Loop)：
 *    - 原始思維直覺：先遍歷找到原始尾端 tail，然後在遍歷過程中試圖把偶數節點 curr 拔下來掛到 tail 後面。
 *    - 踩雷細節：
 *      在 `tail->next = curr;` 時（例如節點 5 的 next 指向節點 2），
 *      此時 curr (節點 2) 的 `next` 仍然指向節點 3！
 *      這導致尾巴 5 連到 2，2 連到 3，3 連到 4，4 又連回 5，瞬間在鏈結中形成了嚴重的【閉環 (Cycle)】！
 *      在後續遍歷或斷言比對時，程式會陷入死循環無法退出。
 * 
 * 2. 尾端指標 tail 未同步推進：
 *    - 在將節點接上 tail 之後，缺少了 `tail = curr;` 與 `tail->next = NULL;`。
 *    - 下一輪迴圈若再執行 `tail->next = curr`，會直接把上一輪接在 tail 後面的節點給覆蓋沖掉，造成節點遺失。
 * 
 * 3. 業界標準滿分思維：【雙鏈交替跳步縫合法 (Leapfrog Split & Stitch)】：
 *    - 不用預先找尾巴，也不用動態在尾巴 append！
 *    - 我們直接開闢兩條獨立鏈條：
 *      * `odd` 串起第 1, 3, 5... 節點
 *      * `even` 串起第 2, 4, 6... 節點
 *      * 用一個常數指標 `evenHead` 記住偶數鏈的起點 (第 2 節點)。
 *    - 迴圈中奇偶交替跳步：
 *      `odd->next = even->next; odd = odd->next;`
 *      `even->next = odd->next; even = even->next;`
 *    - 迴圈結束後，只需優雅的一行：`odd->next = evenHead;`，
 *      直接將偶數鏈頭拼在奇數鏈尾，100% 保證無環、0 節點丟失、O(1) 空間、O(N) 單趟遍歷！
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
    printf("=== Test P3: Odd Even Linked List ===\n");

    // Test 1: [1, 2, 3, 4, 5] -> [1, 3, 5, 2, 4]
    int a1[] = {1, 2, 3, 4, 5};
    int exp1[] = {1, 3, 5, 2, 4};
    struct ListNode* l1 = createList(a1, 5);
    l1 = oddEvenList(l1);
    assert(compareList(l1, exp1, 5));
    freeList(l1);
    printf("Test 1 Passed: [1, 2, 3, 4, 5] -> [1, 3, 5, 2, 4]\n");

    // Test 2: [2, 1, 3, 5, 6, 4, 7] -> [2, 3, 6, 7, 1, 5, 4]
    int a2[] = {2, 1, 3, 5, 6, 4, 7};
    int exp2[] = {2, 3, 6, 7, 1, 5, 4};
    struct ListNode* l2 = createList(a2, 7);
    l2 = oddEvenList(l2);
    assert(compareList(l2, exp2, 7));
    freeList(l2);
    printf("Test 2 Passed: [2, 1, 3, 5, 6, 4, 7] -> [2, 3, 6, 7, 1, 5, 4]\n");

    // Test 3: [] -> []
    struct ListNode* l3 = NULL;
    l3 = oddEvenList(l3);
    assert(l3 == NULL);
    printf("Test 3 Passed: [] -> []\n");

    // Test 4: [1] -> [1]
    int a4[] = {1};
    int exp4[] = {1};
    struct ListNode* l4 = createList(a4, 1);
    l4 = oddEvenList(l4);
    assert(compareList(l4, exp4, 1));
    freeList(l4);
    printf("Test 4 Passed: [1] -> [1]\n");

    // Test 5: [1, 2] -> [1, 2]
    int a5[] = {1, 2};
    int exp5[] = {1, 2};
    struct ListNode* l5 = createList(a5, 2);
    l5 = oddEvenList(l5);
    assert(compareList(l5, exp5, 2));
    freeList(l5);
    printf("Test 5 Passed: [1, 2] -> [1, 2]\n");

    printf("All P3 Local Tests Passed!\n");
    return 0;
}
