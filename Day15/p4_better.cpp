void delete_node(Node_t **head, int target_value) {
    Node_t *prev = NULL;
    Node_t *curr = *head;

    while (curr != NULL) {
        if (curr->data == target_value) {
            // 🔪 找到目標了！準備拔車廂
            
            if (prev == NULL) {
                // 狀況 A：要拔的是第一節車廂 (Head)
                *head = curr->next;  // 重新指定火車頭
            } else {
                // 狀況 B：要拔的是中間或尾巴的車廂
                prev->next = curr->next; // 讓前面的車廂，直接牽住後面的車廂 (繞過 curr)
            }
            
            free(curr); // 安全釋放記憶體
            return;     // 任務完成，直接收工 (確保只刪除第一個)
        }
        
        // 沒找到，繼續往下走
        prev = curr;
        curr = curr->next;
    }
    // 如果走到這裡，代表整個 while 迴圈都跑完了還是找不到 target_value
    // 程式會自動結束，完美符合「找不到就不做事」的邊界條件
}