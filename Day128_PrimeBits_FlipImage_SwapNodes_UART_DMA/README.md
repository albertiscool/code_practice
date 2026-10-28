# Day 128 - 群聯 (Phison) & 電子五哥 研發替代役 / 碩士預聘實戰訓練

## 學習大綱與核心主題
1. **白板程式題（Part 1）**：
   - **P1**：LeetCode #762 - Prime Number of Set Bits in Binary Representation（位元計數 + 質數遮罩判斷，二進位 Bitwise 快速判定）
   - **P2**：LeetCode #832 - Flipping an Image（水平翻轉與反轉原地操作 In-place 演算法，雙指標對稱交換與 XOR 反轉）
   - **P3**：LeetCode #1721 - Swapping Nodes in a Linked List（鏈結串列正向第 k 個節點與倒數第 k 個節點數值交換，快慢雙指標單趟掃描 $O(N)$ 時間、$O(1)$ 空間）
2. **面試致命陷阱題（Part 2）**：
   - **Trap 1**：無號數下溢位（Unsigned Integer Underflow）與迴圈終止條件失效（`unsigned int i; i >= 0` 永真陷阱與硬體暫存器越界存取 HardFault）
   - **Trap 2**：編譯期算子 vs 執行期副作用（`sizeof(a++)` 副作用忽視、`sizeof(str)` vs `strlen(str)` 末端 `\0` 差異與通訊封包長度截斷防禦）
3. **基本 0x10 韌體與作業系統觀念題（Part 3）**：
   - **Q1**：`static` 關鍵字在 C 語言的三大作用域（區域變數生命週期延伸、全域變數與函式之 Internal Linkage 模組化封裝，底層 `.data` / `.bss` 配置）
   - **Q2**：記憶體映射 I/O（MMIO）與 Bitwise 暫存器控制（Set / Clear / Toggle / Check Bit 巨集實作，`volatile` 輪詢防止編譯器暫存器快取死迴圈）
4. **自走車專案實務科普**：
   - 雙核心高吞吐通訊：UART DMA + IDLE Line 中斷的不定長封包零拷貝接收架構（Zero-Copy DMA Architecture）
   - 告別 Byte-by-byte 中斷淹沒 CPU，釋放 MCU 算力維持馬達高頻 PID 控制與即時反應

---

## Part 1：白板程式題詳細覆盤

### P1：LeetCode #762 - Prime Number of Set Bits in Binary Representation
- **題目要求**：給定區間 $[left, right]$，計算二進位表示法中 1 的個數為「質數」的整數數量。
- **最佳解法（Bitwise 遮罩法）**：
  因為題目限制 $right \le 10^6 < 2^{20}$，二進位中 1 的個數最多不超過 19。20 以內的質數集合為 $\{2, 3, 5, 7, 11, 13, 17, 19\}$。
  我們可以用一個 32-bit 整數常數做為質數遮罩：
  ```c
  // 質數遮罩：第 2, 3, 5, 7, 11, 13, 17, 19 位為 1
  #define PRIME_MASK ((1 << 2) | (1 << 3) | (1 << 5) | (1 << 7) | \
                      (1 << 11) | (1 << 13) | (1 << 17) | (1 << 19))

  static inline int countSetBits(int n) {
      int count = 0;
      while (n > 0) {
          n &= (n - 1); // Brian Kernighan 演算法
          count++;
      }
      return count;
  }

  int countPrimeSetBits(int left, int right) {
      int ans = 0;
      for (int i = left; i <= right; i++) {
          int bits = countSetBits(i);
          if ((PRIME_MASK >> bits) & 1) {
              ans++;
          }
      }
      return ans;
  }
  ```
- **使用者盲點紀錄（已保留於原始碼註解）**：
  - 盲點 1：質數判斷迴圈變數在每次迴圈未正確重置或判斷邊界，若用標準質數檢查易超時。
  - 盲點 2：除法或迴圈計數器若未正確更新，易造成局部無窮迴圈。改用 Brian Kernighan + 預算 Bitmask 達到極致 $O(1)$ 質數判定。

---

