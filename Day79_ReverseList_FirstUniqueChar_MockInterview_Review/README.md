# Day 79: 反轉單向鏈結串列、字串第一個唯一字元與全真面試實戰模擬 (自駕車電梯簡報/指標位移/I2C開漏)

今天正式切換至「九月研替衝刺：全真無提示白板手寫 + 技術主管口試實戰」訓練節奏。順利攻克了科技廠出題率 No.1 的 2 大經典白板題（反轉單向鏈結串列 [三指標防斷鏈雙向倒轉]、字串第一個唯一字元 [頻率統計陣列兩遍走訪]），並在全真模擬面試中 100% 答對 3 大硬核面試題（自駕車異質雙核心 GPOS vs RTOS 電梯簡報、`&a+1` 指標位移深層解析、I2C 開漏架構與上拉電阻缺失之後果）！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【反轉單向鏈結串列 (Reverse Linked List - LeetCode #206)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (全台灣科技廠/IC 廠出題率 No.1 必考神題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day79_ReverseList_FirstUniqueChar_MockInterview_Review/p1.c)
* **演算法**：經典三指標雙向倒轉法 (Iterative 3 Pointers)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 初始化 `prev = NULL`（反轉後的新尾巴指向 NULL），`curr = head`。
  * 每回合四步標準動作：
    1. `next_temp = curr->next;` ➔ 先牽住後續節點（救命繩索，防止斷鏈）。
    2. `curr->next = prev;` ➔ 關鍵反轉：將箭頭倒轉向後指。
    3. `prev = curr;` ➔ `prev` 往前推進一步。
    4. `curr = next_temp;` ➔ `curr` 往前推進一步。
  * 當 `curr == NULL` 時，`prev` 剛好停在新的頭節點，直接回傳 `prev`！

---

### 2. 題目 2【字串中的第一個唯一字元 (First Unique Character - LeetCode #387)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻經典題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day79_ReverseList_FirstUniqueChar_MockInterview_Review/p2.c)
* **演算法**：頻率統計陣列 + 兩遍走訪 (Two-Pass Frequency Array)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 開一個長度 26 的計數陣列 `count[26] = {0}`。
  * 第一遍走訪字串：統計各字元總出現次數 `count[s[i] - 'a']++`。
  * 第二遍依字串原本順序走訪：第一個遇到 `count[s[j] - 'a'] == 1` 的字元，立即回傳其索引下標 `j`！
  * **面試效率細節**：先用 `int len = strlen(s);` 快取長度，避免在 `for` 迴圈條件中重複計算導致 $O(N^2)$ 開銷。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自駕車專題 2 分鐘技術電梯簡報】
* **核心架構分工**：
  * **高階大腦運算層 (Raspberry Pi 4 / Linux GPOS)**：專門負責吃算力的相機影像串流與 **YOLOv8 邊緣深度學習物件辨識**。
  * **底層小腦控制層 (Arduino MCU / Hard Real-Time)**：負責微秒級 **硬體 PWM 馬達差速驅動、IR 循跡閉迴路控制與超音波緊急防撞煞停**。
  * **通訊層**：全雙工 UART 序列傳輸二進位控制指令。
* **設計價值**：將吃算力的 AI 任務與微秒級運動控制實體隔離，**徹底避免 Linux 通用作業系統的排程延遲 (Scheduling Jitter) 影響馬達控制的硬即時性 (Hard Real-Time)**。

---

### 🗣️ 題 2【0x10 指標運算陷阱判定】
```c
int a[5] = {10, 20, 30, 40, 50};
int *p = (int *)(&a + 1);
printf("%d\n", *(p - 2)); // 輸出為 40
```
* **原理解析**：
  * `a` 的型態為 `int *`，`a + 1` 步長為 4 Bytes（指向 `a[1]`）。
  * `&a` 的型態為 `int (*)[5]`，`&a + 1` 步長為整條陣列大小 $5 \times 4 = 20 \text{ Bytes}$，直接跳到陣列屁股後面的記憶體位址。
  * 轉型為 `int *p` 後，步長為 4 Bytes。`p - 2` 往回推 2 個整數 (8 Bytes)，剛好踩在倒數第二個元素 `a[3]` 上，數值為 **`40`**！

---

### 🗣️ 題 3【硬體通訊：I2C 開漏架構與上拉電阻】
* **SDA 與 SCL 電氣架構**：皆為 **開漏 (Open-Drain)** 輸出架構。內部 NMOS 只能將線路強行拉低到接地 (GND)，無法主動輸出高電位 (HIGH)。
* **缺少上拉電阻之後果**：
  * 當 NMOS 截止時，訊號線處於 **高阻抗浮空態 (High-Z Floating)**，沒有任何電源將電位拉高至 VDD。
  * 訊號線電位只能維持在 LOW 或隨雜訊漂移，**永遠無法產生穩定的邏輯 1 (HIGH)**，亦無法產生 I2C 啟動條件 (Start Condition: SCL 為 HIGH 時 SDA 下降)，導致整個 I2C 匯流排完全癱瘓無法通訊！
