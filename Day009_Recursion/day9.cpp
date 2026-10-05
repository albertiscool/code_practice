#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int data;
    struct Node *next;
} Node_t;

// --- Core Interview Question: Reverse Linked List ---
Node_t* reverse_list(Node_t *head) {
    Node_t *prev = NULL;
    Node_t *curr = head;
    Node_t *next_node = NULL;

    while (curr != NULL) {
        next_node = curr->next; // Step 1: Save the next node address
        curr->next = prev;      // Step 2: Reverse the current node's pointer
        prev = curr;            // Step 3: Move prev forward to current node
        curr = next_node;       // Step 4: Move curr forward to next node
    }

    // Crucial: 'prev' now points to the new head of the reversed list
    return prev; 
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

// Helper function to print the list (from Day 8)
void print_list(Node_t *head) {
    Node_t *current = head;
    printf("List Path: ");
    while (current != NULL) {
        printf("[%d] -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

int main() {
    Node_t *my_list = NULL;
    
    insert_at_tail(&my_list, 10);
    insert_at_tail(&my_list, 20);
    insert_at_tail(&my_list, 30);
    insert_at_tail(&my_list, 40);

    printf("--- Before Reversal ---\n");
    print_list(my_list);

    printf("\n--- Executing Reversal ---\n");
    my_list = reverse_list(my_list);

    printf("\n--- After Reversal ---\n");
    print_list(my_list);

    // Clean up memory
    Node_t *curr = my_list;
    while (curr != NULL) {
        Node_t *next = curr->next;
        free(curr);
        curr = next;
    }
    return 0;
}