### P2：LeetCode #832 - Flipping an Image
- **題目要求**：給定 $n \times n$ 二進位矩陣，先水平翻轉每列，再將 0 反轉成 1、1 反轉成 0（Invert），回傳處理後的矩陣。
- **最佳解法（原地修改 In-Place + 雙指標對稱 XOR）**：
  水平翻轉時，第 $j$ 個與第 $n - 1 - j$ 個元素對稱互換。
  若兩元素相等（例如都為 0 或都為 1），反轉後剛好數值翻轉（$0 \to 1, 1 \to 0$）；若兩元素不相等（一 0 一 1），交換後再反轉，數值保持不變！
  因此，我們可以直接原地修改：
  ```c
  int** flipAndInvertImage(int** image, int imageSize, int* imageColSize, int* returnSize, int** returnColumnSizes) {
      *returnSize = imageSize;
      *returnColumnSizes = imageColSize;

      for (int i = 0; i < imageSize; i++) {
          int left = 0;
          int right = imageColSize[i] - 1;
          while (left <= right) {
              if (left == right) {
                  image[i][left] ^= 1;
              } else if (image[i][left] == image[i][right]) {
                  image[i][left] ^= 1;
                  image[i][right] ^= 1;
              }
              left++;
              right--;
          }
      }
      return image;
  }
  ```
- **使用者盲點紀錄（已保留於原始碼註解）**：
  - 盲點 1：誤對 `**returnColumnSizes` 進行二級解指標賦值導致記憶體 Segmentation Fault。正解為直接指向陣列指標 `*returnColumnSizes = imageColSize;`。
  - 盲點 2：原本額外動態配置 `malloc` 陣列，佔用額外 $O(N^2)$ 空間。優化為原地修改，達到 LeetCode 0 ms Beats 100%。

---

### P3：LeetCode #1721 - Swapping Nodes in a Linked List
- **題目要求**：交換鏈結串列中正向第 $k$ 個節點與倒數第 $k$ 個節點的數值。
- **最佳解法（雙指標快慢滑動視窗，單趟掃描 $O(N)$ 時間、$O(1)$ 額外空間）**：
  ```c
  struct ListNode* swapNodes(struct ListNode* head, int k) {
      struct ListNode *first = head;
      for (int i = 1; i < k; i++) {
          first = first->next;
      }

      struct ListNode *fast = first;
      struct ListNode *second = head;
      while (fast->next != NULL) {
          fast = fast->next;
          second = second->next;
      }

      int temp = first->val;
      first->val = second->val;
      second->val = temp;

      return head;
  }
  ```
- **核心思維**：先將 `first` 指標移動到正向第 $k$ 個節點。接著讓 `fast` 從 `first` 出發，`second` 從 `head` 出發，保持距離差為 $n - k$。當 `fast` 走到鏈結尾端時，`second` 剛好落在倒數第 $k$ 個節點。

---

## Part 2：面試致命陷阱題深度解析

### Trap 1：無號數下溢位（Unsigned Underflow）與死迴圈陷阱
```c
void read_fifo(uint32_t *buffer, size_t count) {
    unsigned int i;
    for (i = count - 1; i >= 0; i--) {
        buffer[i] = HW_FIFO_REG;
    }
}
```
- **底層原因剖析**：
  - `i` 為 `unsigned int`，其值域落在 $[0, 2^{32}-1]$。
  - 當 `i == 0` 時，執行 `i--`，在二補數系統中 `0 - 1` 所有位元變成全 1（`0xFFFFFFFF`，4,294,967,295）。
  - 無號數永遠不可能小於 0，因此條件式 `i >= 0` **永遠為真（Always True）**，迴圈無限執行。
  - `buffer[0xFFFFFFFF]` 存取了非法記憶體空間，直接觸發 ARM Cortex-M 的 **HardFault** 或引發系統 Bus Error 當機。
- **安全防範寫法**：
  ```c
  // 韌體經典倒數寫法
  while (count--) {
      buffer[count] = HW_FIFO_REG;
  }
  ```

---

