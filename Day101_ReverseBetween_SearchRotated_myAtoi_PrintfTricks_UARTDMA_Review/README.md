# Day 101 - 一線 IC 設計廠與系統廠進階戰場模式

## 📋 今日訓練總覽

本日訓練涵蓋 **3 道一線 IC 設計廠（聯發科、瑞昱、聯詠、群聯）高頻白板題**、**C 語言經典照妖鏡題（聖誕樹與 Printf 五大考點）**、**Dcard 台灣科技業韌體面試題庫深度剖析**、**雙核心智慧車 UART DMA 抗掉包科普**，以及 **2 道 0x10 韌體架構實戰情境題**。

---

## 💻 一、 今日白板程式題深入複習

### 1. 題目 1: [反轉指定區間鏈結串列 (Reverse Linked List II)](./p1.c) - LeetCode #92
* **題目要求**：給定單向鏈結串列頭節點 `head` 與區間 `left`, `right`（1-indexed），在原地（In-place）一次走訪（One-pass）完成指定區間反轉。
* **面試核心解法：【頭插法 (Head Insertion)】**
  * 引入 `dummy` 虛擬頭節點，避免 `left = 1` 時頭節點被替換的特判問題。
  * `pre` 指標釘死在 `left - 1` 位置；`curr` 指標釘死在第 `left` 個節點（它反轉後將自然成為區間尾部）。
  * 執行 `(right - left)` 次拔插：每次將 `curr` 後面的 `next_node` 拔除，強行插入至 `pre` 的正後方。
  * **原始盲點解析**：
    1. 筆誤：`return NULL:` 分號打成冒號。
    2. 指標走訪時 `curr` 前進但 `pre` 未跟進，導致 `pre` 停留在 `NULL`。
    3. 迴圈內 `curr = next_node; pre = curr; curr->next = pre;` 造成兩指標指到同一處，產生自我迴圈（Cycle）。
    4. 缺少頭插法或前後重接縫合，導致串列脫節或 `left = 1` 時頭節點丟失。

### 2. 題目 2: [搜尋旋轉排序陣列 (Search in Rotated Sorted Array)](./p2.c) - LeetCode #33
* **題目要求**：在未知軸心旋轉後的升序陣列中，以 $O(\log N)$ 時間複雜度搜尋 `target`。
* **面試核心解法：【兩層決策樹雙邊包夾】**
  * 第一層：透過 `nums[left] <= nums[mid]` 判斷當前是「左半段連續遞增」還是「右半段連續遞增」。
  * 第二層：在確認連續遞增的那半邊，利用**雙邊包夾不等式**判斷 `target` 是否落在此區間：
    * 若左半段有序：檢查 `nums[left] <= target && target < nums[mid]`。若在裡面則 `right = mid - 1`，否則往右半段 `left = mid + 1`。
    * 若右半段有序：檢查 `nums[mid] < target && target <= nums[right]`。若在裡面則 `left = mid + 1`，否則往左半段 `right = mid - 1`。
  * **原始盲點解析**：
    * 只寫了 `target < nums[mid]`，漏掉了下邊界 `nums[left] <= target` 的檢查，導致當目標值小於左邊界時（如找 0 但左邊界為 4），誤入不存在目標的遞增區間。

### 3. 題目 3: [實作 myAtoi 字串轉整數 (String to Integer)](./p3.c) - LeetCode #8
* **題目要求**：跳過前導空格、檢查正負號、逐字讀取數字，並進行 32 位元溢位飽和截斷（`INT_MAX` / `INT_MIN`）。
* **面試核心解法：【32 位元整數溢位防禦公式】**
  * `INT_MAX = 2147483647`（`INT_MAX / 10 = 214748364`）。
  * 溢位判定條件：
    ```c
    if (result > INT_MAX / 10 || (result == INT_MAX / 10 && digit > 7)) {
        return (sign == 1) ? INT_MAX : INT_MIN;
    }
    ```
  * **原始盲點解析**：
    1. 致命無窮迴圈：在讀取數字迴圈中**漏寫 `i++`**。
    2. 正負號檢查遺漏：只檢查了 `'-'`，漏掉了 `'+'` 號的處理。
    3. 溢位判定代數化簡後漏邊界：式子兩邊消去 `digit` 後等價於只檢查 `result > INT_MAX / 10`，漏掉了剛好等於臨界值且尾數大於 7 的狀況。
    4. 負數溢位截斷值：回傳 `minus * INT_MAX` 得到 `-2147483647`，而真正的 `INT_MIN` 是 `-2147483648`（少扣 1）。

---

## 🎄 二、 C 語言面試經典加練：聖誕樹與 Printf 五大考點

### 1. 聖誕樹 (金字塔) 雙層迴圈通式 ([tree.c](./tree.c))
* **核心通式（0-indexed C 語言迴圈）**：
  * 第 $i$ 行（$0 \le i < n$）：
    * 前導空白數：$n - 1 - i$
    * 星星數：$2i + 1$（若誤寫為 $2i - 1$，在 $i=0$ 時會產生 0 顆星的空白行，且使樹幹向右錯位無法置中！）。

