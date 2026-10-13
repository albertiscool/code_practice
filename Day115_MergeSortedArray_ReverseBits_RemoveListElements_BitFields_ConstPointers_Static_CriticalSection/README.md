# Day 115 - 群聯 Phison ＋ 電子五哥 研發替代役 / 碩士預聘實戰模式

## 訓練概況
- **日期**: 2026-10-13
- **訓練主軸**: 原地陣列合併 (Merge Sorted Array)、二進位對摺分治位元顛倒 (Divide & Conquer Reverse Bits)、鏈結串列節點安全釋放 (Remove Elements)、結構體位元欄 (Bit-fields) 對齊陷阱、`const` 指標與 Flash 寫入保護、`static` 作用域與驅動模組化封裝、單晶片臨界區 (PRIMASK 關中斷) 與 RTOS Mutex 擁有權。
- **目標鎖定**: 群聯電子 (Phison SSD 韌體)、台積電 (TSMC IMC)、電子五哥 (廣達、緯創、和碩、仁寶、英業達)。

---

## Part 1: 白板程式題 (LeetCode 0 ms / Beats 100% 遠端驗證)

### P1: LeetCode #88 - Merge Sorted Array (原地合併排序陣列)
- **核心思維**: 逆向雙指標技術。由尾端 `tail = m + n - 1` 往前填入較大數值，完全杜絕覆蓋 `nums1` 前端有效資料的記憶體踩踏。此法對應 SSD 韌體 FTL 垃圾回收 (Garbage Collection) 合併兩組已排序 Page 清單時的標準操作。
- **邊界盲點與除錯**:
  - `void` 函式不可返回數值 (`return nums1;` 觸發編譯錯誤)。
  - 迴圈終止條件必須為 `>= 0` 而非 `!= 0`，避免第 0 個有效元素被遺漏跳過。
  - 條件設為 `curr1 >= 0 && curr2 >= 0` 可自然通吃 `m == 0` 或 `n == 0`，無需任何冗餘特判。
