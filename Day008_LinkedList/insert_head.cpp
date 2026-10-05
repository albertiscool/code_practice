void insert_head(Node_t **head, int value)
{
    Node_t *new_node = (Node_t*)malloc(sizeof(Node_t));
    if (new_node == NULL) return; // Always check allocation

    new_node->data = value;
    new_node->next = *head; // New node points to the current head
    *head = new_node; // Update head to point to the new node
}