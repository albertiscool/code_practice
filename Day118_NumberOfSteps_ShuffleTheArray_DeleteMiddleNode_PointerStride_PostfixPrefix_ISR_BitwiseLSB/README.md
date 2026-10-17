# Day 118 - 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式 (HackMD 考古專題)

## 訓練概況
- **日期**: 2026-10-18
- **訓練主軸**: HackMD 群聯面試共筆實戰化、位元右移奇偶計數 (Number of Steps to Reduce a Number to Zero)、陣列原地位元打包壓縮與逆向解包 (Shuffle the Array)、單向鏈結串列快慢雙指標中點刪除 (Delete the Middle Node of a Linked List)、陣列指標步進與退化陷阱 (`&array + 1`)、指標前置/後置自增與複合運算子時序 (`*(p++)` vs `*(++p)`)、Function Call vs ISR 底層中斷向量表與 Auto-stacking 機制、2's Complement 位元神技 (`a & (~a + 1)` vs `a & (a - 1)`) 與暫存器中斷調度實務、自走車樹莓派與單晶片跨晶片 UART 通訊之 DMA ＋ 空閒中斷 (IDLE Line) 與無鎖環狀緩衝區 (Lock-free Ring Buffer)。
- **目標鎖定**: 群聯電子 (Phison SSD 韌體)、台積電 (TSMC IMC)、電子五哥 (廣達、緯創、和碩、仁寶、英業達)。

---

## Part 1: 白板程式題 (LeetCode 0 ms / Beats 100% 遠端驗證)

### P1: LeetCode #1342 - Number of Steps to Reduce a Number to Zero
- **HackMD 考點對照**: 對應筆記中電訪與筆試核心題「`a & (~a + 1)` 2's Complement 最低位運算」、「`n & 1` 奇偶快速判斷」與「位元右移代替除法」。
- **核心思維**: $O(\log N)$ 位元右移與奇偶計數。當 `(num & 1) == 0` 時右移一位（等同於除以 2），奇數時減 1，直到歸零。
- **架構級數學優化**:
  - 總步數公式：$\text{總步數} = (\text{最高位 1 的位元長度} - 1) + (\text{二進位中 1 的總數量})$。
  - 在 ARM Cortex-M 指令集架構中，利用硬體單週期指令 `CLZ`（Count Leading Zeros）與 `POPCNT`，可直接於 $O(1)$ 常數時間一行計算完畢：
    `return (num == 0) ? 0 : (31 - __builtin_clz(num)) + __builtin_popcount(num);`
