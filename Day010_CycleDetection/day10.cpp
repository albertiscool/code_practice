#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node_t;

// --- Core Interview Question: Detect Cycle in Linked List ---
bool has_cycle(Node_t *head) {
    if (head == NULL || head->next == NULL) {
        return false; // An empty list or single node cannot have a cycle
    }

    Node_t *slow = head;
    Node_t *fast = head;

    while (fast != NULL && fast->next != NULL) {
        slow = slow->next;          // Moves 1 step
        fast = fast->next->next;    // Moves 2 steps

        // If they meet, there is a cycle!
        if (slow == fast) {
            return true; 
        }
    }

    return false; // Fast reached the end, no cycle exists
}

// Helper function to insert at tail (from Day 8)
void insert_at_tail(Node_t **head, int value) {
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
    if (!new_node) return;
    new_node->data = value;
    new_node->next = NULL;
    
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    Node_t *current = *head;
    while (current->next != NULL) current = current->next;
    current->next = new_node;
}

int main() {
    Node_t *my_list = NULL;
    insert_at_tail(&my_list, 10);
    insert_at_tail(&my_list, 20);
    insert_at_tail(&my_list, 30);
    insert_at_tail(&my_list, 40);

    // Test Case 1: Normal List without cycle
    printf("--- Test Case 1: Normal List ---\n");
    if (has_cycle(my_list)) {
        printf("Result: Cycle Detected!\n");
    } else {
        printf("Result: No Cycle (Safe).\n");
    }

    // Creating a cycle manually for testing:
    // Make the last node (40) point back to the second node (20)
    printf("\n--- Creating a manual cycle: [40] -> [20] ---\n");
    Node_t *node_40 = my_list->next->next->next; // Tail node
    Node_t *node_20 = my_list->next;             // Second node
    node_40->next = node_20;                     // Cycle formed!

    // Test Case 2: List with cycle
    printf("--- Test Case 2: Corrupted List ---\n");
    if (has_cycle(my_list)) {
        printf("Result: Cycle Detected! Preventing infinite crash.\n");
    } else {
        printf("Result: No Cycle.\n");
    }

    // Unlink the cycle before exit to prevent memory leaks during cleanup
    node_40->next = NULL; 
    
    // Clean up memory safely
    Node_t *curr = my_list;
    while (curr != NULL) {
        Node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    return 0;
}