### 2. Printf 五大面試連環考點
1. **回傳值陷阱**：`printf()` 回傳值為**「成功輸出的字元總數」**。
   * 經典考題：`printf("%d", printf("%d", printf("%d", 43)))` $\to$ 輸出為 **`4321`**。
2. **韌體暫存器格式化**：
   * `0x%08X`：固定 8 碼、十六進位大寫、不足位補零。
   * `%.*s`：指定長度印出字串，避免無結尾 `\0` 之封包緩衝區越界。
3. **格式化字串漏洞 (Format String Bug)**：
   * 嚴禁寫 `printf(user_input)`，若輸入含有 `%s%x` 會沿著 Stack 記憶體外洩資料或觸發 HardFault；永遠寫 `printf("%s", user_input)`。
4. **唯一回寫記憶體的指令 `%n`**：
   * 不印出任何內容，而是將目前為止印出的字元總數回寫進指標變數 `int*` 中。
5. **為什麼 ISR（中斷常式）嚴禁呼叫 `printf`？**
   * **太慢且阻塞**：115200 波特率下印 30 字元要花近 3ms，嚴重耽誤高優先級中斷。
   * **不可重入與死結**：內部使用共用緩衝區與 Lock，主迴圈持有鎖時被 ISR 搶佔會導致死結（Deadlock）。

---

## 🔍 三、 Dcard 台灣科技業韌體面試精華整理

整理自 Dcard 科技業板近 3 年聯發科、群聯、瑞昱、聯詠、廣達、台達真實面試回報：

1. **C 語言 0x10 必考核心**：
   * `volatile`（硬體暫存器映射、ISR 變數、避免編譯器錯誤優化快取）。
   * `static`（修飾全域變數縮減 Scope、修飾區域變數常駐 Data segment、修飾函式限制單一編譯單元）。
   * `const` 指標指針變形（`const int*` vs `int* const`）。
   * 位元操作：`set bit`、`clear bit`、`toggle bit`、`n & (n - 1)` 判斷 2 的次方。
   * 記憶體對齊：`sizeof(struct)` 計算與 Padding 原因。
2. **白板程式題範疇**：
   * 絕非純軟 Hard，而是集中於 LeetCode Easy ~ Medium 邊界的指標與陣列操作（Linked List 反轉/找環、旋轉二分搜、字串溢位防禦）。
3. **OS 與中斷**：
   * Deadlock 四大必要條件、Mutex vs Spinlock、Stack vs Heap 記憶體溢位排查。
4. **通訊介面必問表**：
   * I2C vs SPI vs UART 全方位比較（腳位、同步/非同步、全雙工/半雙工、Open-Drain 上拉電阻、I2C 匯流排鎖死 9-Clock 復原 SOP）。

---

## 🏎️ 四、 自走車專案實務科普（系統廠 / 廣達 / 台達最愛考點）

### 題目：UART DMA + 環形緩衝區（Circular Buffer）抗掉包架構
* **傳統單字節中斷（RX Interrupt）瓶頸**：
  * 高波特率下每收到 1 byte 即觸發一次中斷，CPU 頻繁進行 Context Switching（暫存器壓棧/出棧），拖垮馬達即時控制。
* **DMA + IDLE Line + Circular Buffer 解決方案**：
  * **硬體 DMA**：硬體直接將 Byte 搬移至 RAM 緩衝區，CPU 全程 0% 負擔。
  * **IDLE Line 斷幀中斷**：一整包指令（如 16 Bytes）發送完畢、匯流排空閒時才觸發一次中斷通知 CPU 解析。
  * **Circular Buffer**：頭進尾出的無鎖先進先出（FIFO）佇列，解耦接收與解包，徹底杜絕封包覆蓋丟失。

---

## ⚔️ 五、 0x10 與韌體架構實戰情境題

### 【情境題 1】：中斷 ISR 與主迴圈的「資料撕裂（Torn Read）」
* **問題**：32 位元編碼器計數器 `volatile uint32_t encoder_ticks;` 在非 32 位元 MCU 主迴圈讀取時偶發暴增值。
* **解析**：讀取 32 位元需分兩次組合語言指令，低 16 位元讀完若剛好被中斷進位，主迴圈會拼裝出「舊低位 + 新高位」的撕裂值。
* **解法**：在讀取臨界區前後關閉中斷（`__disable_irq()` / `__enable_irq()`），或使用雙重讀取校驗法（Double-reading）。

### 【情境題 2】：跨晶片結構體封包 Padding 與 Endian 陷阱
* **問題**：樹莓派（64-bit Linux）透過 UART 傳送 `struct ControlPacket { uint8_t cmd; uint32_t speed; };` 至 MCU，MCU 解出亂碼。
* **解析**：
  1. 編譯器自然對齊（Natural Alignment）在 `cmd` 後填充 3 bytes padding，導致跨平台位移量不同步。
  2. 兩端晶片大小端序（Endianness）不一致。
* **解法**：使用 `#pragma pack(push, 1)` 或 `__attribute__((packed))` 強制 1 位元組對齊，或採用顯式位元組序列化（Byte-level Serialization）通訊協議。
