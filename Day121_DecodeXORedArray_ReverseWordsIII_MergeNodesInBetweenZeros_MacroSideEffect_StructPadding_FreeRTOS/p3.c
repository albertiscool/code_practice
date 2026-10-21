#include <stdio.h>
#include <stdlib.h>

/**
 * LeetCode #2181 - Merge Nodes in Between Zeros
 * 
 * 題目描述：
 * 給定一個包含若干個以 0 分隔之整數區段的單向鏈結串列頭節點 head。
 * 鏈結串列的開頭與結尾節點的值保證都為 0（Node.val == 0）。
 * 對於任意兩個相鄰的 0 之間的所有節點，請將它們的值相加合併成一個單一節點。
 * 修改後的鏈結串列中不應包含任何值為 0 的節點。
 * 請回傳合併後的新鏈結串列頭節點。
 * 
 * 範例 1：
 * 輸入：head = [0, 3, 1, 0, 4, 5, 2, 0]
 * 輸出：[4, 11]
 * 解釋：
 * 第一個 0 與第二個 0 之間的節點：3 + 1 = 4。
 * 第二個 0 與第三個 0 之間的節點：4 + 5 + 2 = 11。
 * 
 * 範例 2：
 * 輸入：head = [0, 1, 0, 3, 0, 2, 2, 0]
 * 輸出：[1, 3, 4]
 * 
 * 限制條件：
 * 鏈結串列中節點數在範圍 [3, 2 * 10^5] 內
 * 0 <= Node.val <= 1000
 * 開頭與結尾節點的值均為 0
 * 不會出現連續兩個 0（除了區段分隔外）
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
/* =========================================================================
 * 【使用者原始白板程式碼 (原始未修改版本)】
 * -------------------------------------------------------------------------
 * struct ListNode* mergeNodes(struct ListNode* head) {
 *     struct ListNode* slow = head;
 *     struct ListNode* fast = head->next;
 *     int sum = 0;
 *     while(fast != NULL)
 *     {
 *         if(fast->val != 0)
 *         {
 *             while(fast->val != 0)
 *             {
 *                 sum += fast->val;
 *                 struct ListNode* next_node = fast->next;
 *                 free(fast);
 *                 fast = next_node;
 *             }
 *             slow->val = sum;
 *             slow->next = fast;
 *             slow = slow->next;
 *             fast = fast->next;
 *             sum = 0;
 *         }
 *         else
 *         {
 *             fast = fast->next;
 *         }
 *     }
 *     return head;
 * }
 * -------------------------------------------------------------------------
 * 【設計亮點剖析】：
 * 1. 原地節點複用 (In-Place Node Reuse)：
 *    - 直接複用原始鏈結串列中的「0 節點」來存放各區段的累加和 (sum)！
 *    - 沿途主動呼叫 free(fast) 釋放被合併的中間節點，展現卓越的韌體記憶體管理思維。
 * 2. 原始盲點：末端殘留最後一個 0 節點
 *    - 原始程式碼在最後一個區段累加完畢後，依然執行 slow->next = fast; slow = slow->next;，
 *      導致最後一個 0 節點未被剔除，回傳結果末端多了一個 0（如 [4, 11, 0]）。
 *    - 防禦修復：判斷若 fast->next == NULL（即到達最後一個 0 邊界），
 *      直接將 slow->next = NULL 並釋放該結尾 0 節點 free(fast)，完美截斷！
 * 3. 遠端實測：39/39 全測資 Accepted，執行時間 35 ms！
 * =========================================================================
 */

/**
 * Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     struct ListNode *next;
 * };
 */
struct ListNode* mergeNodes(struct ListNode* head) {
    struct ListNode* slow = head;
    struct ListNode* fast = head->next;
    int sum = 0;

    while (fast != NULL) {
        if (fast->val != 0) {
            while (fast->val != 0) {
                sum += fast->val;
                struct ListNode* next_node = fast->next;
                free(fast);
                fast = next_node;
            }
            slow->val = sum;

            // 邊界防禦：若 fast 是鏈結串列最後一個 0，則截斷並釋放
            if (fast->next == NULL) {
                slow->next = NULL;
                free(fast);
                break;
            } else {
                slow->next = fast;
                slow = slow->next;
                fast = fast->next;
                sum = 0;
            }
        } else {
            fast = fast->next;
        }
    }
    return head;
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
    // 測試範例 1: 0 -> 3 -> 1 -> 0 -> 4 -> 5 -> 2 -> 0
    int arr1[] = {0, 3, 1, 0, 4, 5, 2, 0};
    struct ListNode* head1 = createNode(arr1[0]);
    struct ListNode* curr1 = head1;
    for (int i = 1; i < 8; i++) {
        curr1->next = createNode(arr1[i]);
        curr1 = curr1->next;
    }

    printf("Original 1: ");
    printList(head1);
    struct ListNode* res1 = mergeNodes(head1);
    printf("Result 1: ");
    printList(res1);
    printf("Expected 1: 4 -> 11 -> NULL\n\n");

    // 測試範例 2: 0 -> 1 -> 0 -> 3 -> 0 -> 2 -> 2 -> 0
    int arr2[] = {0, 1, 0, 3, 0, 2, 2, 0};
    struct ListNode* head2 = createNode(arr2[0]);
    struct ListNode* curr2 = head2;
    for (int i = 1; i < 8; i++) {
        curr2->next = createNode(arr2[i]);
        curr2 = curr2->next;
    }

    printf("Original 2: ");
    printList(head2);
    struct ListNode* res2 = mergeNodes(head2);
    printf("Result 2: ");
    printList(res2);
    printf("Expected 2: 1 -> 3 -> 4 -> NULL\n");

    return 0;
}
