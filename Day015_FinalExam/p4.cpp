void delete_node(Node_t **head, int target_value)
{
    Node_t *temp = NULL;
    Node_t *prev = NULL;
    Node_t *curr = *head;
    while(curr != NULL)
    {
        if(curr->value != target_value)
        {
            prev = curr;
            curr = curr->next;
        }
        else
        {
            temp = curr->next;
            free(curr);
            curr = temp;
            prev->next = curr;
        }
    }
}