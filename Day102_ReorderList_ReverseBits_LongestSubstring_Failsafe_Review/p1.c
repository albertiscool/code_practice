#include <stdio.h>
#include <stdlib.h>

/**
 * ============================================================================
 * 題目一：重排鏈結串列 (LeetCode #143 - Reorder List)
 * ============================================================================
 * 【難度頻率】一線 IC 設計廠（瑞昱、聯詠、聯發科）高頻指標綜合題
 * 
 * 【題目說明】
 * 給定一個單向鏈結串列 L: L0 -> L1 -> … -> Ln-1 -> Ln
 * 請在「原地 (In-place)」將其重新排列為：
 * L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 -> …
 * 
 * 【限制條件】
 * 1. 不能只是修改節點內部的數值 (val)，必須實際重新串接節點指標 (next)。
 * 2. 空間複雜度建議為 O(1) 原地操作。
 * 
 * 【範例 1】
 * 輸入: head = [1, 2, 3, 4]
 * 輸出: [1, 4, 2, 3]
 * 
 * 【範例 2】
 * 輸入: head = [1, 2, 3, 4, 5]
 * 輸出: [1, 5, 2, 4, 3]
 * 
 * 【極限邊界測試】
 * 1. head == NULL 或只有 1 個節點：不需做任何更動。
 * 2. 只有 2 個節點 [1, 2]：結果依然是 [1, 2]。
 * ============================================================================
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

void reorderList(struct ListNode* head) {
    if (head == NULL || head->next == NULL || head->next->next == NULL) {
        return;
    }

    // 步驟 1：快慢指標找中點（讓 slow 停在前半段的最後一個節點）
    struct ListNode *slow = head;
    struct ListNode *fast = head;
    while (fast->next != NULL && fast->next->next != NULL) {
        slow = slow->next;
        fast = fast->next->next;
    }

    // 步驟 2：切斷前後兩半，並反轉後半段串列
    struct ListNode *second = slow->next;
    slow->next = NULL; // 核心關鍵：徹底斷開，避免成環！

    struct ListNode *prev = NULL;
    struct ListNode *curr = second;
    while (curr != NULL) {
        struct ListNode *nxt = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nxt;
    }
    // 反轉完成後，prev 即為後半段翻轉後的頭節點

    // 步驟 3：交錯合併 (Zip / Interleave) 兩條獨立鏈結串列
    struct ListNode *p1 = head;
    struct ListNode *p2 = prev;
    while (p2 != NULL) {
        struct ListNode *p1_next = p1->next;
        struct ListNode *p2_next = p2->next;

        p1->next = p2;
        p2->next = p1_next;

        p1 = p1_next;
        p2 = p2_next;
    }
}

/* ================= 測試輔助函式 ================= */
static struct ListNode* createList(int* arr, int size) {
    if (size == 0) return NULL;
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;
    struct ListNode* tail = head;
    for (int i = 1; i < size; i++) {
        tail->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        tail = tail->next;
        tail->val = arr[i];
        tail->next = NULL;
    }
    return head;
}

static void printList(struct ListNode* head) {
    printf("[");
    while (head != NULL) {
        printf("%d%s", head->val, head->next ? ", " : "");
        head = head->next;
    }
    printf("]\n");
}

static void freeList(struct ListNode* head) {
    while (head != NULL) {
        struct ListNode* temp = head;
        head = head->next;
        free(temp);
    }
}

int main(void) {
    printf("=== Day 102 P1: Reorder List 驗證 ===\n");
    
    // 測試 1：偶數長度 [1, 2, 3, 4] -> [1, 4, 2, 3]
    int arr1[] = {1, 2, 3, 4};
    struct ListNode* l1 = createList(arr1, 4);
    printf("測試 1 原始: "); printList(l1);
    reorderList(l1);
    printf("測試 1 結果: "); printList(l1);

    // 測試 2：奇數長度 [1, 2, 3, 4, 5] -> [1, 5, 2, 4, 3]
    int arr2[] = {1, 2, 3, 4, 5};
    struct ListNode* l2 = createList(arr2, 5);
    printf("測試 2 原始: "); printList(l2);
    reorderList(l2);
    printf("測試 2 結果: "); printList(l2);

    // 測試 3：長度 2 [1, 2] -> [1, 2]
    int arr3[] = {1, 2};
    struct ListNode* l3 = createList(arr3, 2);
    printf("測試 3 原始: "); printList(l3);
    reorderList(l3);
    printf("測試 3 結果: "); printList(l3);

    freeList(l1);
    freeList(l2);
    freeList(l3);
    return 0;
}

/*
 * ============================================================================
 * 【原始白板盲點複盤 (Original Blind Spots)】
 * ============================================================================
 * 盲點 1：NULL 指標解引用 (Fatal Crash)
 *   原始寫法：
 *     struct ListNode* pre = NULL;
 *     pre->next = head; // 致命錯誤！對 NULL 取 ->next 會直接觸發 Segmentation Fault。
 * 
 * 盲點 2：未「切斷 (Cut)」前後兩半串列
 *   原始寫法中 slow 與 fast 走完後，前半段尾巴沒有補上 `slow->next = NULL`。
 *   鏈結串列在反轉或穿插時，若沒有徹底斷開，指標會互相指涉形成循環迴圈 (Cycle) 導致死機。
 * 
 * 盲點 3：穿插合併時的雙指標遺失
 *   原始在做 p1、p2 交錯時，試圖用原地拔除插頭，但因為沒有同時備份 p1->next 與 p2->next，
 *   導致 p2 移動時節點丟失且陷入無窮迴圈。
 *   正解模式：三步驟經典模組化
 *     (1) 快慢指標找中點 (fast->next && fast->next->next)
 *     (2) 斷開 (slow->next = NULL) 並反轉後半 (prev, curr, nxt 經典三指標)
 *     (3) 雙指針拉鍊式穿插 (同時備份 p1_next, p2_next 後交叉串接)
 * ============================================================================
 */
