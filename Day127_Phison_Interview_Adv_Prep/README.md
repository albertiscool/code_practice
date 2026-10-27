# Day 127 - 群聯 (Phison) & 電子五哥 研發替代役 / 碩士預聘實戰訓練

## 學習大綱與核心主題
1. **白板程式題（Part 1）**：
   - **P1**：【群聯面試 Q3 續集】靜態陣列鏈結串列有序插入（Sorted Insert into Static Linked List in Array、雙階比對 DataH/DataL、頭插法與非頭插法指針串接、避免覆蓋 `now` 與有號/無號數轉型防禦）
   - **P2**：【群聯面試 Q1/Q2 變形】區間缺失範圍摘要（Summary Missing Ranges、三段式區間掃描 vs $O(1)$ 串流輸出點名指針 `next` 法、格式化輸出 `["x", "l->r"]`）
   - **P3**：【群聯底層手撕題】記憶體重疊防禦安全拷貝（`my_memmove`、重疊方向判斷 `dest > src && dest < src + n` 反向拷貝、`void*` 轉型 `unsigned char*` 逐 Byte 搬移、避免 Off-by-one 越界）
2. **面試致命陷阱題（Part 2）**：
   - **Trap 1**：陣列指標退化與步進算術（Creteken Q7 / HackMD Q1 原題、`a` vs `&a` 型別本質區別、步進單位 `sizeof(int)` vs `sizeof(int[5])`）
   - **Trap 2**：指標運算子優先級與副作用（HackMD Q6 / Creteken 0x10 原題、`*(p++) += 100` vs `*(++p) += 100` 執行順序與記憶體狀態變化）
3. **基本 0x10 韌體與作業系統觀念題（Part 3）**：
   - **Q1**：Little-Endian vs Big-Endian 判定與記憶體排列（指標法、聯集 Union 法、記憶體低位址到高位址數值分佈 `78 56 34 12` vs `12 34 56 78`）
   - **Q2**：嵌入式中斷服務常式（ISR）三大禁忌與底層原理（Creteken 0x10 Q12、無 caller 上下文無法傳參返回、`printf` 內部取鎖引發死鎖、`malloc`/`free` 非可重入性與阻塞延遲破壞 Real-Time）
4. **自走車專案實務科普**：
   - 馬達霍爾編碼器（Encoder）正交中斷測速與雜訊防禦（Quadrature Decoding & Noise Immunity）
   - 外部中斷引腳（EXTI）與 4 倍頻解析度提升原理
   - 馬達電磁干擾（EMI 毛邊）防禦：硬體 RC 低通濾波、軟體微秒級防彈跳濾波（Delta Time Filter）、硬體計時器編碼器介面（Timer Encoder Mode）

---

## Part 1：白板程式題詳細覆盤

### P1：【群聯 Q3 續集】靜態陣列鏈結串列有序插入 (Sorted Insert)
- **題目背景**：在靜態 `ListArray[1000]` 中實作節點插入，保持 `(DataH, DataL)` 嚴格遞增排序，若重複則回傳 `-1`。
- **最佳實作架構**：
  ```c
  int insertListEntry(unsigned int DATA_A, unsigned int DATA_B) {
      if (ListHead == NULL_PTR) {
          int new_idx = allocateEntry();
          if (new_idx == -1) return -1;
          ListHead = new_idx;
          ListArray[ListHead].DataH = DATA_A;
          ListArray[ListHead].DataL = DATA_B;
          ListArray[ListHead].NextPtr = NULL_PTR;
          return ListHead;
      }
      struct ListStruct curr;
      unsigned int pre = NULL_PTR;
      unsigned int now = ListHead;

      while (now != NULL_PTR && now < 1000) {
          curr = ListArray[now];
          if (curr.DataH > DATA_A || (curr.DataH == DATA_A && curr.DataL >= DATA_B)) break;
          pre = now;
          now = curr.NextPtr;
      }

      if (now != NULL_PTR && DATA_A == curr.DataH && DATA_B == curr.DataL) return -1;

      int new_entry = allocateEntry();
      if (new_entry == -1) return -1;

      if (pre == NULL_PTR) {
          unsigned int temp = ListHead;
          ListHead = new_entry;
          ListArray[new_entry].DataH = DATA_A;
          ListArray[new_entry].DataL = DATA_B;
          ListArray[new_entry].NextPtr = temp;
          return new_entry;
      } else {
          unsigned int temp = ListArray[pre].NextPtr;
          ListArray[pre].NextPtr = new_entry;
          ListArray[new_entry].DataH = DATA_A;
          ListArray[new_entry].DataL = DATA_B;
          ListArray[new_entry].NextPtr = temp;
          return new_entry;
      }
  }
  ```
