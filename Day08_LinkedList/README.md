# Day 8: 資料結構基石 - 單向連結串列 (Singly Linked List)

## 1. 連結串列的強大之處
相較於傳統陣列（大小固定、需一整塊連續記憶體），連結串列將資料散佈在 Heap (堆積區) 的各個角落，並透過**指標 (Pointer)** 像繩子一樣把每個節點 (Node) 串起來，因此可以隨時無限延伸。
```c
typedef struct Node {
    int data;            // 負責裝資料
    struct Node *next;   // 指向下一節點的繩子
} Node_t;
```

## 2. 雙重指標的絕對必要性 (以 Insert 為例)
當我們要在串列中新增節點時，函式必須傳入**雙重指標 (`Node_t **head`)**。
* **Pass by Value 的限制**：如果只傳單一指標 (`Node_t *head`)，函式只會收到指標的「影印本」。
* **致命災難 (Memory Leak)**：當串列為空時，如果只改了影印本，`main` 函式裡的正本依然是 `NULL`。剛 `malloc` 出來的新節點將因為沒有人記住它的位址而永遠遺失，造成嚴重的記憶體外洩。

## 3. 釋放串列的死機殺手：Use-After-Free (UAF)
在 `free_list` 釋放整個串列時，最容易犯下「釋放後使用」的致命錯誤：
* **❌ 錯誤示範 (引發 Segmentation Fault)**：
  先 `free(current);` 接著才走下一步 `current = current->next;`。因為房子已經拆了，你卻還想進去找下一把鑰匙！
* **✅ 正確做法 (先記住再拆房子)**：
  ```c
  Node_t *temp = current->next; // 先把下一家的地址抄在手上
  free(current);                // 然後再把現在這家釋放
  current = temp;               // 走向下一家
  ```

## 4. 白板題實戰：頭部插入 (Insert at Head)
在串列最前面新增節點的標準動作，稱為「兩步交接儀式」（順序絕對不能顛倒，否則會弄丟原本的串列）：
1. **新車廂牽起舊車廂的手**：
   `new_node->next = *head;`
2. **把火車頭標示牌移交給新車廂**：
   `*head = new_node;`
