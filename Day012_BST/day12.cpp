#include <stdio.h>
#include <stdlib.h>

// 1. Define the Tree Node structure
typedef struct TreeNode {
    int data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode_t;

// 2. Function to create a new node on Heap
TreeNode_t* create_node(int value) {
    TreeNode_t *new_node = (TreeNode_t*)malloc(sizeof(TreeNode_t));
    if (new_node != NULL) {
        new_node->data = value;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}

// 3. Recursive function to insert a value into the BST
TreeNode_t* insert_node(TreeNode_t *root, int value) {
    // Base Case: If the tree (or subtree) is empty, return a new node
    if (root == NULL) {
        return create_node(value);
    }

    // Otherwise, recur down the tree
    if (value < root->data) {
        root->left = insert_node(root->left, value); // Go left
    } else if (value > root->data) {
        root->right = insert_node(root->right, value); // Go right
    }

    // Return the (unchanged) node pointer
    return root;
}

// 4. In-order Traversal: Left -> Root -> Right (Yields sorted output)
void print_inorder(TreeNode_t *root) {
    if (root == NULL) return; // Base case for recursion termination

    print_inorder(root->left);       // Visit Left Subtree
    printf("[%d] ", root->data);     // Visit Root Node
    print_inorder(root->right);      // Visit Right Subtree
}

// 5. Clean up memory safely (Post-order cleanup: Left -> Right -> Root)
void free_tree(TreeNode_t *root) {
    if (root == NULL) return;
    
    free_tree(root->left);
    free_tree(root->right);
    printf("[Freeing] Node: %d\n", root->data);
    free(root);
}

int main() {
    TreeNode_t *root = NULL;

    printf("--- Part 1: Dynamically Inserting into BST ---\n");
    // Let's build a balanced-looking tree
    root = insert_node(root, 50);
    insert_node(root, 30);
    insert_node(root, 70);
    insert_node(root, 20);
    insert_node(root, 40);
    insert_node(root, 60);
    insert_node(root, 80);

    printf("\n--- Part 2: In-order Traversal (Sorted Output) ---\n");
    printf("Sorted Tree Elements: ");
    print_inorder(root);
    printf("\n");

    printf("\n--- Part 3: Memory Cleanup ---\n");
    free_tree(root);
    root = NULL;

    return 0;
}