- **核心盲點剖析**：
  1. `curr != NULL_PTR` 編譯錯誤：`curr` 是結構體，不能與整數 `NULL_PTR` 比較。
  2. 拆成兩個 `while` 迴圈會打亂排序：低位比較必須在高位相等的前提下才成立。
  3. 指標覆蓋盲點：避免使用 `now = allocateEntry()`，因為 `now` 記錄著後續節點 index，覆蓋會導致鏈結遺失；且需用 `int` 接收以正確判斷 `-1` 溢位。

---

### P2：【群聯 Q1/Q2 變形】區間缺失範圍摘要 (Summary Missing Ranges)
- **題目要求**：找出 `[lower, upper]` 範圍內遺失的數字，印出區間格式 `["2", "4->49", ...]`，要求 $O(N)$ 時間與 $O(1)$ 空間。
- **最佳實作架構**：
  ```c
  void printMissingRanges(const int *nums, int numsSize, long long lower, long long upper) {
      long long next = lower;
      int first = 1;
      printf("[");
      for (int i = 0; i < numsSize; i++) {
          if (nums[i] < next) continue;
          if (nums[i] > upper) break;
          if (nums[i] > next) {
              long long start = next;
              long long end = nums[i] - 1;
              if (!first) printf(", ");
              if (start == end) printf("\"%lld\"", start);
              else printf("\"%lld->%lld\"", start, end);
              first = 0;
          }
          next = (long long)nums[i] + 1;
      }
      if (next <= upper) {
          long long start = next;
          long long end = upper;
          if (!first) printf(", ");
          if (start == end) printf("\"%lld\"", start);
          else printf("\"%lld->%lld\"", start, end);
      }
      printf("]\n");
  }
  ```
- **核心盲點剖析**：
  - 避免在 `void` 函式中動態配置 `char**`，應直接使用 `printf` 串流即時輸出，以達成 $O(1)$ 空間複雜度並消除記憶體洩漏風險。

---

### P3：【群聯底層手撕題】記憶體重疊防禦安全拷貝 (`my_memmove`)
- **題目要求**：不使用標準庫函式，以 $O(1)$ 空間手撕安全拷貝，防禦重疊。
- **最佳實作架構**：
  ```c
  void* my_memmove(void *dest, const void *src, size_t n) {
      if (dest == NULL || src == NULL || n == 0 || dest == src) return dest;
      unsigned char *d = (unsigned char *)dest;
      const unsigned char *s = (const unsigned char *)src;

      if (d > s && d < s + n) {
          for (size_t i = n; i > 0; i--) {
              d[i - 1] = s[i - 1]; // 反向拷貝，避免 off-by-one 越界
          }
      } else {
          for (size_t i = 0; i < n; i++) {
              d[i] = s[i]; // 正向拷貝
          }
      }
      return dest;
  }
  ```
- **核心盲點剖析**：
  1. 重疊方向：僅有在 `d > s && d < s + n`（目標在來源後方且重疊）時才必須反向拷貝；`d < s` 時正向拷貝絕對安全。
  2. `void*` 不能直接做算術運算與解引用，需強制轉型為 `unsigned char*`。
  3. 反向迴圈以 `d[i - 1] = s[i - 1]` 存取，避免索引到達 `n` 引發 Buffer Overflow。

