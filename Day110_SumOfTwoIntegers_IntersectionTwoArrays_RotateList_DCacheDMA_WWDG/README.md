# Day 110: 群聯 Phison ＋ 電子五哥 韌體實戰模式 (位元半加器 × 查表交集陣列 × 鏈結串列環狀斷鏈 × 指標尺度洋蔥定理 × D-Cache DMA 一致性 × WWDG 視窗看門狗)

## 📑 今日訓練概覽
- **P1 (矽晶片半加器位元運算)**: LeetCode #371: Sum of Two Integers (Medium) - 嚴格禁止使用 `+` 與 `-`，硬體 ALU 半加器與進位位移實作 + `(unsigned int)` 杜絕負數溢位未定義行為 (0 ms, Beats 100.00%, Sub ID: 2165077289)
- **P2 (陣列交集與查表動態配置)**: LeetCode #349: Intersection of Two Arrays (Easy) - $O(M + N)$ 雜湊標記與原地去重 + 邊界記憶體精準配置 `malloc` (0 ms, Beats 100.00%, Sub ID: 2165087399)
- **P3 (鏈結串列環狀斷鏈手術)**: LeetCode #61: Rotate List (Medium) - 原地閉環、大數取模 $k = k \% \text{len}$、步數方向精準計算 `len - k` + 斷尾取新頭 (0 ms, Beats 100.00%, Sub ID: 2165117634)
- **面試經典致命陷阱題**:
  - 二維陣列三種指標尺度 (`a + 1` 位移 16B、`&a + 1` 位移 48B、`*a + 1` 位移 4B)
  - N 維陣列剝洋蔥降維定理：每解開一層 `*`，剝掉最外層維度括號，步長為剩餘維度之乘積
- **自走車專案實務科普 (第 110 講)**:
  - L298N 馬達驅動 H 橋上下臂直通（Shoot-Through）危機與互補 PWM 死區時間（Dead-Time）防護
  - 電感性負載反電動勢（Back-EMF）$V_L = -L \frac{di}{dt}$ 與 8 顆續流二極體（Flyback Diodes）箝位保護
  - 馬達啟動湧入電流引發地彈（Ground Bounce）與 MCU 掉電重啟排查（光耦隔離 + 單點星形共地 Star Grounding）
