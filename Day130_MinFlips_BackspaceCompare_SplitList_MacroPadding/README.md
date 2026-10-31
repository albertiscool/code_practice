# Day 130 - 群聯 (Phison) & 電子五哥 研發替代役 / 碩士預聘實戰訓練

## 學習大綱與核心主題
1. **白板程式題（Part 1）**：
   - **P1**：LeetCode #1318 - Minimum Flips to Make a OR b Equal to c（位元遮罩與逐位狀態判定，進階 Popcount 一行解，0 ms Beats 100%）
   - **P2**：LeetCode #844 - Backspace String Compare（雙指標由後向前逆向掃描、退格累加計數器 Skip Counter，0 ms Beats 100%）
   - **P3**：LeetCode #725 - Split Linked List in Parts（單向鏈結串列原地指標斷鏈切割 In-place Slicing、商數與餘數均等分配，0 ms Beats 100%）
2. **面試致命陷阱題（Part 2）**：
   - **Trap 1**：巨集文字代換與副作用陷阱（`SQR(2 + 3) = 11` 運算子優先級、`MIN(x++, y)` 雙重自增副作用使 `x` 暴增為 7）
   - **Trap 2**：32-bit 結構體自然對齊（Natural Alignment）與 Padding 計算（`struct A = 12` vs `struct B = 8` 空間優化）
3. **基本 0x10 韌體與作業系統觀念題（Part 3）**：
   - **Q1**：`volatile` 關鍵字三大應用時機（硬體周邊暫存器 MMIO、ISR 與主程式共享變數、RTOS 多執行緒共享變數）
   - **Q2**：Little Endian（小端序）記憶體排布（低位元組存放於低記憶體位址，`0x12345678` 之最低位址存放 `0x78`）
4. **自走車專案實務科普**：
   - 電源隔離與地迴路（Ground Loop）干擾防禦
   - 解決「馬達一轉 MCU 就當機/重開機」的實務解法：光耦合器隔離、星型單點共地（Star Grounding）與電解電容吸收反電動勢

---

## Part 1：白板程式題詳細覆盤

### P1：LeetCode #1318 - Minimum Flips to Make a OR b Equal to c
- **題目要求**：給定三個正整數 $a, b, c$，求最少翻轉 $a$ 與 $b$ 的 bit 數使 $(a \mid b) == c$。
- **最佳實作**：
  ```c
  int minFlips(int a, int b, int c) {
      int count = 0;
      for (int i = 0; i < 32; i++) {
          if ((c & 1) == 1) {
              if ((a & 1) == 0 && (b & 1) == 0) count += 1;
          } else {
              if ((a & 1) == 1) count++;
              if ((b & 1) == 1) count++;
          }
          c >>= 1; a >>= 1; b >>= 1;
      }
      return count;
  }
  ```
- **面試進階加分公式（Popcount 一行解）**：
  ```c
  int diff = (a | b) ^ c;
  int extra = a & b & diff;
  return __builtin_popcount(diff) + __builtin_popcount(extra);
  ```

---

### P2：LeetCode #844 - Backspace String Compare
- **題目要求**：字串中 `'#'` 代表退格鍵，比對兩字串在文字編輯器中的最終內容是否相同，要求 $O(1)$ 額外空間。
- **最佳解法（由後向前逆向雙指標）**：
  ```c
  bool backspaceCompare(char* s, char* t) {
      int curr_s = strlen(s) - 1;
      int curr_t = strlen(t) - 1;
      int count_s = 0, count_t = 0;

      while (curr_s >= 0 || curr_t >= 0) {
          while (curr_s >= 0) {
              if (s[curr_s] == '#') { count_s++; curr_s--; }
              else if (count_s > 0) { curr_s--; count_s--; }
              else break;
          }
          while (curr_t >= 0) {
              if (t[curr_t] == '#') { count_t++; curr_t--; }
              else if (count_t > 0) { curr_t--; count_t--; }
              else break;
          }

          if (curr_s >= 0 && curr_t >= 0) {
              if (s[curr_s] != t[curr_t]) return false;
          } else if ((curr_s >= 0) != (curr_t >= 0)) {
              return false;
          }
          curr_s--;
          curr_t--;
      }
      return true;
  }
  ```
- **使用者盲點紀錄**：
  - 盲點 1：條件寫 `curr_s > 0` 造成索引 0 的字元未被檢查（Off-by-one）。
  - 盲點 2：字元比對成功時，指針推進放在 `else` 區塊，導致外層命中 `if` 後跳過 `else`，指針卡住形成無窮迴圈。修正為比對後無條件推進指針。

---

### P3：LeetCode #725 - Split Linked List in Parts
- **題目要求**：將鏈結串列均等分為 $k$ 段，各段長度差不超過 1，前面段落長度 $\ge$ 後面段落。
- **最佳解法（原地指標切割 In-place Slicing）**：
  ```c
  struct ListNode** splitListToParts(struct ListNode* head, int k, int* returnSize) {
      *returnSize = k;
      struct ListNode** result = (struct ListNode**) calloc(k, sizeof(struct ListNode*));

      int count = 0;
      struct ListNode* curr = head;
      while (curr != NULL) { count++; curr = curr->next; }

      int size = count / k;
      int plus = count % k;

      struct ListNode* p = head;
      for (int j = 0; j < k && p != NULL; j++) {
          result[j] = p;
          int cur_part_size = size + (j < plus ? 1 : 0);

          for (int i = 0; i < cur_part_size - 1; i++) {
              p = p->next;
          }

          struct ListNode* next_head = p->next;
          p->next = NULL;
          p = next_head;
      }
      return result;
  }
  ```
