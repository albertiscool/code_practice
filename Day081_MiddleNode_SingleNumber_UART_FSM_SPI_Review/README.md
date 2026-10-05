# Day 81: 鏈結串列中間節點、只出現一次的數字與全真面試實戰模擬 (UART封包FSM/volatile/SPI Mode 0 vs 3)

今天維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」高效率訓練節奏。在零提示狀態下，100% 獨立秒殺了 2 大經典白板題（鏈結串列的中間節點 [快慢雙指標只需一次走訪]、只出現一次的數字 [全域 XOR 異或消去法]），並在全真模擬面試中 3 題全數高分通關（自駕車 UART 封包黏包撕裂與 FSM 狀態機解析、`volatile` 核心機制與 `counter++` 非原子操作競態、SPI Mode 0 與 Mode 3 之 CPOL/CPHA 上升緣採樣原理）！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【鏈結串列的中間節點 (Middle of the Linked List - LeetCode #876)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day81_MiddleNode_SingleNumber_UART_FSM_SPI_Review/p1.c)
* **演算法**：快慢雙指標法 (Fast & Slow Pointers)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * `slow` 與 `fast` 指標起點皆設為 `head`。
  * 迴圈走訪：`while (fast != NULL && fast->next != NULL)`。
  * `slow` 每次走 1 步，`fast` 每次走 2 步。
  * 當 `fast` 抵達尾端時，`slow` 剛好停留在正中間節點（奇數長度精確居中，偶數長度自動停在第二個中間節點），直接回傳 `slow`！

---

### 2. 題目 2【只出現一次的數字 (Single Number - LeetCode #136)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (IC 設計廠必考經典題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day81_MiddleNode_SingleNumber_UART_FSM_SPI_Review/p2.c)
* **演算法**：全域 XOR 異或消去法 ($a \oplus a = 0$)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 初始化 `result = 0`。
  * 走訪陣列，將所有數值逐一異或：`result ^= nums[i];`。
  * 根據異或交換律與結合律，所有成對出現的數字相互抵消歸零 ($x \oplus x = 0$)，最後留下來的必定是那個唯一單獨存在的數字！

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自駕車專題：UART 封包撕裂與有限狀態機 (FSM) 架構】
* **黏包與撕裂根因**：高頻發送與作業系統排程導致串列緩衝區一次讀取的資料跨越多個封包或只有半個封包。
* **工業級解決方案**：
  1. **自定義二進位幀通訊協定**：
     * `[0xAA 0x55 (Sync Header)] + [Length] + [Command & Payload] + [CRC-16 Checksum]`。
     * 固定同步標頭保障雜訊干擾後能瞬間重新對齊；長度欄位杜絕黏包。
  2. **接收端有限狀態機 (FSM)**：
     * `WAIT_HEADER` ➔ `GET_LENGTH` ➔ `RECEIVE_DATA` ➔ `VERIFY_CRC`。
     * 配合超時機制 (Timeout)，超過 100ms 殘缺未收齊直接重置，確保系統永不死鎖。

---

### 🗣️ 題 2【0x10 底層：`volatile` 本質與 Thread-Safe 誤區】
* **`volatile` 核心作用**：指示編譯器禁止將該變數快取於 CPU 內部暫存器，**每次讀寫操作都必須強制存取記憶體 (RAM/MMIO Register)**。
* **`counter++` 不是 Thread-Safe 的原因**：
  * `counter++` 在組合語言層級是由 3 條指令組成：`LDR (讀) ➔ ADD (改) ➔ STR (寫)`（Read-Modify-Write）。
  * 該操作並非單週期原子操作 (Non-Atomic)。在多執行緒搶占下，會產生競態條件 (Race Condition) 與髒讀覆蓋，必須搭配互斥鎖 (Mutex) 或硬體原子指令 (Atomic) 保護。

---

### 🗣️ 題 3【硬體通訊：SPI Mode 0 與 Mode 3 之 CPOL / CPHA 原理】
* **`CPOL` (Clock Polarity, 時鐘極性)**：控制 SCLK 閒置時的電位狀態（`0`: 閒置 LOW；`1`: 閒置 HIGH）。
* **`CPHA` (Clock Phase, 時鐘相位)**：控制資料採樣時機（`0`: 第一個時鐘邊緣採樣；`1`: 第二個時鐘邊緣採樣）。
* **為什麼工業晶片 (Flash / SD Card) 常同時支援 Mode 0 與 Mode 3？**
  * **Mode 0 (`CPOL=0, CPHA=0`)**：閒置為 LOW，第 1 個邊緣是 **上升緣 (Rising Edge)** 進行資料採樣。
  * **Mode 3 (`CPOL=1, CPHA=1`)**：閒置為 HIGH，第 1 個邊緣為下降緣 (資料變更)，第 2 個回升的邊緣恰好也是 **上升緣 (Rising Edge)** 進行資料採樣！
  * **共同本質**：兩種模式**均是在時鐘的「上升緣」鎖存資料 (Sample on Rising Edge)**，因此硬體相容性極佳！
