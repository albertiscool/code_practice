# Day 85: 翻轉二元樹、買賣股票最佳時機與全真面試實戰模擬 (雙核通訊防溢位/Stack Overflow金絲雀機制/LIN Bus vs CAN Bus)

今天維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」高效率訓練節奏。在零提示狀態下，攻克了 2 大經典白板手寫題（翻轉二元樹 [DFS 遞迴左右指標交換]、買賣股票最佳時機 [雙指針滑動窗口與歷史最低價動態維護]），並將原始盲點與改良註解同步留存於程式碼中供日後複習。此外，在全真面試模擬中深度拆解車載與 RTOS 核心關鍵（自走車雙核通訊速率不對稱防溢位與超時看門狗、FreeRTOS 堆疊溢位 0xA5 金絲雀檢測原理、車用 LIN Bus 與 CAN Bus 的成本與實體層對比）！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【翻轉二元樹 (Invert Binary Tree - LeetCode #226)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (Google、聯發科、瑞昱、群聯白板高頻題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day85_InvertTree_StockMaxProfit_StackOverflow_LINBus_Review/p1.c)
* **演算法**：二元樹 DFS 遞迴走訪，時間 $O(N)$，空間 $O(H)$。
* **核心邏輯**：
  * **單一守門員**：`if (root == NULL) return NULL;` 即可優雅處理所有節點與葉子。
  * **指標暫存交換**：使用 `temp = root->left` 交換左右子樹指標：
    ```c
    struct TreeNode *temp = root->left;
    root->left = root->right;
    root->right = temp;
    ```
  * **遞迴翻轉**：分別對左右子節點遞迴呼叫 `invertTree(root->left)` 與 `invertTree(root->right)`，最後回傳 `root`。
* **⚠️ 實作盲點複習**：
  * 切勿在遇到葉子節點 (`root->left == NULL && root->right == NULL`) 時誤寫 `return NULL;`，這會導致上層呼叫端接收到空指針，整棵樹所有葉子節點全數被斬斷丟失！

---

### 2. 題目 2【買賣股票的最佳時機 (Best Time to Buy and Sell Stock - LeetCode #121)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠高頻動態規劃/雙指針入門題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day85_InvertTree_StockMaxProfit_StackOverflow_LINBus_Review/p2.c)
* **演算法**：雙指針滑動窗口 / 歷史最低價動態維護，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 左指針 `slow = 0` 代表「買入日」，右指針 `fast = 1` 代表「賣出日」。
  * 當 `fast < pricesSize` 時：
    * **更低進場點**：若 `prices[fast] < prices[slow]`，代表今天發現了更便宜的進場成本，直接跳轉買入日：`slow = fast;`。
    * **獲利結算**：若今天賣出有賺，計算 `prices[fast] - prices[slow]`，挑戰歷史最大獲利 `max`。
    * 賣出日持續前進：`fast++;`。
* **⚠️ 實作盲點複習**：
  * 切勿在「未打破最大獲利」時貿然執行 `slow++`。買入點只有在找到「比當前買入價更便宜」時才需要更新，否則股票稍微回檔就會弄丟歷史最低進場點，導致後續大暴漲時的獲利縮水。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【智慧車雙核通訊：速率不對稱、緩衝區防溢位與超時看門狗】
* **實務場景**：上位機樹莓派（Linux / ROS 2）為 GHz 級高頻發送，下位機 Arduino (ATmega328P, 16MHz) 僅 2KB SRAM，UART 硬體 FIFO 僅 64 Bytes。
* **防溢位策略（Drop-Old Policy）**：
  * UART ISR 僅負責接收 byte 寫入緩衝區。
  * 主迴圈透過 FSM 解析封包。在運動控制領域，「延遲的舊指令比丟包更危險」，因此緩衝區滿時**直接丟棄舊指令、只執行最新完整指令**，避免動作延遲導致撞牆。
* **通訊看門狗急停（Communication Timeout Fail-Safe）**：
  * 紀錄 `last_received_time` 時間戳記。
  * 若超過 200ms 未收到上位機心跳指令（ROS Node 死鎖或 USB 斷線），立刻將 PWM 歸零並啟動動態能耗制動（Dynamic Braking）急停，防止車輛暴衝。

---

### 🗣️ 題 2【0x10 C / RTOS 底層：Stack Overflow 與 FreeRTOS 檢測機制】
* **Stack Overflow 本質**：Cortex-M 等架構堆疊「向下生長（高位址往低位址）」。溢位時 SP 穿透下界，踩踏相鄰的 Heap 堆積區、其他 Task TCB 或全域變數，導致資料被篡改或 `HardFault`。
* **三大常見肇因**：函式內部宣告過大區域陣列（`char buf[1024]`）、無終止遞迴呼叫、中斷巢狀過深。
* **FreeRTOS 兩種檢測機制 (`configCHECK_FOR_STACK_OVERFLOW`)**：
  1. **Method 1（檢查 SP 指針）**：Task 切換時檢查 SP 是否超出邊界。缺點是若函式在溢位後又 return 回合法範圍，Method 1 無法察覺。
  2. **Method 2（0xA5 金絲雀浮水印 Canary）**：Task 建立時將整個 Stack 刷成 `0xA5`。每次切換時檢查最底部最後 16 Bytes 是否「仍全為 0xA5」。只要被踩過覆蓋，立即捕捉並跳入 `vApplicationStackOverflowHook`。

---

### 🗣️ 題 3【車用電子 / 通訊協定：CAN Bus vs. LIN Bus 深度對比】
* **為什麼有 CAN 還需要 LIN？**
  * **成本與線束輕量化**：車身末梢電子（車窗、後照鏡、座椅、雨刷）對頻寬要求極低，全用 CAN 成本過高且雙絞線過重。
* **四大核心差異**：
  1. **實體線路**：CAN 為 2 條雙絞線（CAN_H / CAN_L 差動）；LIN 為 **單線傳輸（1 條線 ＋ 車身金屬搭鐵接地）**。
  2. **電壓與硬體**：CAN 需專用收發器晶片與高精度外部石英振盪器（Crystal）；LIN 直接吃車載 **12V 電瓶**，MCU 使用標準 UART 即可，從節點（Slave）**只需內部廉價 RC 振盪器**。
  3. **仲裁模式**：CAN 為多主架構（Multi-Master）無損位元仲裁；LIN 為 **嚴格單主多從（Single Master, Multi-Slave）**，由 Master 排程輪詢，**零碰撞（No Collision）**。
  4. **傳輸速率**：CAN 達 1 Mbps（CAN FD 達 5~8 Mbps）；LIN 最高僅 20 kbps（速度慢但抗電磁干擾 EMI 極佳）。
