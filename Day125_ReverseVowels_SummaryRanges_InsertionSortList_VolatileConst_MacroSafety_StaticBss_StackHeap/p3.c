#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #147 - Insertion Sort List
 * 
 * 題目描述：
 * 給定一個單向鏈結串列的頭節點 head，請使用「插入排序（Insertion Sort）」對該鏈結串列進行排序，並回傳排序後的鏈結串列頭節點。
 * 
 * 插入排序步驟：
 * 1. 插入排序每次只將一個元素插入到已排序好的子串列適當位置中。
 * 2. 每次迭代取下一個待排序的節點，在已排序子串列中尋找合適的插入點將其接入。
 * 3. 重複此過程，直到所有節點皆完成排序。
 * 
 * 範例 1：
 * 輸入：head = [4, 2, 1, 3]
 * 輸出：[1, 2, 3, 4]
 * 
 * 範例 2：
 * 輸入：head = [-1, 5, 3, 4, 0]
 * 輸出：[-1, 0, 3, 4, 5]
 * 
 * 限制條件：
 * 串列中的節點數在範圍 [1, 5000] 內
 * -5000 <= Node.val <= 5000
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
 * 【LeetCode 官方提交成績】：
 * - 狀態：Accepted (通過 19/19 測試案例)
 * - 執行時間：2 ms
 * - 記憶體：11.5 MB
 * - Submission ID：2180262866
 * -------------------------------------------------------------------------
 * 【使用者原始白板程式碼】：
 * struct ListNode* insertionSortList(struct ListNode* head) {
 *     struct ListNode* c = head;
 *     int count = 0;
 *     while(c != NULL) { count++; c = c->next; }
 *     struct ListNode dummy;
 *     dummy.next = head;
 *     for(int i = 0; i < count; i++)
 *     {
 *         struct ListNode* tail = &dummy;
 *         struct ListNode* p = head;
 *         for(int j = 0; j < i; j++) { tail = tail->next; p = p->next; }
 *         struct ListNode* pre = &dummy;
 *         struct ListNode* curr = head;
 *         for(int k = 0; k < i; k++)
 *         {
 *             if(p->val <= curr->val)
 *             {
 *                 struct ListNode* next_node = p->next;
 *                 pre->next = p;
 *                 p->next = curr;
 *                 tail->next = next_node;
 *                 // 盲點 1：插入後未 break，造成迴圈繼續對已被改動的節點重複接線
 *             }
 *             else { pre = pre->next; curr = curr->next; }
 *         }
 *     }
 *     return dummy.next;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 鏈結串列形成環（Cycle / 死迴圈 TLE）：
 *    - 當 p 插入到 pre 與 curr 之間後，指標結構已經改變，但最內層 for(k) 迴圈沒有 break。
 *    - 後續步驟再次移動或修改指標時，會讓節點互相指向，形成閉環（Cycle），引發無窮走訪！
 * 2. 靜態 head 指標失效（Pointer Stale）：
 *    - 程式中多次依賴 p = head 和 curr = head 從頭出發。
 *    - 但一旦有節點插入到原本 head 的前面，真正的表頭已經變成了 dummy.next，
 *      繼續以舊 head 當作起點走訪，會跳過已被排至最前方的節點！
 * 3. 鏈結串列插入排序的標準優雅解法（雙指標維護）：
 *    - 維護 lastSorted（已排序部分的尾端）與 curr（當前待排序節點）。
 *    - 若 lastSorted->val <= curr->val：說明 curr 本來就在正確位置，直接 lastSorted = lastSorted->next 推進。
 *    - 若 lastSorted->val > curr->val：從 dummy 從頭搜尋插入位置 prev，
 *      僅需 3 條指標賦值即可將 curr 移至 prev 之後，既無環、無溢位，又極簡優雅！
 * =========================================================================
 */

struct ListNode* insertionSortList(struct ListNode* head) {
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // 計算串列總長度
    struct ListNode* c = head;
    int count = 0;
    while (c != NULL) {
        count++;
        c = c->next;
    }

    struct ListNode dummy;
    dummy.next = head;

    // 從第 1 個元素開始（第 0 個元素視為長度為 1 的已排序子串列）
    for (int i = 1; i < count; i++) {
        // 找到第 i 個節點 p 以及其前一個節點 tail（從 dummy 開始走訪 i 步）
        struct ListNode* tail = &dummy;
        for (int j = 0; j < i; j++) {
            tail = tail->next;
        }
        struct ListNode* p = tail->next;

        // 在前 i 個已排序節點中尋找適合 p 插入的位置
        struct ListNode* pre = &dummy;
        struct ListNode* curr = dummy.next; // 【修正 1】：從當前真正的表頭 dummy.next 開始

        for (int k = 0; k < i; k++) {
            if (p->val <= curr->val) {
                // 【修正 2】：將 p 移至 pre 與 curr 之間
                struct ListNode* next_node = p->next;
                tail->next = next_node;
                pre->next = p;
                p->next = curr;
                break; // 【修正 3】：插入完成後必須立刻 break，防止閉環死鎖！
            } else {
                pre = pre->next;
                curr = curr->next;
            }
        }
    }

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
        printf("%d", curr->val);
        if (curr->next) printf(" -> ");
        curr = curr->next;
    }
    printf("\n");
}

int main() {
    int a1[] = {4, 2, 1, 3};
    struct ListNode* l1 = createList(a1, 4);
    printf("Original: "); printList(l1);
    struct ListNode* res1 = insertionSortList(l1);
    printf("Sorted:   "); printList(res1);
    printf("Expected: 1 -> 2 -> 3 -> 4\n\n");

    int a2[] = {-1, 5, 3, 4, 0};
    struct ListNode* l2 = createList(a2, 5);
    printf("Original: "); printList(l2);
    struct ListNode* res2 = insertionSortList(l2);
    printf("Sorted:   "); printList(res2);
    printf("Expected: -1 -> 0 -> 3 -> 4 -> 5\n");

    return 0;
}