- **使用者盲點紀錄**：
  - 盲點 1：誤將回傳型別宣告為節點實體陣列 `(struct ListNode*) malloc(...)`，應為指標陣列 `(struct ListNode**) calloc(...)`。
  - 盲點 2：試圖將數值複製到陣列並重新建立鏈結，實際上鏈結已存在，只需在每段尾端將 `p->next` 置為 `NULL` 進行原地斷鏈。
  - 盲點 3：餘數分配寫為 `size + plus` 導致第一段過長，正解為前 `plus` 個段落各自加 1。

---

## Part 2：面試致命陷阱題深度解析

### Trap 1：巨集括號與副作用
```c
#define SQR(x)    x * x
#define MIN(a, b) ((a) < (b) ? (a) : (b))

int res1 = SQR(2 + 3);  // 展開為 2 + 3 * 2 + 3 = 11 (非 25)
int x = 5, y = 10;
int z = MIN(x++, y);    // 展開為 ((x++) < (y) ? (x++) : (y))，x 自增 2 次變成 7
```
- **核心鐵律**：
  1. 巨集參數和最外層必須加括號：`#define SQR(x) ((x) * (x))`。
  2. 嚴禁對巨集傳入具副作用的引數（如 `x++`, `*ptr++`）。

---

### Trap 2：結構體記憶體對齊與 Padding
```c
struct A {
    char a;    // 1 byte + 3 pad
    int b;     // 4 bytes
    short c;   // 2 bytes + 2 pad
}; // 總大小 = 12 bytes

struct B {
    int b;     // 4 bytes
    short c;   // 2 bytes
    char a;    // 1 byte + 1 pad
}; // 總大小 = 8 bytes
```
- **對齊兩大原則**：
  1. 成員起始位址必須是該成員型別大小的整數倍。
  2. 結構體整體大小必須是最大成員型別大小的整數倍。

---

## Part 3：基本 0x10 韌體核心觀念題

### 觀念題 1：`volatile` 三大時機
1. **記憶體映射硬體暫存器（MMIO）**：暫存器數值由外部硬體硬線即時變更，防止編譯器將讀值快取到 CPU 暫存器中。
2. **ISR 中斷服務常式與主迴圈共享的全域變數**：中斷非同步修改狀態旗標，必須讓主迴圈每次存取都強制從 RAM 讀取。
3. **多執行緒 / RTOS Task 間共享之全域變數**：防止各 Task 執行緒因編譯器排程最佳化而讀取到陳舊數據。

---

### 觀念題 2：Little Endian 小端序排布
- **核心記憶原則**：「**低位元組放低位址（Low byte at Low address）**」。
- `uint32_t data = 0x12345678;`：
  - 低位元組（LSB）為 `0x78`
  - 高位元組（MSB）為 `0x12`
  - 因此在記憶體起始位址 `0x20000000` 存放的數值必定為 **`0x78`**！
  - 記憶體位址由低到高連續排布為：`78 56 34 12`。

---

## Part 4：自走車專案實務科普

### 雙核心自走車電源隔離與地迴路（Ground Loop）雜訊干擾防禦

#### 1. 系統痛點：為什麼馬達一轉 MCU 就當機 / 重開機？
在雙核心智慧自走車（樹莓派 + STM32/Arduino + L298N 馬達驅動）中，初學者常遇到一個怪現象：
- 「靜態測試感測器都很正常，但只要一送出指令讓馬達旋轉瞬間，MCU 就立刻重開機（Brown-out Reset）或通訊中斷死機！」
- **底層物理原因**：
  1. **馬達啟動突波電流（Inrush Current）**：馬達在啟動或堵轉時瞬間電流高達數安培，導致電池內阻壓降，電源電壓瞬間拉低（Voltage Sag），觸發 MCU 的欠壓復位（BOR）。
  2. **反向電動勢（Flyback Voltage）**：馬達是電感性負載，停止或換向時會產生數十伏特的高壓反電動勢脈衝，透過電源線打回 MCU。
  3. **地電位彈跳（Ground Bounce）**：大電流流經共用地線時產生壓降，破壞訊號基準電位。

---

#### 2. 硬體防禦三部曲（面試應答核心亮點）
1. **雙電源隔離（Dual Power Supply & Optical Isolation）**：
   - 邏輯控制端（MCU / 樹莓派）與動力驅動端（L298N / 馬達）採用獨立電源供電。
   - 控制 PWM 訊號透過**光耦合器（Optocoupler）**進行電氣隔離，切斷高壓干擾路徑。
2. **星型單點共地（Star Grounding）**：
   - 強電（馬達驅動迴路）與弱電（MCU / 感測器訊號迴路）嚴禁串聯共地。
   - 所有地線集中回到電池負極的一點連接（星型接地），防止大電流雜訊流經弱電地平面。
3. **吸收電容與續流二極體（Flyback Diode & Decoupling Capacitor）**：
   - 在馬達電源端並聯大容量電解電容（如 470uF）穩定電壓，並在馬達兩端並聯 104（0.1uF）陶瓷電容過濾高頻電火花雜訊。
