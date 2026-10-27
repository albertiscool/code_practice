# Day 126 - 群聯 (Phison) & 電子五哥 研發替代役 / 碩士預聘實戰訓練

## 學習大綱與核心主題
1. **白板程式題（Part 1）**：
   - **P1**：LeetCode #1356 - Sort Integers by The Number of 1 Bits（Brian Kernighan 位元計數演算法 `n & (n - 1)` + C 語言 `qsort` 雙條件排序）
   - **P2**：群聯面試原題 Q2 - 已排序陣列區間過濾輸出（指標越界防禦、小於區間前置過濾、連續重複元素跳過、邊界值短路求值安全）
   - **P3**：群聯面試原題 Q3 - 靜態陣列鏈結串列有序搜尋（Static Linked List in Array、迪摩根定律否定轉換、遞增單調性提早剪枝、前驅節點 `pre_entry` 追蹤）
2. **面試致命陷阱題（Part 2）**：
   - **Trap 1**：巨集（Macro）副作用與暫存器操作陷阱（Creteken 0x10 Q2 衍生、序列點 Sequence Point、多重自增導致步進失真與 FIFO 資料丟失）
   - **Trap 2**：嵌入式無限迴圈語義與硬體安全設計（Creteken 0x10 Q4 衍生、`while(1)` vs `for(;;)` 歷史差異與現代編譯器機器碼、硬體死等防護 Safe Design Patterns、看門狗 WDT 與超時中斷）
3. **基本 0x10 韌體與作業系統觀念題（Part 3）**：
   - **Q1**：`volatile` 三大時機（硬體周邊暫存器、ISR 與 Main 共享變數、多工 RTOS 共享變數）與編譯器暫存器緩存（Register Caching）機制
   - **Q2**：結構體 Padding 計算與 `#pragma pack(1)` 記憶體對齊（32-bit 自然對齊、通訊協定映射、ARM Cortex-M 未對齊存取 HardFault / 效能懲罰）
4. **自走車專案實務科普**：
   - 雙核心架構之安全保護與失效安全（Failsafe Mechanism & Heartbeat Protocol）
   - 上位機 ROS 2 與下位機 Arduino 雙向心跳超時機制（200ms Timeout）
   - L298N 馬達安全制動：自由滑行（Coasting）vs 主動電氣煞車（Dynamic Braking，短路反電動勢產生電磁阻尼）

---

## Part 1：白板程式題詳細覆盤

### P1：LeetCode #1356 - Sort Integers by The Number of 1 Bits
- **題目要求**：依二進位中 1 的數量升冪排序；若 1 的數量相同，則依照數值本身大小升冪排序。
- **最佳解法**：
  ```c
  static int countBits(int n) {
      int count = 0;
      while (n != 0) {
          n = n & (n - 1);
          count++;
      }
      return count;
  }

  int cmp(const void *a, const void *b) {
      int val_a = *(const int*)a;
      int val_b = *(const int*)b;
      int count_a = countBits(val_a);
      int count_b = countBits(val_b);
      if (count_a == count_b) return val_a - val_b;
      return count_a - count_b;
  }
  ```
- **原始白板盲點保留**：
  - 盲點 1：原實作在 `cmp` 內部直接扣減原數值，導致迴圈結束後數值變成 0，使平手條件 `val_a - val_b` 永遠回傳 `0 - 0 = 0`。
  - 盲點 2：`memcpy(result, arr, arrSize)` 漏乘 `sizeof(int)`，導致僅複製前 1/4 內容，其餘為未初始化 heap 垃圾值引發溢位崩潰。

---

### P2：【群聯原題 Q2】已排序陣列之區間過濾輸出
- **題目要求**：依參數 $b$ ($0 \le b \le 4$)，輸出該區間內的所有數字，但排除陣列 `a` 內出現的元素。
- **最佳解法**：
  ```c
  void printRangeExceptA(const int *a, int size, int b) {
      int start = b * 100;
      int end = start + 100;
      if (a == NULL || size <= 0) {
          for (int i = start; i < end; i++) printf("%d ", i);
          printf("\n");
          return;
      }
      int curr = 0;
      while (curr < size && a[curr] < start) curr++;
      for (int i = start; i < end; i++) {
          if (curr < size && a[curr] == i) {
              while (curr < size && a[curr] == i) curr++;
          } else {
              printf("%d ", i);
          }
      }
      printf("\n");
  }
  ```
- **原始白板盲點保留**：
  - 盲點 1：短路求值條件寫反（`i == a[curr] && curr < size`），導致在檢查邊界前先存取了越界記憶體。
  - 盲點 2：缺少小於區間起點的過濾，若陣列開頭有小於 100 的數字，會導致指標永久卡死。
  - 盲點 3：重複元素若只 `curr++` 一次，指標會卡在第二個重複值，無法比對後續數字。

---

### P3：【群聯原題 Q3】靜態陣列鏈結串列有序搜尋
- **題目要求**：在 `ListArray[1000]` 的靜態鏈結串列中尋找符合 `(DataH, DataL)` 的節點，回傳其 index 並設定前驅節點 `pre_entry`。利用單調性提早退出。
- **最佳解法**：
  ```c
  unsigned int searchListArray(unsigned int DATA_A, unsigned int DATA_B, unsigned int *pre_entry) {
      if (ListHead == NULL_PTR || ListHead >= 1000) return NULL_PTR;
      unsigned int curr_idx = ListHead;
      unsigned int pre = NULL_PTR;
      while (curr_idx != NULL_PTR && curr_idx < 1000) {
          struct ListStruct *curr = &ListArray[curr_idx];
          if (curr->DataH == DATA_A && curr->DataL == DATA_B) {
              if (pre_entry) *pre_entry = pre;
              return curr_idx;
          }
          if (curr->DataH > DATA_A || (curr->DataH == DATA_A && curr->DataL > DATA_B)) {
              return NULL_PTR; // 提早剪枝
          }
          pre = curr_idx;
          curr_idx = curr->NextPtr;
      }
      return NULL_PTR;
  }
  ```
