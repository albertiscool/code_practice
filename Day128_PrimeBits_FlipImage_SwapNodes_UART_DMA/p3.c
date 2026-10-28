#include <stdio.h>
#include <stdlib.h>

/*
 * LeetCode #1721 - Swapping Nodes in a Linked List
 *
 * Description:
 * You are given the head of a linked list, and an integer k.
 *
 * Return the head of the linked list after swapping the values of the kth node
 * from the beginning and the kth node from the end (the list is 1-indexed).
 *
 * Constraints:
 * The number of nodes in the list is n.
 * 1 <= k <= n <= 10^5
 * 0 <= Node.val <= 100
 *
 * Example 1:
 * Input: head = [1,2,3,4,5], k = 2
 * Output: [1,4,3,2,5]
 *
 * Example 2:
 * Input: head = [7,9,6,6,7,8,3,0,9,5], k = 5
 * Output: [7,9,6,6,8,7,3,0,9,5]
 */

struct ListNode {
    int val;
    struct ListNode *next;
};

struct ListNode* swapNodes(struct ListNode* head, int k) {
    struct ListNode dummy;
    dummy.next = head;
    struct ListNode* tail = head;
    struct ListNode* right = head;
    struct ListNode* left = head;
    for(int i = 0; i < k - 1; i++)
    {
        left = left->next;
        tail = tail->next;
    }
    while(tail->next != NULL)
    {
        tail = tail->next;
        right = right->next;
    }
    int temp = right->val;
    right->val = left->val;
    left->val = temp;
    return dummy.next;
}

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

static void printList(struct ListNode* head) {
    printf("[");
    while (head) {
        printf("%d%s", head->val, head->next ? "," : "");
        head = head->next;
    }
    printf("]\n");
}

static void freeList(struct ListNode* head) {
    while (head) {
        struct ListNode* tmp = head;
        head = head->next;
        free(tmp);
    }
}

int main() {
    // Test 1: [1,2,3,4,5], k = 2
    int a1[] = {1, 2, 3, 4, 5};
    struct ListNode* l1 = createList(a1, 5);
    struct ListNode* res1 = swapNodes(l1, 2);
    printf("Test 1 Result: ");
    printList(res1);
    printf("Expected:      [1,4,3,2,5]\n");
    freeList(res1);

    // Test 2: [7,9,6,6,7,8,3,0,9,5], k = 5
    int a2[] = {7, 9, 6, 6, 7, 8, 3, 0, 9, 5};
    struct ListNode* l2 = createList(a2, 10);
    struct ListNode* res2 = swapNodes(l2, 5);
    printf("Test 2 Result: ");
    printList(res2);
    printf("Expected:      [7,9,6,6,8,7,3,0,9,5]\n");
    freeList(res2);

    // Test 3: [1], k = 1
    int a3[] = {1};
    struct ListNode* l3 = createList(a3, 1);
    struct ListNode* res3 = swapNodes(l3, 1);
    printf("Test 3 Result: ");
    printList(res3);
    printf("Expected:      [1]\n");
    freeList(res3);

    return 0;
}
