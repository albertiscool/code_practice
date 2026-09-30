#include <stdio.h>
#include <stdlib.h>

/**
 * ============================================================================
 * 【Day 103 - 題目三】反轉鏈結串列 II (LeetCode #92 - Reverse Linked List II)
 * 【LeetCode 難度】Medium (中等)
 * ============================================================================
 * 【題目說明】
 * 給定一個單向鏈結串列的頭節點 head，以及兩個整數 left 和 right（其中 left <= right）。
 * 請你反轉從位置 left 到位置 right 的鏈結串列節點，並回傳反轉後的鏈結串列。
 * 節點編號由 1 開始起算。
 * 
 * 【範例 1】
 * 輸入: head = [1, 2, 3, 4, 5], left = 2, right = 4
 * 輸出: [1, 4, 3, 2, 5]
 * 
 * 【範例 2】
 * 輸入: head = [5], left = 1, right = 1
 * 輸出: [5]
 * 
 * 【限制條件】
 * 1. 鏈結串列的節點數在範圍 [1, 500] 內。
 * 2. -500 <= Node.val <= 500
 * 3. 1 <= left <= right <= 節點總數。
 * 4. 必須在原地 (In-place) 完成，空間複雜度為 O(1)。
 * ============================================================================
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* reverseBetween(struct ListNode* head, int left, int right) {
    if (head == NULL || left == right || head->next == NULL) {
        return head;
    }

    // 哨兵節點 (Dummy Node)，用來優雅防禦 left == 1 (頭節點被反轉) 的特例
    struct ListNode dummy;
    dummy.val = 0;
    dummy.next = head;

    struct ListNode* pre = &dummy;

    // 盲點修正：pre 只需要從 dummy 往前走 (left - 1) 步，
    // 即可精準停在「待反轉區間的前一個節點 (anchor)」！
    for (int i = 0; i < left - 1; i++) {
        pre = pre->next;
    }

    // curr 為待反轉區間的起點 (反轉完成後會變成該區間的尾巴)
    struct ListNode* curr = pre->next;

    // 頭插法 (Head Insertion)：每一次把 curr 後面的節點拔除，插到 pre 後面
    // 總共需要搬動 (right - left) 個節點
    for (int j = 0; j < right - left; j++) {
        struct ListNode* next_node = curr->next;
        curr->next = next_node->next;
        next_node->next = pre->next;
        pre->next = next_node;
    }

    return dummy.next;
}

/* ================= 測試輔助函式 ================= */
static struct ListNode* createList(int* arr, int size) {
    if (size == 0) return NULL;
    struct ListNode* head = (struct ListNode*)malloc(sizeof(struct ListNode));
    head->val = arr[0];
    head->next = NULL;
    struct ListNode* tail = head;
    for (int i = 1; i < size; i++) {
        struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
        node->val = arr[i];
        node->next = NULL;
        tail->next = node;
        tail = node;
    }
    return head;
}

static void printList(struct ListNode* head) {
    printf("[");
    while (head != NULL) {
        printf("%d%s", head->val, head->next ? " -> " : "");
        head = head->next;
    }
    printf("]\n");
}

static void freeList(struct ListNode* head) {
    while (head != NULL) {
        struct ListNode* tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main(void) {
    printf("=== Day 103 P3: Reverse Linked List II 驗證 ===\n");

    // 測試 1: [1, 2, 3, 4, 5], left = 2, right = 4 -> [1, 4, 3, 2, 5]
    int a1[] = {1, 2, 3, 4, 5};
    struct ListNode* l1 = createList(a1, 5);
    printf("測試 1 原始: "); printList(l1);
    l1 = reverseBetween(l1, 2, 4);
    printf("測試 1 結果: "); printList(l1);
    printf("測試 1 預期: [1 -> 4 -> 3 -> 2 -> 5]\n\n");
    freeList(l1);

    // 測試 2: [5], left = 1, right = 1 -> [5]
    int a2[] = {5};
    struct ListNode* l2 = createList(a2, 1);
    l2 = reverseBetween(l2, 1, 1);
    printf("測試 2 結果: "); printList(l2);
    printf("測試 2 預期: [5]\n\n");
    freeList(l2);

    // 測試 3: 極限頭部反轉 [3, 5], left = 1, right = 2 -> [5, 3]
    int a3[] = {3, 5};
    struct ListNode* l3 = createList(a3, 2);
    printf("測試 3 原始: "); printList(l3);
    l3 = reverseBetween(l3, 1, 2);
    printf("測試 3 結果: "); printList(l3);
    printf("測試 3 預期: [5 -> 3]\n\n");
    freeList(l3);

    return 0;
}

/*
 * ============================================================================
 * 🔍 使用者原始白板程式碼存檔 (Original Implementation Archive)
 * ============================================================================
 * 【原始完整程式碼】：
 * struct ListNode* reverseBetween(struct ListNode* head, int left, int right) {
 *     if(head == NULL)
 *     {
 *         return NULL;
 *     }
 *     if(left == right || head->next == NULL)
 *     {
 *         return head;
 *     }
 * 
 *     struct ListNode dummy;
 *     dummy.next = head;
 *     struct ListNode* pre = &dummy;
 *     struct ListNode* curr = head;
 *     for(int i = 0; i < left; i++)
 *     {
 *         pre = pre->next;
 *         curr = curr->next;
 *     }
 * 
 *     for(int j = 0; j < right - left; j++)
 *     {
 *         struct ListNode* next_node = curr->next;
 *         curr->next = next_node->next;
 *         next_node->next = pre->next;
 *         pre->next = next_node;
 *     }
 * 
 *     return dummy.next;
 * }
 * 
 * ============================================================================
 * 【原始白板盲點複盤 (Original Blind Spots)】
 * ============================================================================
 * 盲點：指針定位步數多走了一步 (Off-By-One Pointer Placement Bug)
 *   原始寫法：
 *     for (int i = 0; i < left; i++) {
 *         pre = pre->next;
 *         curr = curr->next;
 *     }
 *   問題：
 *     - pre 從 dummy 開始出發，要停在「待反轉區間的前一個節點」，只需要走 (left - 1) 步！
 *     - 若迴圈跑 left 次 (i < left)：
 *       以 left = 2 為例，pre 走了 2 步停在第 2 個節點 (節點 2)，curr 走了 2 步停在第 3 個節點 (節點 3)。
 *       但節點 2 本身才是待反轉的起點，導致 pre 與 curr 全部多踏了一步，頭插反轉範圍完全位移！
 *   正解：
 *     只需要讓 pre 走 `left - 1` 步：
 *       for (int i = 0; i < left - 1; i++) pre = pre->next;
 *       curr = pre->next;
 *     後續的 4 行頭插法反轉寫得極為標準精準！
 * ============================================================================
 */