### Trap 2：編譯期算子 vs 執行期副作用與封包長度計算
```c
int a = 10;
int size = sizeof(a++);
printf("a = %d, size = %d\n", a, size); // a = 10, size = 4

char str[] = "Phison";
printf("sizeof = %zu, strlen = %zu\n", sizeof(str), strlen(str)); // sizeof = 7, strlen = 6
```
- **核心觀念**：
  - `sizeof` 是 C 語言的**編譯期運算子（Compile-time Operator）**，只推導型別的大小，括號內的運算式（如 `a++`）完全不會生成機器指令，無任何副作用，因此 `a` 保持 10。
  - `sizeof(str)` 計算陣列在記憶體中的實體大小（包含自動補上的字串終端符 `'\0'`，共 7 bytes）。
  - `strlen(str)` 是執行期函式，線性掃描直到遇到 `'\0'` 停止，不包含 `'\0'`（共 6 bytes）。
- **韌體實務風險**：
  - 二進位資料（如感測器封包、暫存器數據）中若包含 `0x00`，使用 `strlen()` 會提前截斷長度，導致傳輸資料短缺。
  - 在函式參數傳遞 `void send(char *buf)` 時若寫 `sizeof(buf)`，會發生陣列指標退化（Array Decay to Pointer），計算結果變成 4 或 8 bytes，引發通訊協議解析崩潰。

---

## Part 3：基本 0x10 韌體與作業系統觀念題

### 觀念題 1：`static` 關鍵字三大作用域與記憶體佈局
1. **三大用法與作用域差異**：
   - **區域靜態變數（Local Static）**：生命週期為整個程式執行期間（從開機到關機），但可視範圍僅限於宣告它的函式區塊內。函式結束後數值不消亡，下次進入函式保留前次結果。
   - **全域靜態變數（Global Static）**：將全域變數鏈結屬性設為 Internal Linkage，僅在該 `.c` 編譯單元內可見，外部 `.c` 無法透過 `extern` 存取。
   - **靜態函式（Static Function）**：限制該函式僅能在定義它的 `.c` 檔案內被呼叫，防止全域命名空間污染（Namespace Pollution）。
2. **記憶體佈局（Memory Layout）**：
   - 區域變數加上 `static` 後，儲存位置由暫時性的 **Stack（堆疊區）** 搬移到靜態資料區：
     - 若有賦予非零初始值，存放在 **`.data` 區段**。
     - 若未初始化或初始值為 0，存放在 **`.bss` 區段**（開機啟動碼 Startup code 會自動清零）。
3. **韌體工程價值**：
   - 驅動程式內部私有函式（如底層暫存器操作、中斷旗標清除等）宣告為 `static`，具備資訊隱藏（Encapsulation）效益，避免不同晶片驅動間函式名稱衝突，並方便編譯器進行 Inline 展開最佳化。

---

### 觀念題 2：記憶體映射 I/O（MMIO）與 Bitwise 暫存器控制
1. **Bitwise 4 大基本操作巨集**：
   ```c
   #define SET_BIT(REG, k)     ((REG) |= (1U << (k)))
   #define CLEAR_BIT(REG, k)   ((REG) &= ~(1U << (k)))
   #define TOGGLE_BIT(REG, k)  ((REG) ^= (1U << (k)))
   #define CHECK_BIT(REG, k)   (((REG) >> (k)) & 1U)
   ```
2. **`volatile` 暫存器指標核心機制**：
   ```c
   #define UART0_STATUS_REG  (*(volatile uint32_t *)0x40001000)
   ```
   - 硬體暫存器的狀態由硬體外部線路即時變更，對 CPU 而言是不可預測的。
   - 若缺少 `volatile`，編譯器在執行 `-O2` 最佳化時，會認為在目前迴圈區塊中沒有任何軟體指令寫入該記憶體位址，進而將第一次讀到的暫存器值暫存在 CPU 一般用途暫存器（如 R0）中進行快取（Register Caching）。
   - 結果：在進行輪詢等待 `while (!(UART0_STATUS_REG & TX_READY_FLAG));` 時，CPU 不會每次去存取真實硬體匯流排，導致軟體陷入死迴圈而當機！加了 `volatile` 保證每次存取都強制發出 Memory Read 匯流排週期。

