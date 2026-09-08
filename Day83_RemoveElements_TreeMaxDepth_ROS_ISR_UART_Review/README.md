# Day 83: 移除鏈結串列元素、二元樹最大深度與全真面試實戰模擬 (ROS相機延遲Queue優化/ISR四大鐵律/UART晶振頻率)

今天維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」高效率訓練節奏。在零提示狀態下，連續攻克 2 大經典白板題（移除鏈結串列元素 [Dummy Head 刪除與 free 記憶體防洩漏]、二元樹最大深度 [1 + max 遞迴與變數暫存避免 O(2^N)]），並在全真模擬面試中 3 題全數高分通關（ROS 相機即時串流 Queue Size=1 丟棄舊幀零延遲策略、中斷服務常式 ISR 四大不可違背禁忌與延遲處理架構、UART 11.0592 MHz 整數除頻與非同步累計採樣漂移原理）！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【移除鏈結串列元素 (Remove Linked List Elements - LeetCode #203)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day83_RemoveElements_TreeMaxDepth_ROS_ISR_UART_Review/p1.c)
* **演算法**：虛擬頭節點 (Dummy Head) 雙指針走訪刪除，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 建立區域變數 `struct ListNode dummy; dummy.next = head;`，徹底消除頭節點被刪除的特殊邊界分支。
  * 前驅指針 `pre = &dummy`，當前指針 `curr = head`。
  * 走訪條件 `while (curr != NULL)`：
    * 若 `curr->val == val`：記錄 `next_node = curr->next;`，將 `pre->next = next_node;` 跨過節點，呼叫 `free(curr)` 釋放記憶體，並推進 `curr = next_node;`。
    * 若無刪除：`pre` 與 `curr` 雙雙向前推進一步。
  * 回傳真實頭節點 `dummy.next`。

---

### 2. 題目 2【二元樹的最大深度 (Maximum Depth of Binary Tree - LeetCode #104)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (樹狀結構入門必考神題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day83_RemoveElements_TreeMaxDepth_ROS_ISR_UART_Review/p2.c)
* **演算法**：分治法 (Divide & Conquer) 後序遍歷遞迴，時間 $O(N)$，空間 $O(H)$。
* **核心邏輯**：
  * 終止條件：`if (root == NULL) return 0;`。
  * 遞迴計算左右子樹深度並快取至變數：`int leftDepth = maxDepth(root->left);`、`int rightDepth = maxDepth(root->right);`，避免在條件表達式中重複遞迴引發 $O(2^N)$ 指數爆炸。
  * 當前節點最大深度 ＝ `1 + (leftDepth > rightDepth ? leftDepth : rightDepth)`（加上當前自己這一層的 `+1`）。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自駕車專題：ROS 相機影像串流延遲與 Queue 排空優化】
* **延遲根本原因**：相機發布幀率高於 YOLO 神經網路推理幀率時，影像積累在訊息佇列中。
* **Queue Size 大小之致命陷阱**：
  * 加大 Queue Size 會導致隊列積滿 0.5~1 秒前的「歷史舊畫面」，自駕車拿過期畫面做決策將引發嚴重碰撞事故。
* **工業級零延遲策略**：
  1. **Queue Size 強制設為 1 (Drop Old Frames)**：配合 `best_effort` QoS，YOLO 運算忙碌時直接丟棄中間舊影格，永遠只處理最新鮮的一張照片。
  2. **調降解析度至 320x240**：降低 USB 頻寬負載與神經網路預處理延遲。
  3. **V4L2 底層 Buffer 限制為 1**：消除 Linux 核心緩衝排隊。

---

### 🗣️ 題 2【0x10 底層：中斷服務常式 (ISR) 四大設計鐵律】
1. **絕不允許阻塞與睡眠 (No Sleep / No Mutex)**：中斷無 Task 實體，阻塞將直接凍結 CPU。
2. **越短越快 (Short and Fast)**：避免長時間佔用 CPU 導致低優先級中斷遺失與系統抖動。
3. **無參數、無回傳值 (`void ISR(void)`)**：由硬體向量觸發，無呼叫者傳參。
4. **禁止動態記憶體配置與重度 I/O (`malloc`/`free`/`printf`)**：非確定性時間與不可重入鎖風險。
* **大包數據後續處理架構**：
  * **MCU**：ISR 寫入環形緩衝區 (Ring Buffer) 並立 Flag，由主迴圈 `while(1)` 處理。
  * **FreeRTOS**：ISR 透過信號量/隊列發送信號，喚醒背景 Worker Task (中斷延遲處理)。
  * **Linux**：中斷上半部 (Top Half, 快速應答) + 下半部 (Bottom Half: Workqueue/Tasklet 耗時運算)。

---

### 🗣️ 題 3【硬體通訊：UART 11.0592 MHz 晶振與 Baud Rate 採樣誤差】
* **為什麼採用 11.0592 MHz 晶振？**
  * 該頻率為常見標準鮑率的整數倍（$11059200 \div 115200 = 96$、$11059200 \div 9600 = 1152$）。
  * 晶片內部 Baud Rate 分頻器可達成 **0.00% 完美零除頻誤差**；反之整數 12 MHz 分頻會產生小數截斷誤差 (誤差達 8.5%)。
* **為何誤差超過 3%~5% 會出現亂碼？**
  * UART 為非同步串列傳輸（無共用時脈線），接收端靠自身時脈在每 bit 中間（50% 處）進行採樣。
  * 在 1 個 Frame (10 bits) 的傳輸過程中，頻率誤差會隨 bit 逐位累計；當累積時序偏差超過半個 bit 寬度時，採樣點將漂移至相鄰 bit，造成位元取樣錯誤、Framing Error 與亂碼！
