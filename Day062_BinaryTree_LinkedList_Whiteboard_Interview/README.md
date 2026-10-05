# Day 62: 科技大廠高頻白板題特訓 (二元樹最大深度、二元樹鏡像翻轉、合併有序鏈結串列與 Stack 假頭節點記憶體架構)

今天是針對 Garmin、聯發科、群聯、瑞昱、台達電等科技大廠「高頻白板手寫題」的實戰衝刺。我們精準攻克了 3 道經典白板題（二元樹最大深度、二元樹鏡像翻轉、合併有序鏈結串列），並深度探討了遞迴語意、宣告 vs 呼叫語法、以及 Stack 假頭節點 (`dummy node`) 的底層記憶體運作機制。

---

## 📝 實戰白板題詳細解析與 Code Review

### 題目 1【二元樹的最大深度 (Maximum Depth of Binary Tree)】
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day62_BinaryTree_LinkedList_Whiteboard_Interview/p1.c)
* **核心思維**：分治法 (Divide & Conquer) 遞迴。
  * 終止條件：`if (root == NULL) return 0;`
  * 遞迴計算：`left = maxDepth(root->left); right = maxDepth(root->right);`
  * 當前深度：`return (left > right ? left : right) + 1;`（左右較大者加上自己這一層）。
* **重點觀念**：葉子節點 (`left=NULL, right=NULL`) 不需要寫額外的 `if`，因為底層遞迴回傳 `0`，`1 + max(0, 0) = 1` 自動算出深度為 1！

---

### 題目 2【二元樹翻轉 / 鏡像 (Invert / Mirror Binary Tree)】
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day62_BinaryTree_LinkedList_Whiteboard_Interview/p2.c)
* **核心思維**：原地指標交換 + 遞迴下傳。
  1. 終止條件：`if (root == NULL) return NULL;`
  2. 原地交換：`temp = root->left; root->left = root->right; root->right = temp;`
  3. 遞迴子樹：`invertTree(root->left); invertTree(root->right);`
  4. 回傳樹根：`return root;`
* **C 語言語法辨析**：
  * **宣告新變數**：`struct TreeNode *temp = root->left;`（創立新箱子，必須給型態）。
  * **呼叫已定義函式**：`invertTree(root->left);`（叫函式做事，不加型態）。

---

### 題目 3【合併兩個已排序的鏈結串列 (Merge Two Sorted Lists)】
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day62_BinaryTree_LinkedList_Whiteboard_Interview/p3.c)
* **核心思維**：Dummy Node (定錨假頭) + Tail (游動工頭) 穿針引線。
  1. **迴圈條件**：必須用 **`while (list1 != NULL && list2 != NULL)`**（兩邊同時有節點才能比大小，避免 Null Pointer 當機）。
  2. **穿針引線**：誰小就接誰，接完工頭往前推一格 (`tail = tail->next`)。
  3. **一鍵收尾神技**：剩下的鏈結串列已排好序，不需要 `while` 迴圈，直接 **`tail->next = list1 ? list1 : list2;`** 一整條火車掛載上去！
  4. **回傳結果**：回傳 `return dummy.next;`。

---

## 🔬 深度剖析：為什麼 `dummy` 要宣告為 Stack 物件而非指標？

```c
struct ListNode dummy;          // 🌟 Stack (堆疊) 區域變數
struct ListNode *tail = &dummy; // 用 & 抓出地址
```

### 1. 三種寫法大比拼：
| 宣告方式 | 優點 | 致命缺點 |
| :--- | :--- | :--- |
| **`struct ListNode *dummy;`** | 無 | 💥 **野指標 (Wild Pointer)**，存取 `dummy->next` 直接當機！ |
| **`struct ListNode *dummy = malloc(...);`** | 可正常執行 | 耗費 Heap 效能，函式結束前若未手動 `free()` 會造成 **Memory Leak**！ |
| **`struct ListNode dummy;` (最推薦)** | **0 成本 (Stack 分配)、函式結束自動蒸發回收 (零洩漏)、安全有實體空間** | 無！頂級工程師標配！ |

---

### 2. 執行時 `tail->next` 存取的記憶體位址動態轉移：

```
【初始狀態】：tail 指著 Stack 上的 dummy (0x7000)
  [ dummy (0x7000) ] ◄── tail (0x7000)
     next = NULL

【第 1 步】：tail->next = list1 (0x2000)
  👉 寫入的是 Stack 上 dummy.next 的記憶體！dummy.next 變成 0x2000！
  👉 接著 tail = tail->next; tail 移到 Heap 上的節點 1 (0x2000)！

  [ dummy (0x7000) ] ──► [ 節點 1 (0x2000) ] ◄── tail (0x2000)
                            next = NULL

【第 2 步】：tail->next = list2 (0x3000)
  👉 寫入的是「節點 1 (0x2000)」內部的 next 欄位！
  👉 接著 tail = tail->next; tail 移到節點 2 (0x3000)！

  [ dummy ] ──► [ 節點 1 (0x2000) ] ──► [ 節點 2 (0x3000) ] ◄── tail
```

* **第一次 `tail->next`**：修改的是 **Stack 上 `dummy.next`**，定錨第一節車廂。
* **後續所有 `tail->next`**：修改的都是 **各個實體節點自己的 `next`**，一路將整個鏈結串列串接起來！
