# Day 107: 群聯 Phison ＋ 電子五哥 韌體實戰模式 (暫存器 Hex Dump × 原地快慢雙指標分區 × 串流位元左移 × BSRR 硬體原子操作 × FreeRTOS 0xA5 金絲雀堆疊檢測)

## 📑 今日訓練概覽
- **P1 (暫存器傾印與位元二補數)**: LeetCode #405: Convert a Number to Hexadecimal (Easy) - `unsigned int` 邏輯右移補數 + MSB 尋找與動態分群配置 + 查表法 LUT (0 ms, Beats 100.00%)
- **P2 (陣列原地分區與雙指標)**: LeetCode #905: Sort Array By Parity (Easy) - 快慢指標 Lomuto 原地交換 + `*returnSize` 防護 + 低階 MCU `& 1` 位元判偶優化 (0 ms, Beats 100.00%)
- **P3 (鏈結串列遍歷與位元合成)**: LeetCode #1290: Convert Binary Number in a Linked List to Integer (Easy) - 原地三指標反轉權重累加 + 單趟無損霍納法則串流左移法 `(ans << 1) | head->val` (0 ms, Beats 100.00%)
- **自走車專案實務科普 (第 107 講)**: 馬達驅動電路大電流突波反電動勢（Back-EMF）與電源隔離共地實戰
  - 感應電勢 $V = -L \frac{di}{dt}$ 與突波逆灌 MCU 致命原理
  - 續流二極體（Flyback Diode）反向洩放迴路
  - 光耦隔離（Optocoupler PC817）與動力/邏輯電源星狀單點共地（Star Grounding）
- **0x10 韌體架構實戰情境題**:
  1. **暫存器硬體原子操作 (BSRR vs ODR RMW Race Condition)**: `LDR -> ORR -> STR` 中斷交錯撕裂重現、STM32 32-bit `BSRR` 暫存器（Bit 0~15 Set / Bit 16~31 Reset，寫 0 無動作）單週期無鎖原子防護。
  2. **FreeRTOS 堆疊溢位（Stack Overflow）檢測與排查 SOP**: `configCHECK_FOR_STACK_OVERFLOW` Method 1 (SP 越界) vs Method 2 (`0xA5` 金絲雀魔術字元邊界破壞)、`uxTaskGetStackHighWaterMark()` 水位量化與區域變數大陣列優化。

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Convert a Number to Hexadecimal (#405 - Easy)
- **核心思維**:
  1. 負數在硬體暫存器與 32-bit 電路上即為二補數。第一時間強制轉為 `unsigned int n = (unsigned int)num;`，保證右移時使用**邏輯右移（LSR，補 0）**而非有號數的算術右移（ASR，補 1）。
  2. 使用者原創思路訂正：透過迴圈偵測最後一個 1 的 index 定位 MSB，一次性算出需要字元長度 `size = msb / 4 + 1`，倒序每 4 個 bit 轉換一次，補齊 `'\0'` 與 10~15 轉 `'a'~'f'`。
  3. 工業級查表法：`const char hex_map[] = "0123456789abcdef";` 搭配 `n & 0xF` 與 `n >>= 4`，單週期即刻完成映射。

### P2: Sort Array By Parity (#905 - Easy)
- **一發入魂 (One-shot AC)**:
  - 嚴格記取昨日長度遺漏教訓，第一行立即宣告 `*returnSize = numsSize;`。
  - 快慢指標（Fast-Slow Pointer）原地分區：`fast` 尋找偶數，與 `slow` 進行 In-place Swap，達成 $O(N)$ 時間與 $O(1)$ 額外空間極限。
  - 韌體小彩蛋：低階 MCU 無硬體除法器時，可寫成 `((nums[fast] & 1) == 0)`，單指令週期完成判別。

### P3: Convert Binary Number in a Linked List to Integer (#1290 - Easy)
- **反轉法與串流法雙璧**:
  - 使用者思路：三指標反轉鏈結串列，將 MSB->LSB 轉為 LSB->MSB 後用 `(1 << count)` 累加（0 ms AC）。
  - 工業級單趟串流法（不破壞傳入鏈結串列）：每遇到一個 bit，將累積結果左移一位並 OR 該 bit：
    ```c
    int ans = 0;
    while (head != NULL) {
        ans = (ans << 1) | head->val;
        head = head->next;
    }
    ```

---

## 🚗 自走車實務科普：馬達大電流反電動勢與隔離防護

### 突波成因
馬達電感線圈斷電瞬間，依據 $V = -L \frac{di}{dt}$，反向產生數十伏特高壓突波，容易打死 MCU GPIO 或使 5V 邏輯電源驟降導致 Brown-out Reset 重開機。

### 工業級三大防線
1. **續流二極體（Flyback Diode）**: 馬達兩端反向並聯蕭特基二極體，形成安全洩流迴路。
2. **光耦隔離（Optocoupler）**: 控制訊號以光傳導，動力側與邏輯側完全斷絕電氣直接連接。
3. **星狀單點共地（Star Grounding）**: 避免大電流馬達回路的地線電阻引發地彈（Ground Bounce），邏輯地與動力地在電源負極唯一點匯合。

---

## ⚔️ 0x10 韌體架構實戰情境題深度複盤

### 題目一：暫存器硬體原子操作 (BSRR vs ODR)
- **Race Condition 根本原因**:
  - `GPIOA->ODR |= (1 << 3)` 編譯為 `LDR`、`ORR`、`STR` 3 條指令。
  - 若 `LDR` 讀出舊值後被 ISR 插隊修改了 Pin 5，ISR 結束後主迴圈仍使用暫存器中的舊值執行 `STR`，將導致 Pin 5 剛被更新的狀態慘遭抹除覆蓋。
- **BSRR 暫存器硬體解法**:
  - 32 位元設計：低 16 位元為 Bit Set，高 16 位元為 Bit Reset。
  - **核心機制：寫入 0 無任何動作（No Operation）**。
  - 僅需單一 `STR` 指令，硬體邏輯閘直接切換目標腳位正反器，完全不需關閉中斷（Zero Interrupt Latency），實現純硬體單週期原子操作。

### 題目二：FreeRTOS 任務堆疊溢位排查 SOP
- **Method 1 vs Method 2**:
  - Method 1：僅檢查當前 Stack Pointer (SP) 是否越界。缺點為函式內大陣列溢位後返回時 SP 已復位，無法抓出暫態踩線。
  - Method 2：任務初始化時 Stack 全面填寫 **`0xA5`（金絲雀魔術字元）**。排程器每次 Context Switch 檢查邊界最後 16~20 Bytes 是否被竄改，抓到立即進入 `vApplicationStackOverflowHook()`。
- **`uxTaskGetStackHighWaterMark()`**:
  - 掃描未被改寫的 `0xA5` 剩餘量（單位為 Words），量化歷史最高堆疊消耗，用於精準縮減 Stack 尺寸或設定警戒水位。
  - 優化方針：嚴禁 Stack 宣告大區域陣列（改用 static 或記憶體池）、禁止遞迴呼叫、精簡 printf 呼叫。
