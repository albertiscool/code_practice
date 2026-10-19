#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #86 - Partition List (分割鏈結串列)
 * 
 * 考點對照 (HackMD 群聯面試題)：
 * 1. 筆記考古第 7 題「linklist insert & array reverse」與「群聯三題」第三題鏈結串列結構指標走訪。
 * 2. 韌體面試必考指標重鏈接 (Pointer Relinking) 與虛擬頭節點 (Dummy Node) 邊界防禦。
 * 
 * 題目描述：
 * 給定一個單向鏈結串列的頭節點 head 和一個特定值 x。
 * 請重新排列鏈結串列，使得「所有小於 x 的節點都出現在大於或等於 x 的節點之前」。
 * 你必須「保留」兩個分區內每個節點的「原始相對順序」。
 * 
 * 限制條件：
 * 鏈結串列中的節點數在範圍 [0, 200] 內
 * -100 <= Node.val <= 100
 * -200 <= x <= 200
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * -------------------------------------------------------------------------
 * struct ListNode* partition(struct ListNode* head, int x) {
 *     // 請於此處實作純白板解法
 *     struct ListNode dummy;
 *     dummy.next = head;
 *     struct ListNode* curr = head;
 *     struct ListNode* pre1 = &dummy;
 *     struct ListNode* pre2 = &dummy;
 *     while(curr != NULL)
 *     {
 *         if(curr->val >= x)
 *         {
 *             curr = curr->next;
 *             pre2 = pre2->next;
 *         }
 *         else
 *         {
 *             struct ListNode* next_node = curr->next;
 *             struct ListNode* pre2_next = pre2->next;
 *             curr->next = pre1->next;
 *             pre1->next = curr;
 *             pre2->next = next_node;
 *             curr = next_node;
 *             pre2 = pre2_next;
 *         }
 *     }
 *     return dummy.next;
 * }
 * -------------------------------------------------------------------------
 * 【原始盲點剖析】：
 * 1. 致命自我成環 (Self-Cycle / Memory Loop) 導致 TLE：
 *    - 當一開始 `curr->val < x` 進入 else 分支時：
 *      `pre1` 指向 `&dummy`，而 `pre1->next` 正是 `curr`。
 *      執行 `curr->next = pre1->next;` 直接使 `curr->next = curr`（自己指向自己），
 *      瞬間形成 1 個節點的死循環，導致後續走訪死鎖（LeetCode 報 Time Limit Exceeded）。
 * 2. 指標未推進與相對順序顛倒：
 *    - `pre1` 在插入小於節點後從未向後移動（`pre1 = curr`），
 *      若有多個小於 x 的節點，會變成頭部逆序插入，破壞題目「保留原始相對順序」的硬性要求。
 * 3. 韌體標準工業解法 ——「雙虛擬頭分流法 (Two Dummy Nodes Demux)」：
 *    - 建立兩個 Stack 虛擬頭：`lessHead` 與 `greaterHead`。
 *    - 走訪一次，將節點分別掛載到兩條串列尾端（天然維持 FIFO 相對順序）。
 *    - 【關鍵陷阱】：務必執行 `greater->next = NULL;` 截斷原始殘留指標，杜絕環狀死循環。
 *    - 最後直接串接 `less->next = greaterHead.next;`，兼顧 O(1) 額外空間與零錯誤率！
 * =========================================================================
 */

struct ListNode* partition(struct ListNode* head, int x) {
    // 建立兩個虛擬頭節點（配置於 Stack，0 malloc 開銷）
    struct ListNode lessHead;
    struct ListNode greaterHead;
    struct ListNode* less = &lessHead;
    struct ListNode* greater = &greaterHead;

    lessHead.next = NULL;
    greaterHead.next = NULL;

    struct ListNode* curr = head;

    // 單趟走訪分流
    while (curr != NULL) {
        if (curr->val < x) {
            less->next = curr;
            less = less->next;
        } else {
            greater->next = curr;
            greater = greater->next;
        }
        curr = curr->next;
    }

    // 致命關鍵防禦：斷開大於等於串列的最後一個節點，防止出現環狀死循環 (Cycle)
    greater->next = NULL;

    // 縫合兩條串列
    less->next = greaterHead.next;

    return lessHead.next;
}

// 輔助測試函式
struct ListNode* createNode(int val) {
    struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
    node->val = val;
    node->next = NULL;
    return node;
}

void printList(struct ListNode* head) {
    struct ListNode* curr = head;
    while (curr) {
        printf("%d -> ", curr->val);
        curr = curr->next;
    }
    printf("NULL\n");
}

int main() {
    // 測試範例 1: [1, 4, 3, 2, 5, 2], x = 3
    struct ListNode* head = createNode(1);
    head->next = createNode(4);
    head->next->next = createNode(3);
    head->next->next->next = createNode(2);
    head->next->next->next->next = createNode(5);
    head->next->next->next->next->next = createNode(2);

    printf("Original: ");
    printList(head);

    struct ListNode* res = partition(head, 3);
    printf("Partitioned (x = 3): ");
    printList(res);
    printf("Expected: 1 -> 2 -> 2 -> 4 -> 3 -> 5 -> NULL\n");

    return 0;
}