- **LeetCode 成績**:
  - **狀態**: Accepted (204/204 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 8.3 MB
  - **Submission ID**: `2173673201`

---

### P2: LeetCode #1470 - Shuffle the Array (重新排列陣列)
- **HackMD 考點對照**: 筆記目錄「考古」第 4 題點名題目 `[1470. Shuffle the Array]`，對應韌體雙通道 DMA Interleaved Buffer（交錯緩衝區）資料穿插重組。
- **核心思維**: 極致 $O(1)$ 空間原地位元打包壓縮（Bitwise Packing）與逆向解包（Reverse Unpacking）。
  - 因 $1 \le nums[i] \le 1000 < 1024$（僅佔 10 bits），利用 32-bit `int` 的高 16 位儲存 $y_i$，低 16 位保留 $x_i$：`nums[i] = nums[i] | (nums[i + n] << 16);`。
- **邊界盲點與除錯**:
  - **正向覆蓋踩踏 (Forward Overwrite)**：若正向從 $i = 0$ 往後寫入 `nums[2*i]`，會導致尚未被讀取的原始元素被提前覆蓋踩壞。
  - **逆向解包破解法**：從 $i = n - 1$ 倒數至 0，將 $y_i$ 寫入 `2*i + 1`、$x_i$ 寫入 `2*i`。因 $2i \ge i$ 恆成立，寫入目標永遠在讀取位置的右方，保證未讀取的左側資料 100% 完好無損，達成無任何 `malloc` 開銷之原地操作。
- **LeetCode 成績**:
  - **狀態**: Accepted (53/53 Passed)
  - **額外空間**: $O(1)$ 嚴格原地操作（0 額外 heap 分配）
  - **Submission ID**: `2173698387`

---

### P3: LeetCode #2095 - Delete the Middle Node of a Linked List
- **HackMD 考點對照**: 筆記「面試」第 8 題點名題目 `[linked list找中點]`。
- **核心思維**: 快慢雙指標（Fast & Slow Pointers）單趟定位與 Stack Dummy Node 邊界防禦。
  - `fast` 一次兩步、`slow` 一次一步，搭配前驅指標 `pre` 同步跟進。
  - 迴圈結束瞬間 `slow` 必然精準停在正中間（$\lfloor n / 2 \rfloor$）。
  - 執行 `pre->next = slow->next;` 斷鏈重接，並主動呼叫 `free(slow)` 釋放節點記憶體，杜絕記憶體洩漏。
- **極限邊界特判**: 開頭精準捕捉 `head->next == NULL`，單節點刪除後直接返回 `NULL`。
- **LeetCode 成績**:
  - **狀態**: Accepted (70/70 Passed)
  - **執行時間**: 2 ms (Beats 95%+)
  - **記憶體**: 82.9 MB
  - **Submission ID**: `2173725115`

---

## Part 2: 面試經典致命陷阱題 (取材自 HackMD 考古題)

### Trap 1: 指標步進 (Stride) 與陣列指標退化陷阱
- **問題分析**:
  ```c
  int array[10] = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};
  int *ptr = (int *)(&array + 1);
  int *ptr2 = array;
  ```
  若 `array` 起始位址為 200，在 32-bit 編譯器下：
- **解答與深度推導**:
  1. **`&array + 1 = 240`**：
     * `&array` 型別為 `int (*)[10]`（指向整個包含 10 個整數的陣列），其步進單位（Stride）為一整塊陣列大小：$10 \times 4 = 40 \text{ bytes}$。
     * 因此位址為 $200 + 40 = 240$。
  2. **`ptr - 1 = 236`，`*(ptr - 1) = 100`**：
     * 強轉為 `(int *)` 後，步進單位恢復為單一整數（4 bytes）。
     * $240 - 4 = 236$，該位址恰好存放 `array` 最後一個元素 `array[9] = 100`。
  3. **`*array + 1 = 11` vs `*(array + 1) = 20`**：
     * `*array + 1`：解引用優先於加法，先取出首元素 10，再加 1 得到 11。
     * `*(array + 1)`：括號優先，指標位移到下一個元素 `array[1]`，解引用得到 20。

---

### Trap 2: 指標前置/後置自增與複合運算子優先級
- **問題分析**:
  ```c
  int a[] = {1, 2, 3, 4, 5, 6};
  int *p = a;
  *(p++) += 100;
  *(++p) += 100;
  ```
- **解答與時序拆解**:
  1. **`*(p++) += 100;`**：
     * 後置自增 `p++` 先返回當前 `p`（指向 `a[0]`）。
     * 執行加法 `a[0] = 1 + 100 = 101`。
     * 循序點生效，`p` 向後移一步指向 `a[1]`。
  2. **`*(++p) += 100;`**：
     * 前置自增 `++p` 先將 `p` 往前推一步，由 `a[1]` 移至 `a[2]`。
     * 執行加法 `a[2] = 3 + 100 = 103`。
     * 最終陣列變為：`[101, 2, 103, 4, 5, 6]`。
  3. **延伸思考 `int e = (*p2)++;`**：
     * 此時 `a[0]` 數值已為 101。
     * 解引用先鎖定 `a[0]`，後置自增將**舊值 101 賦值給 `e`**，隨後記憶體中的 `a[0]` 遞增變為 **102**。
     * 輸出：`e = 101, *p2 = 102`。

---

## Part 3: 基本 0x10 韌體與作業系統觀念題 (取材自 HackMD 原題)

### Q1: Function Call vs ISR 底層本質差異
1. **觸發機制**：
   * 普通函式：軟體同步主動調用（CPU 執行 `BL/BLX` 跳轉指令，將返回位址存入 `LR`）。
   * ISR：外部硬體非同步事件觸發（外設訊號通知 NVIC，硬體強行中斷 CPU，自動查中斷向量表載入 ISR 函式指標）。
2. **參數傳遞與回傳值（嚴格 `void ISR(void)`）**：
   * ISR 由硬體突發觸發，當下背景無任何 Caller 能傳入參數；
   * 若 ISR 有回傳值塞入 `R0` 暫存器，中斷返回時將直接覆蓋破壞背景程式正在使用的暫存器，引發系統隨機崩潰。
3. **Auto-Stacking 與耗時禁忌**：
   * 進入 ISR 前硬體自動將 8 個核心暫存器（`R0-R3, R12, LR, PC, xPSR`）壓入 Stack，退出時自動出棧還原。
   * 嚴禁耗時（如 `printf`、延時）：避免長時間阻斷同級或低優先級中斷，防止高頻通訊丟包（Starvation）。
   * 嚴禁睡眠：ISR 無獨立 TCB，無法進入 Blocked 狀態，等待阻塞會觸發 HardFault 當機。

---

### Q2: 位元遮罩神技：`a & (~a + 1)` vs `a & (a - 1)` 與暫存器調度
1. **`~a + 1` 取最低位元 1 (Lowest Set Bit, LSB)**：
   * 二進位補數中 `~a + 1 == -a`。
   * 取反加 1 會使低位連續的 0 與最低位的 1 在加 1 進位後重新變回 1，而更高位全部取反。
   * 與原數做 AND 運算時，高位全部抵消歸 0，唯獨最低位的 1 被完美萃取出來。
2. **`a & (a - 1)` 抹除最低位元 1**：
   * 減 1 向最低位的 1 借位，使該位變為 0 且更低位變為 1。相與後精準抹去最右側的第一個 1。
   * 若 $a > 0$ 且 `(a & (a - 1)) == 0`，代表全數僅有單一個 1，必為 2 的冪次方。
3. **中斷狀態暫存器工業級迴圈調度實務**：
   ```c
   while (ISR_STATUS > 0) {
       uint32_t current_irq = ISR_STATUS & (~ISR_STATUS + 1); // 提取當前優先級最高 (最右側) 的中斷
       handle_irq(current_irq);                               // 處理該中斷事件
       ISR_STATUS = ISR_STATUS & (ISR_STATUS - 1);            // 清除已處理中斷
   }
   ```
   * 迴圈次數嚴格等於觸發的中斷個數，0 冗餘分支，執行效率達到極致。

---

## 🚗 自走車專案實務科普

### 今日主題：雙核心自走車跨晶片 UART 通訊之「DMA ＋ 空閒中斷 (IDLE Line)」與「無鎖環狀緩衝區」架構
- **問題場景**: 樹莓派（ROS 2 導航節點）以 100 Hz 高頻率下發 `cmd_vel`，單晶片同時回傳編碼器里程計。
- **傳統缺陷**: 單字節中斷接收（`USART_IT_RXNE`）每 87 微秒中斷一次 CPU，在 115200 bps 下引發中斷風暴（Interrupt Storm）；當馬達 PID 運算稍有延遲，瞬間觸發硬體 Overrun Error (ORE) 導致通訊死鎖。
- **工業級解法**:
  1. **UART DMA 背景搬移**：資料由 DMA 控制器硬體自動搬入 RAM，CPU 0% 介入。
  2. **IDLE Line 空閒中斷**：整包指令（如 32 bytes）傳輸完畢且匯流排空閒超過 1 個 byte 時間，才觸發一次中斷，中斷開銷銳減 95% 以上。
  3. **Lock-free Ring Buffer**：DMA 更新 Head、主程式消費 Tail，天然符合 SPSC（單生產者單消費者）模型，全程無需上鎖，兼具零拷貝與微秒級即時反應。