- **LeetCode 成績**:
  - **狀態**: Accepted (63/63 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 10.7 MB
  - **Submission ID**: `2170903204`

### P2: LeetCode #190 - Reverse Bits (32 位元顛倒)
- **核心思維**: 二進位對摺分治法 (Divide & Conquer Bit Reversal)。
- **實作原理**:
  - 16-bit 對調 $\to$ 8-bit 對調 $\to$ 4-bit 對調 $\to$ 2-bit 對調 $\to$ 1-bit 對調。
  - 使用遮罩 (`0xFF00FF00`, `0xF0F0F0F0`, `0xCCCCCCCC`, `0xAAAAAAAA`) 於 5 行無分支 (Branchless) 運算內以 ~15 個 CPU cycles 完成。
  - 對應 ARM Cortex-M 單週期硬體指令 `RBIT` 的純 C 軟體模擬。
- **LeetCode 成績**:
  - **狀態**: Accepted (600/600 Passed)
  - **執行時間**: 5 ms (常數時間 $O(1)$)
  - **記憶體**: 9 MB
  - **Submission ID**: `2170930036`

### P3: LeetCode #203 - Remove Linked List Elements (移除鏈結串列元素)
- **核心思維**: Stack Dummy Node 統一頭節點邊界，搭配主動 `free(curr)` 杜絕記憶體洩漏。
- **指標不變量**: 命中目標時「只移動 `curr` 不移動 `pre`」，精準縫合連續相同目標（如 `[2, 2]`）。
- **LeetCode 成績**:
  - **狀態**: Accepted (66/66 Passed)
  - **執行時間**: 0 ms (Beats 100%)
  - **記憶體**: 13.5 MB
  - **Submission ID**: `2170929279`

---

## Part 2: 面試經典致命陷阱題

### Trap 1: 結構體位元欄 (Bit-fields) 與 Padding / Alignment 陷阱
- **程式碼**:
  ```c
  struct Register {
      unsigned int enable    : 1;
      unsigned int mode      : 3;
      unsigned int reserved  : 4;
      unsigned char status;
  };
  ```
- **解答與計算**:
  1. **大小為 8 bytes**：
     - 前三個位元欄型別為 `unsigned int`，合體佔用 1 個 4-byte 儲存單元 (Allocation Unit，Offset 0~3)。
     - 第四個成員 `status` 為 `unsigned char`，佔 1 byte (Offset 4)。
     - 結構體最大成員對齊需求為 4 bytes，尾端補 3 bytes Padding (Offset 5~7)，$4 + 1 + 3 = 8$ bytes。
  2. **MMIO 暫存器風險**:
     - C 語言標準並未規定位元由 LSB 還是 MSB 開始排列 (Implementation-defined)。
     - 編譯器生成的隱式 RMW (Read-Modify-Write) 在具備 W1C (Write-1-to-Clear) 或 Clear-on-Read 特性的硬體暫存器上會意外誤清相鄰中斷旗標。

### Trap 2: `const` 指標排列組合與修改限制
- **程式碼**:
  ```c
  int val1 = 10, val2 = 20;
  const int *ptr1 = &val1;
  int * const ptr2 = &val1;
  ```
- **解答**:
  1. **AD 錯誤，BC 合法**：
     - `const int *ptr1`（Pointer to const）：資料唯讀，指標可改。`*ptr1 = 30` 錯，`ptr1 = &val2` 對。
     - `int * const ptr2`（Const pointer）：指標唯讀，資料可改。`*ptr2 = 30` 對，`ptr2 = &val2` 錯。
  2. **記憶體區段與強改後果**:
     - 全域 `const` 變數存放於 `.rodata` 區段（映射至 Flash / ROM 唯讀記憶體）。
     - 若強行以 `*(int*)&a = 20` 寫入，ARM Cortex-M 匯流排將回報寫入保護錯誤，觸發 **BusFault** 或 **HardFault** 死當。

---

## Part 3: 基本 0x10 韌體與作業系統觀念題

### Q1: 靜態關鍵字 `static` 的三大修飾範圍與記憶體生命週期
- **全域變數**: 作用域限制於當前檔案 (Internal Linkage)，外部無法 `extern`。生命週期為整個程式期間，存放於 `.data` / `.bss`。
- **區域變數**: 作用域限制於函式內，但生命週期延長至整個程式期間，儲存區段由 Stack 搬移至 `.data` / `.bss`。
- **函式**: 限制於當前檔案內呼叫。在驅動程式中用於資訊封裝 (Encapsulation) 並避免跨檔案符號命名衝突 (Multiple Definition Error)。

### Q2: 競爭危害 (Race Condition) 與臨界區 (Critical Section) 保護
- **臨界區定義**: 存取共享資源（全域緩衝區、暫存器）且不可被並行打斷的程式碼區段。
- **單晶片 (Bare-metal) 保護機制**:
  - 進臨界區前關閉中斷，出臨界區後恢復中斷。
  - 專業實務寫法：備份 `__get_PRIMASK()` 狀態，執行 `__disable_irq()`，退出時以 `__set_PRIMASK()` 恢復，避免破壞外層中斷保護。
- **RTOS Mutex vs Counting Semaphore**:
  - **Mutex**: 具備擁有權 (Ownership，誰鎖誰解)，且支援**優先權繼承 (Priority Inheritance)** 機制以防止優先權反轉。
  - **Semaphore**: 無擁有權概念 (常用於 Task 與 ISR 間的事件同步 Signaling)，不支援優先權繼承。

---

## Part 4: 自走車專案實務科普 (第 115 講)
### 雙核心跨晶片通訊：UART 二進位封包協定、狀態機 (FSM) 與防暴衝機制
- **封包結構 (Binary Frame)**:
  `| Header (0xAA 0x55) | Len | CMD | Payload | CRC16/Checksum | Tail (0x0D 0x0A) |`
  採用交錯位元雙 Byte 幀頭防止線路雜訊誤觸發，並輔以 CRC 校驗。
- **非阻塞有限狀態機 (FSM)**:
  逐 Byte 推進狀態 (`WAIT_HEADER_1` $\to$ `WAIT_HEADER_2` $\to$ `WAIT_LEN` $\to$ `RECEIVE_PAYLOAD` $\to$ `CHECK_CRC` $\to$ `EXECUTE`)，失敗時自動失步重同步 (Resync)，不卡死通訊。
- **通訊超時看門狗 (Watchdog Failsafe)**:
  超過 200ms 未收到樹莓派心跳封包時，MCU 硬體定時器中斷自動將馬達 PWM 歸零並強制剎車，杜絕訊號中斷自走車暴衝！
