bool is_empty(CircularQueue_t *q) {
    // 俐落的一行流，展現老手風範
    return (q->head == q->tail && q->last == false);
}

bool is_full(CircularQueue_t *q) {
    return (q->head == q->tail && q->last == true);
}