---

## Part 2：面試致命陷阱題覆盤

### Trap 1：陣列指標退化與步進算術
- **輸出**：`2, 5`
- **底層剖析**：
  - `a` 退化為 `int *`，指向 `a[0]`，`a + 1` 步進 4 bytes 指向 `a[1]`（值為 2）。
  - `&a` 型別為陣列指標 `int (*)[5]`，指向整個陣列，`&a + 1` 步進 $5 \times 4 = 20\text{ bytes}$，跳過整個陣列指到尾端邊界外。
  - `ptr` 轉為 `int *` 後，`ptr - 1` 後退 4 bytes，精準指向陣列最後一顆元素 `a[4]`（值為 5）。

### Trap 2：指標運算子優先級與副作用
- **輸出**：`101 2 103 4 5 6`
- **底層剖析**：
  - `*(p++) += 100`：後置自增回傳原始位址（`a[0]`），對 `a[0]` 加 100 變為 101，隨後指標前移至 `a[1]`。
  - `*(++p) += 100`：前置自增先將指標從 `a[1]` 前移至 `a[2]`，再解引用對 `a[2]` 加 100 變為 103，`a[1]` 保持為 2。

---

## Part 3：基本 0x10 韌體觀念題覆盤

### Q1：Little-Endian vs Big-Endian 判定與記憶體排列
1. **判斷實作**：
   ```c
   uint16_t a = 0x0001;
   uint8_t *b = (uint8_t *)&a;
   if (*b == 0x01) { /* Little Endian */ }
   else { /* Big Endian */ }
   ```
2. **記憶體排列（由低位址至高位址 `0x00 ~ 0x03`）**：
   - Little-Endian（低位資料放低位址）：`78 56 34 12`
   - Big-Endian（高位資料放低位址）：`12 34 56 78`

### Q2：嵌入式中斷服務常式（ISR）三大禁忌
1. **無回傳值與無參數**：ISR 由硬體中斷控制器非同步觸發，執行上下文由硬體自動壓棧，沒有 caller 傳遞參數或接收返回值。
2. **禁止 `printf` 與浮點運算**：`printf` 內部常有互斥鎖，在 ISR 內呼叫會導致與主程式競爭同一把鎖引發系統永久死鎖；浮點運算耗時漫長且增加壓棧開銷，破壞即時性。
3. **禁止 `malloc`/`free` 與阻塞延遲**：動態記憶體分配非可重入（Non-reentrant），會腐化 Heap 鏈結；ISR 擁有最高優先級，若執行阻塞延遲會導致 CPU 獨佔，其餘任務與中斷群集餓死（Starvation）。

---

## 🚗 自走車專案實務科普：馬達編碼器正交中斷測速與雜訊防禦

### 1. 正交編碼測速與方向判定
- A 相與 B 相存在 90 度相位差。
- 將 A 相設為外部中斷（Rising Edge 觸發）：
  - 觸發時讀取 B 相電位：若為 LOW 則為正轉（`ticks++`），若為 HIGH 則為反轉（`ticks--`）。
- 雙相雙邊沿中斷可達成 **4 倍頻解析度**。

### 2. 馬達電磁干擾（EMI 毛邊）防禦策略
1. **硬體 RC 低通濾波**：編碼器線路串聯 $100\,\Omega$ 並對地並聯 $0.1\,\mu\text{F}$ 陶瓷電容，截止頻率設於脈衝上限，硬體濾除高頻尖波。
2. **韌體微秒級防彈跳**：在 ISR 內讀取 `micros()`，若兩次中斷間隔小於閾值（如 $50\,\mu s$）判定為雜訊直接丟棄。
3. **Timer Encoder Mode**：在高階晶片中將信號直接接入硬體定時器編碼器介面，利用晶片內部數位濾波器（Glitch Filter）達成 0% CPU 負載的純硬體解碼。