---

## Part 4：自走車專案實務科普

### 雙核心高速通訊：UART DMA + IDLE Line 中斷之不定長封包零拷貝接收架構

#### 1. 系統痛點：傳統中斷（RXNE ISR）的效能瓶頸
在自走車雙核心架構中，樹莓派（ROS 2 主機）高頻下發速度控制封包（如 Twist 導航座標指令），並由 STM32/MCU 解析執行：
- **傳統作法**：開啟接收中斷 `USART_IT_RXNE`，每收到 1 個 Byte 就產生一次中斷進入 ISR。
- **致命缺陷**：若 Baud Rate 為 115200 或 921600，高吞吐資料串流會讓 MCU 每秒觸發數萬次中斷！CPU 光是進行暫存器保存（Push/Pop Context Switching）就佔據大半算力，導致底層重要的馬達 PWM PID 閉迴路更新延遲甚至抖動。

#### 2. 解決方案：硬體 DMA + 匯流排空閒（IDLE Line）中斷
- **硬體 DMA（Direct Memory Access）**：設定 UART 接收由 DMA 通道自動搬移到記憶體環形緩衝區（Circular Ring Buffer）。資料傳輸過程中，**CPU 介入率為 0%**！
- **IDLE Line（空閒訊號偵測）**：當樹莓派發送完一包不定長指令後，UART 的 RX 引腳會維持 1 個訊框（1 Frame）時間的高電位空閒狀態。硬體 UART 自動偵測到這個狀態後，觸發一次 **`UART_IT_IDLE` 中斷**！

```
[樹莓派 ROS 2] === 發送封包 ===> [UART RX] ---> 硬體 DMA 自動搬移 ---> [DMA Buffer]
                                                       │
                                                傳輸完畢產生空閒
                                                       │
                                                       ▼
                                            觸發一次 IDLE 中斷
                                                       │
                                                       ▼
                                        計算封包長度並由 FSM 處理
```

#### 3. 核心實作範例（STM32 HAL / Register）
```c
#define RX_BUFFER_SIZE  256
uint8_t rx_buffer[RX_BUFFER_SIZE];

// 初始化啟動 UART DMA 接收並開啟空閒中斷
void UART_DMA_Init(void) {
    __HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart1, rx_buffer, RX_BUFFER_SIZE);
}

// UART 中斷服務常式
void USART1_IRQHandler(void) {
    if (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_IDLE)) {
        __HAL_UART_CLEAR_IDLEFLAG(&huart1); // 清除空閒中斷旗標

        // 停止 DMA 傳輸以鎖定長度
        HAL_UART_DMAStop(&huart1);

        // 核心運算：總容量減去剩餘計數器（NDTR），即為收到的實際封包長度
        uint16_t received_len = RX_BUFFER_SIZE - __HAL_DMA_GET_COUNTER(huart1.hdmarx);

        // 將資料交由自走車狀態機 (FSM) 進行封包校驗與馬達指令解析
        Process_Chassis_Packet(rx_buffer, received_len);

        // 重新開啟 DMA 接收，準備接收下一包指令
        HAL_UART_Receive_DMA(&huart1, rx_buffer, RX_BUFFER_SIZE);
    }
}
```

#### 4. 面試應答精華（Phison & 電子五哥亮點）
- **問**：「你在雙核心自走車中，如何確保高頻指令接收不卡死 MCU 即時系統？」
- **答**：「我們採用了 **UART DMA 搭配 IDLE Line 空閒中斷** 的架構。傳統 Byte-by-byte 中斷在高鮑率下會產生嚴重的 Context Switch Overhead，干擾馬達 PID 即時計算。改用 DMA + IDLE 後，資料在背景自動由硬體搬移到 RAM，僅在一筆完整指令傳輸完畢時觸發一次 IDLE 中斷，並透過 DMA 剩餘暫存器（NDTR）常數時間計算出封包長度，實現極致的**零拷貝（Zero-Copy）與零 CPU 佔用**接收機制！」
