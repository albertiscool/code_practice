typedef struct {
    int data[QUEUE_SIZE];
    int head;
    int tail;
    bool last; //F=read,T=write
} CircularQueue_t;

bool is_empty(CircularQueue_t *q)
{
    if(q->head == q->tail && last == false)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool is_full(CircularQueue_t *q)
{
    if(q->head == q->tail && last == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}