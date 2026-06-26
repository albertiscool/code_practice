Node_t* reverse_recursive(Node_t *head) {
    // 1. 終止條件 (沒車廂或只有一節車廂，直接交差)
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // 2. 召喚小弟去處理後面的車廂 (無條件相信他會做好！)
    // 小弟會回傳新的火車頭 (也就是最後一節車廂)
    Node_t *new_head = reverse_recursive(head->next);

    // 3. 善後工作：讓我的下一節車廂，回頭牽住我
    head->next->next = head;
    
    // 我變成最後一節車廂了，我的手要放開 (指向 NULL)
    head->next = NULL;

    // 4. 把小弟找到的新火車頭，往上交差
    return new_head;
}