- **原始白板盲點保留**：
  - 盲點 1：迪摩根定律否定錯誤（誤用 `DataH != DATA_A && DataL != DATA_B`），導致高位相同時迴圈提早誤退。
  - 盲點 2：剪枝比大小時在 `else` 裡直接比 `DataL`，忽略了「高位較小整體即較小」的前提。

---

## Part 2：面試致命陷阱題覆盤

### Trap 1：巨集（Macro）副作用與暫存器操作
- **程式碼**：`#define MIN(A, B) ((A) <= (B) ? (A) : (B))` 搭配 `MIN(*p++, b)`
- **分析結果**：
  - `least = 20`
  - 指標指向 `a[2]`，`*p = 30`
- **底層影響**：
  - 巨集引數多次評估，導致指標步進次數與資料數值相關（不確定性步進）。
  - 若在韌體中替換為 `READ_REG()`，硬體 FIFO 會被連續 Pop 出兩筆資料，造成一筆資料遺失（Data Drop），引發嚴重通訊封包錯位。

### Trap 2：嵌入式無限迴圈語義與硬體安全防護
- **`while(1)` vs `for(;;)`**：
  - 早期 naive 編譯器（如 8051 Keil C51）中，`while(1)` 會生成條件常數檢查指令，而 `for(;;)` 直接無條件跳躍。
  - 現代編譯器在常數折疊最佳化下完全一致，機器碼皆為單一無條件跳躍指令（`b .` 或 `jmp .`），性能差異為 0。
- **硬體死等防護 Safe Design Patterns**：
  1. **軟體計數 / 時間超時防禦（Timeout Counter）**：迴圈內加入 `--timeout` 或 SysTick 檢查，超時跳出並執行軟體重置。
  2. **硬體看門狗（Watchdog Timer, WDT）**：主迴圈定時餵狗，死當未餵狗由硬體強制復位。
  3. **RTOS 中斷 + 信號量超時（Semaphore with Timeout）**：透過 `xSemaphoreTake(sem, timeout_ticks)` 釋放 CPU，超時喚醒。

---

## Part 3：基本 0x10 韌體觀念題覆盤

### Q1：`volatile` 三大時機與編譯器暫存器快取
1. **三大使用時機**：
   - 硬體周邊暫存器（Memory-Mapped I/O Registers）。
   - 中斷服務常式（ISR）與主程式共享的全域變數。
   - 多工 RTOS 任務間共享且無鎖保護的狀態旗標。
2. **編譯器最佳化動作（Register Caching）**：
   - 漏加 `volatile` 時，編譯器自作聰明將變數讀入 **CPU 暫存器（如 R0）**，後續迴圈只檢查暫存器而不發起記憶體匯流排讀取，導致即便 RAM/硬體已更新，CPU 仍陷入死迴圈。

### Q2：結構體 Padding 與記憶體對齊
- 原始結構體：`char a; int b; short c;`
  - 對齊計算：$1 \text{ (a)} + 3 \text{ (pad)} + 4 \text{ (b)} + 2 \text{ (c)} + 2 \text{ (tail pad)} = \mathbf{12\text{ bytes}}$。
- `#pragma pack(1)` 緊湊對齊：
  - 大小：$1 + 4 + 2 = \mathbf{7\text{ bytes}}$。
  - 優點：嚴格映射硬體暫存器與網路/通訊封包格式，消除填充偏差。
  - 代價：低階處理器（如 ARM Cortex-M0）不支援未對齊存取，引發 `HardFault` 死當；支援架構則產生額外 Bus 週期與位移拼裝，存取效能大幅下降。

---

## 🚗 自走車專案實務科普：雙核心安全保護與失效安全（Failsafe）

### 1. 問題場景
- 樹莓派 ROS 2（Linux）負責高階導航路徑規劃，透過 UART 串列埠發送控制封包給 Arduino。
- Arduino 輸出 PWM 與方向控制腳位驅動 L298N 馬達。
- 若樹莓派因 Linux 負載過高、行程崩潰、或通訊中斷，若無保護機制，Arduino 將持續執行最後一次接收的 PWM，導致車輛失控撞牆。

### 2. 雙核心心跳機制（Heartbeat Failsafe）
- 上位機以固定週期（例如 50ms）發送控制或心跳封包。
- 下位機 Arduino 在 `loop()` 中記錄 `last_cmd_time`。
- 若 `millis() - last_cmd_time > 200ms`，立刻觸發 Failsafe 狀態機，切斷 PWM 並啟動煞車。

### 3. L298N 馬達安全制動對比（面試核心）
- **滑行（Coasting / Free-wheeling）**：`IN1=0, IN2=0, ENA=0`，H 橋全關斷呈高阻抗，車體靠慣性繼續滑行數公尺，無法防撞。
- **主動電氣煞車（Dynamic Braking）**：`IN1=1, IN2=1, ENA=1`，將馬達線圈兩端短路。旋轉產生的反電動勢（Back-EMF）產生強大電磁阻尼力矩（Lenz's Law），車體在數公分內極速停定。