- **0x10 韌體架構實戰情境題**:
  1. **STM32H7 / Cortex-M7 D-Cache DMA 一致性翻車案**:
     - 硬體 DMA 繞過 D-Cache 直寫 SRAM 導致 CPU 讀取舊快取（Stale Data）
     - `SCB_InvalidateDCache_by_Addr`（RX 丟棄快取）vs `SCB_CleanDCache_by_Addr`（TX 回寫記憶體）
     - 緩衝區 32-Byte 快取行對齊（Cache Line Alignment）防護
  2. **獨立看門狗 (IWDG) vs 視窗看門狗 (WWDG) 實戰除錯**:
     - 程式陷入死迴圈但仍在定時餵狗（Task Hang but Feeding Dog）之缺陷
     - WWDG 視窗機制（防餵太慢 + 防餵太快）
     - FreeRTOS 工業級多任務事件群組（EventGroup Bitmask）看門狗守護架構

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Sum of Two Integers (#371 - Medium)
- **核心思維**:
  - 半加器（Half-Adder）硬體邏輯：無進位加法為 XOR（`a ^ b`），進位產生為 AND（`a & b`）。
  - 進位必須往高位傳遞一位（`carry << 1`）。
  - 重複迭代直到無進位（`carry == 0`）。
- **關鍵避坑點**:
  - **運算順序相依性**：必須先計算進位 `carry = (a & b) << 1`，再更新無進位和 `a = a ^ b`，不可提前覆寫 `a`。
  - **有號數左移未定義行為 (UB)**：C 語言對負數執行左移 `<<` 屬於未定義行為。必須強制轉型為 `(unsigned int)(a & b) << 1`。
  - **型別轉換警告**：指派回 `int` 時加上 `(int)carry` 滿足 MISRA-C 10.1 規範。

### P2: Intersection of Two Arrays (#349 - Easy)
- **核心思維**:
  - 數值範圍在 $[0, 1000]$，利用 $O(1)$ 空間查表陣列 `seen[1001]` 紀錄 `nums1` 出現過的數字。
  - 遍歷 `nums2` 時，若 `seen[nums2[j]] == 1`，則加入結果，並**立刻將 `seen[nums2[j]] = 0`** 達成原地去重。
- **關鍵避坑點**:
  - 動態記憶體配置大小應為 `min(nums1Size, nums2Size)`，避免無謂記憶體浪費。
  - 語法細節：三元運算子必須先 `?` 後 `:`。

### P3: Rotate List (#61 - Medium)
- **核心思維**:
  - 先遍歷單向鏈結串列取得總長度 `len`，並讓指針停在尾節點 `curr`。
  - 將尾節點接回頭部 `curr->next = head` 形成環狀鏈結串列。
  - 將旋轉量取模 `k = k % len`。
  - 右移 $k$ 步相當於新尾巴位於原尾巴往後走 `len - k` 步。
  - 斷環並回傳新頭部 `curr->next`。
- **關鍵避坑點**:
  - **除以零與長度偏差**：`curr->next != NULL` 停在尾節點時，`len` 必須初始化為 1。若初始化為 0，在單一節點情況下 `len` 為 0，`k % len` 將觸發除以零 SIGFPE / 硬體 Fault 崩潰！
  - **旋轉方向步數**：鏈結串列只能向前走，向右旋轉 $k$ 步是走 `len - k` 步，而非走 $k$ 步（走 $k$ 步為向左旋轉）。

---

## 💣 面試經典陷阱題：二維與三維陣列指標尺度洋蔥定理

### 二維陣列尺度：`int a[3][4]`（sizeof(int) == 4）
- `a + 1`: 型別為 `int (*)[4]`，位移 **16 Bytes**（跳過 1 整列）。
- `&a + 1`: 型別為 `int (*)[3][4]`，位移 **48 Bytes**（跳過整座 3x4 陣列）。
- `*a + 1`: 型別為 `int *`，位移 **4 Bytes**（跳過 1 個 int 元素，指向 `&a[0][1]`）。

### 三維陣列尺度：`int a[2][3][4]`
- `&a + 1`: 型別為 `int (*)[2][3][4]`，位移 **96 Bytes**（跳過整座 3D 陣列）。
- `a + 1`: 型別為 `int (*)[3][4]`，位移 **48 Bytes**（跳過 1 個 2D 面）。
- `*a + 1`: 型別為 `int (*)[4]`，位移 **16 Bytes**（跳過 1 條 1D 列）。
- `**a + 1`: 型別為 `int *`，位移 **4 Bytes**（跳過 1 個 int 元素）。
- `***a + 1`: 型別為 `int`，為**純數值運算 $+ 1$**（非指標位移）。

---

## 🚗 自走車實務科普：L298N H 橋死區時間、反電動勢與地彈隔離

### 1. H 橋上下臂貫通與死區時間（Dead-Time）
- 上下臂開關切換時，因電晶體關斷延遲時間（Fall Time），若上臂未完全關斷即開啟下臂，將造成電源直接對地短路（Shoot-Through），瞬間大電流燒毀 H 橋。
- 高級定時器（如 TIM1/TIM8）硬體死區產生器強制插入 500ns~2μs 空窗時間防護。

### 2. 電感性負載反電動勢（Back-EMF）保護
- 馬達線圈端電壓滿足 $V_L = -L \frac{di}{dt}$。急停或 PWM 切換瞬間產生逆向高壓。
- 8 顆快速蕭特基續流二極體（Flyback Diodes）提供感應電流迴路，箝位保護開關管。

### 3. 光耦隔離與地彈（Ground Bounce）抑制
- 馬達湧入大電流（Inrush Current）流經地線寄生阻抗造成地電位跳動。
- 解決方案：動力電與邏輯電獨立供電 + 控制信號光耦隔離 + 單點星形共地（Star Grounding）。

---

## ⚔️ 0x10 韌體架構實戰情境題深度複盤

### 題目一：STM32H7 / Cortex-M7 D-Cache DMA 一致性
- **問題本質**: DMA 直接操作 SRAM 繞過 CPU D-Cache，CPU 快取中保有陳舊資料（Stale Data）導致讀取錯誤。
- **CMSIS 指令**:
  - `SCB_InvalidateDCache_by_Addr()`：用於 **DMA 接收（RX）**，丟棄 Cache Line 強制 CPU 向 SRAM 讀取最新資料。
  - `SCB_CleanDCache_by_Addr()`：用於 **DMA 發送（TX）**，將 CPU 修改的 Dirty Line 強制回寫 SRAM 供 DMA 搬移。
- **對齊規範**: DMA 緩衝區必須按照 Cache Line 大小（32 Bytes）進行記憶體對齊。

### 題目二：IWDG vs WWDG 與多任務看門狗架構
- **IWDG 盲點**: 程式在死迴圈內定時餵狗時，IWDG 只有超時下限，無法偵測系統邏輯假死。
- **WWDG 優勢**: 具備時間視窗上限與下限，餵太慢（超時）或餵太快（邏輯錯亂高頻呼叫）皆立即觸發 Reset。
- **FreeRTOS 多任務架構**:
  - 使用 EventGroup 為每個任務分配獨立 Bit。
  - 任務週期結束時設置專屬 Bit。
  - 獨立 Watchdog Daemon 任務定時檢查是否所有 Bit 均已就緒，驗證全數存活才執行硬體餵狗並清空 Bits。
