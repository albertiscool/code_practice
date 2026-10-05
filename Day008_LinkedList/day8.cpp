#include <stdio.h>
#include <stdlib.h>

// 1. Define the node structure
typedef struct Node {
    int data;
    struct Node *next;
} Node_t;

// 2. Function to insert a new node at the tail of the list
void insert_at_tail(Node_t **head, int value) {
    // Allocate memory for the new node
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
    if (new_node == NULL) return; // Always check allocation
    
    new_node->data = value;
    new_node->next = NULL; // New tail always points to NULL
    
    // If the list is empty, make new node the head
    if (*head == NULL) {
        *head = new_node;
        return;
    }
    
    // Otherwise, traverse to the last node
    Node_t *current = *head;
    while (current->next != NULL) {
        current = current->next;
    }
    
    // Link the old tail to the new node
    current->next = new_node;
}

// 3. Function to print the entire list
void print_list(Node_t *head) {
    Node_t *current = head;
    printf("List Path: ");
    while (current != NULL) {
        printf("[%d] -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}

// 4. Function to safely free all nodes
void free_list(Node_t **head) {
    Node_t *current = *head;
    Node_t *next_node = NULL;
    
    while (current != NULL) {
        next_node = current->next; // Save the next address BEFORE freeing current
        printf("[Freeing] Node with data: %d\n", current->data);
        free(current);
        current = next_node;       // Move to the saved next node
    }
    *head = NULL; // Reset head to NULL to prevent dangling pointer
}

int main() {
    Node_t *my_list = NULL; // Initialize an empty list
    
    printf("--- Part 1: Dynamically Inserting Nodes ---\n");
    insert_at_tail(&my_list, 10);
    insert_at_tail(&my_list, 20);
    insert_at_tail(&my_list, 30);
    
    print_list(my_list);

    printf("\n--- Part 2: Safely Freeing Memory ---\n");
    free_list(&my_list);
    
    print_list(my_list); // Should print "NULL" safely

    return 0;
}