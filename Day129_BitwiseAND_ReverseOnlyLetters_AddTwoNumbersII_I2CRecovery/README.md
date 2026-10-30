# Day 129 - 群聯 (Phison) & 電子五哥 研發替代役 / 碩士預聘實戰訓練

## 學習大綱與核心主題
1. **白板程式題（Part 1）**：
   - **P1**：LeetCode #201 - Bitwise AND of Numbers Range（區間連續位元 AND、二進位最高公共前綴 Common Prefix、Brian Kernighan 最低位 1 抹除演算法）
   - **P2**：LeetCode #917 - Reverse Only Letters（字串雙指標首尾對稱向中間靠攏、In-place 原地交換、`<ctype.h>` 字元邊界處理，0 ms Beats 100%）
   - **P3**：LeetCode #445 - Add Two Numbers II（高位在前單向鏈結串列大數加法、鏈結串列原地反轉 Reverse List、巧妙「頭插法（Head Insertion）」避免二次反轉）
2. **面試致命陷阱題（Part 2）**：
   - **Trap 1**：運算子優先級與指標遞增解參考陷阱（`*ptr++`, `(*ptr)++`, `*++ptr`, `++*ptr` 數值變化、指標位移與後置遞增「回傳舊值」致命陷阱）
   - **Trap 2**：`const` 指標排列組合（以 `*` 為分界判斷內容唯讀 vs 指標唯讀）與強制轉型寫入 Flash/ROM 常數之硬體行為（ARM Cortex-M HardFault / Linux Segmentation Fault）
3. **基本 0x10 韌體與作業系統觀念題（Part 3）**：
   - **Q1**：韌體 Memory Layout 五大區段（`.text`, `.rodata`, `.data`, `.bss`, `Stack`, `Heap`）分佈、ARM 滿遞減堆疊生長方向、以及 Stack Watermark（0x55/0xA5 填充掃描）堆疊防爆棧監控機制
   - **Q2**：32-bit MCU 存取 64-bit 變數之非原子性（Non-atomic Access）與資料撕裂（Data Tearing）缺陷、`volatile` 無法保證原子性之本質、以及 PRIMASK 備份還原關中斷臨界區（Critical Section）保護
4. **自走車專案實務科普**：
   - 感測器 I2C 匯流排死鎖（I2C Bus Hang / SDA Stuck Low）實務排解
   - 9 個 SCL 時脈脈衝強制解鎖機制（I2C Bus Recovery Pattern & GPIO Bit-Banging）

---

## Part 1：白板程式題詳細覆盤

### P1：LeetCode #201 - Bitwise AND of Numbers Range
- **題目要求**：給定區間 $[left, right]$（$0 \le left \le right \le 2^{31} - 1$），回傳區間內所有整數連續做位元 AND 的運算結果。
- **最佳解法（Brian Kernighan 抹除最低位 1 尋找公共前綴）**：
  ```c
  int rangeBitwiseAnd(int left, int right) {
      while (right > left) {
          right &= (right - 1); // 每次抹除最低位的 1
      }
      return right;
  }
  ```
- **核心思維**：在數字遞增過程中，較低位元必然會翻轉出現 0。連續 AND 只要遇到一個 0 就全為 0，因此最後保留下來的只有 `left` 與 `right` 的「最高公共前綴（Common Prefix）」。
- **使用者盲點剖析（已保留於程式碼註解）**：
  - 盲點 1：使用 $O(N)$ 暴力 `for (int i = left + 1; i <= right; i++)` 累加。當 `right = 2147483647 (INT_MAX)` 時，`i++` 發生二補數有號整數溢位變成負數 `-2147483648`，導致 `i <= right` 永遠為真，引發無窮死迴圈。
  - 盲點 2：當區間長度達 20 億次時必然觸發 TLE 超時。改用 Brian Kernighan 演算法將時間縮減為最多 31 次位移，達到 $O(\log N)$ 0 ms 執行。

---

### P2：LeetCode #917 - Reverse Only Letters
- **題目要求**：給定字串 `s`，非英文字母符號位置保持不變，所有英文字母首尾全域對稱反轉。
- **最佳解法（首尾雙指標向中間靠攏）**：
  ```c
  char* reverseOnlyLetters(char* s) {
      int left = 0;
      int right = strlen(s) - 1;

      while (left < right) {
          if (!isalpha((unsigned char)s[left])) {
              left++;
          } else if (!isalpha((unsigned char)s[right])) {
              right--;
          } else {
              char temp = s[left];
              s[left] = s[right];
              s[right] = temp;
              left++;
              right--;
          }
      }
      return s;
  }
  ```
- **使用者盲點剖析（已保留於程式碼註解）**：
  - 盲點 1：誤解題意為「局部單字內部各自反轉」（如將 `"ab-cd"` 反轉為 `"ba-dc"`）。正確題意為全域首尾對調，`'a'` 與 `'d'` 對調，`'b'` 與 `'c'` 對調，產出 `"dc-ba"`。
  - 盲點 2：手刻字母範圍寫 `s[i] - 'a' <= 26` 會多算 ASCII 第 26 號偏移字元 `'{'`。正解為直接呼叫標準函式庫 `<ctype.h>` 的 `isalpha()`。

---

### P3：LeetCode #445 - Add Two Numbers II
- **題目要求**：兩個非空單向鏈結串列代表兩個高位在前的非負整數，求和並以鏈結串列回傳。
- **最佳解法（鏈結反轉 + 頭插法 Head Insertion）**：
  ```c
  static struct ListNode* reverseList(struct ListNode* head) {
      struct ListNode* prev = NULL;
      struct ListNode* curr = head;
      while (curr != NULL) {
          struct ListNode* next = curr->next;
          curr->next = prev;
          prev = curr;
          curr = next;
      }
      return prev;
  }

  struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
      l1 = reverseList(l1);
      l2 = reverseList(l2);

      int carry = 0;
      struct ListNode* res_head = NULL;

      while (l1 != NULL || l2 != NULL || carry != 0) {
          int sum = carry;
          if (l1 != NULL) {
              sum += l1->val;
              l1 = l1->next;
          }
          if (l2 != NULL) {
              sum += l2->val;
              l2 = l2->next;
          }

          carry = sum / 10;

          // 核心技巧：使用頭插法（Head Insertion），免去二次反轉結果！
          struct ListNode* node = (struct ListNode*)malloc(sizeof(struct ListNode));
          node->val = sum % 10;
          node->next = res_head;
          res_head = node;
      }

      return res_head;
  }
  ```
- **核心亮點**：使用者精準提出先反轉兩鏈結讓個位數到前端的直覺思路。更進一步採用「頭插法（`node->next = res_head; res_head = node;`）」，使先計算出的低位自然被推到串列後方，直接產出高位在前之正確結果，省去二次反轉代價！

---

## Part 2：面試致命陷阱題深度解析

### Trap 1：指標運算子優先級與解參考遞增陷阱
```c
int arr[] = {10, 20, 30, 40, 50};
int *ptr = arr;

int val1 = *ptr++;    // val1 = 10, ptr 移至 &arr[1]
int val2 = (*ptr)++;  // val2 = 20, arr[1] 變成 21, ptr 仍在 &arr[1]
int val3 = *++ptr;    // ptr 先移至 &arr[2], 取出 val3 = 30
int val4 = ++*ptr;    // 取出 arr[2] (30) 先 +1 變 31, val4 = 31
```
- **關鍵陷阱：`val2 = (*ptr)++` 為何是 20 而非 21？**
  - 後置遞增（Postfix `++`）的語言標準定義為：**回傳「遞增前之舊值」，隨後才產生寫入 +1 的副作用**。
  - 因此賦值給 `val2` 的是原始數值 `20`，而記憶體中的 `arr[1]` 則在賦值後被修改為 `21`。
- **最終陣列狀態**：`arr = {10, 21, 31, 40, 50}`。

---

### Trap 2：`const` 指標排列與 Flash/ROM 唯讀保護硬體異常
```c
const int *p1 = &x;        // 內容唯讀，指標可改指 (*p1 = 10 錯; p1 = &y 過)
int * const p2 = &x;        // 指標唯讀，內容可改 (*p2 = 20 過; p2 = &y 錯)
const int * const p3 = &x;  // 指標與內容皆唯讀 (*p3 = 30 錯; p3 = &y 錯)
```
- **面試口訣**：以星號 `*` 為界，`const` 在左內容唯讀，`const` 在右指標唯讀。
- **強制轉型寫入 Flash / ROM 常數的硬體行為**：
  - 編譯期：`(char *)firmware_version` 會騙過編譯器通過編譯。
  - 執行期：`.rodata` 燒錄於 Flash 晶片物理位址（如 `0x08000000`）。CPU 發出寫入匯流排週期（STR 指令）時，Flash 匯流排控制器或 MPU 會判定非法寫入，硬體直接觸發 **BusFault / HardFault** 導致 MCU 當機重開；在 Linux 系統則由 MMU 觸發 Page Fault，核心發送 `SIGSEGV`（Segmentation Fault）強退程式。

---

## Part 3：基本 0x10 韌體與作業系統觀念題

### 觀念題 1：Memory Layout 五大區段與 Stack Watermark 堆疊水位監控
1. **五大區段精準分佈**：
   - 韌體機器碼指令 $\implies$ **`.text`**（Flash / ROM，唯讀可執行）
   - 未初始化全域變數 $\implies$ **`.bss`**（RAM，開機 Startup Code 自動清零）
   - 有初值全域變數 $\implies$ **`.data`**（初值存於 Flash，開機複製到 RAM）
   - 區域變數 $\implies$ **`Stack`**（由 SP 堆疊指標動態上下增減）
   - `malloc()` 動態分配 $\implies$ **`Heap`**（動態記憶體池，容易產生碎片化）
2. **生長方向與 Stack Watermark 實務**：
   - **生長方向**：ARM Cortex-M 採滿遞減堆疊（Full Descending Stack），Stack 從高位址往低位址生長；Heap 則由低往高生長。
   - **Stack Watermark（堆疊水位線）技巧**：
     在韌體初始化或 FreeRTOS 建立 Task 時，將整個堆疊空間預先填滿魔術位元組（Magic Pattern，如 `0x55` 或 `0xA5`）。在系統運行或 Idle Task 中，從堆疊底部往上掃描第一個非 `0x55` 的位址，即可精準算出開機以來的**歷史最大堆疊深度（Peak Stack Usage）**，防止爆棧踩壞全域變數或引發 HardFault。

---

### 觀念題 2：ISR 與主迴圈間非原子性資料撕裂與臨界區保護
1. **資料撕裂（Data Tearing）陷阱**：
   - 32-bit CPU 暫存器寬度為 32 位元，存取 64-bit 變數（如 `uint64_t g_system_ticks`）必須分成兩條指令（兩次 `LDR`）。
   - 若讀完低 32 位（恰為 `0xFFFFFFFF`），下一個瞬間 Timer 中斷插進來執行 `g_system_ticks++`，低 32 位進位翻轉為 `0x00000000`、高 32 位加 1；中斷返回後主迴圈繼續讀取更新後的高 32 位，拼湊出的 64 位元時間會產生巨大的時間跳躍（暴衝 49.7 天）。
   - `volatile` 僅禁止編譯器快取暫存器，**完全無法保證原子性**！
2. **裸機臨界區保護（黃金標準）**：
   - 使用 **PRIMASK 備份與還原** 確保支援巢狀呼叫：
   ```c
   uint64_t get_system_ticks_safe(void) {
       uint32_t primask = __get_PRIMASK(); // 備份當前中斷狀態
       __disable_irq();                    // 關閉中斷進入臨界區
       uint64_t now = g_system_ticks;      // 安全讀取兩次 LDR
       __set_PRIMASK(primask);             // 還原原先中斷狀態
       return now;
   }
   ```

---

## Part 4：自走車專案實務科普

### 感測器 I2C 匯流排死鎖（I2C Bus Lockup / SDA Stuck Low）實務排解與 9-Clock 脈衝硬體喚醒機制

#### 1. 系統痛點：最棘手的 I2C 硬體掛死（Bus Hang）
在雙核心智慧車專案中，車身姿態由陀螺儀/加速度計（如 MPU6050 / IMU）透過 I2C 匯流排高頻回傳：
- **意外發生場景**：當 MCU 正在透過 I2C 讀取 IMU 暫存器資料時，Slave（IMU）正在傳送資料並把 **SDA 線路拉低（Low）**。
- 此時若車體震動造成接觸不良、電源暫降、或是 MCU 觸發了 Watchdog 軟體復位（Reset）：
  - **MCU 重開機**：MCU 內部狀態機復位，準備重新初始化 I2C。
  - **Slave（IMU）未重開機**：IMU 沒有收到重開訊號，它的內部硬體狀態機依然停留在「等待 Master 發出下一個 SCL Clock 脈衝，以便釋放 SDA」！
- **致命僵局**：
  - MCU 的硬體 I2C 模組一偵測匯流排，發現 SDA 線路被持續拉在 Low，硬體判斷匯流排為 **BUSY**，無法發送 START 條件！
  - MCU 在開機階段呼叫 `HAL_I2C_Init()` 或 `I2C_Start()` 直接死鎖逾時（Timeout）卡死。
  - 即使按了 MCU 上的 Reset 按鈕 100 次也沒用，除非將整台車「拔掉電池完全斷電」冷重啟！

```
Master (MCU)   : 重開機後等待 SDA 變 High ... (一直偵測到 Low, 判斷匯流排 Busy 死鎖)
Slave (IMU)    : 等待 Master 送出下一個 SCL Clock ... (SDA 保持拉低 Low 不放手)
--------------------------------------------------------------------------------
【死鎖僵局】：雙方都在乾等對方，整個 I2C 匯流排永久卡死！
```

---

#### 2. 解決方案：9 個 SCL Clock 脈衝解鎖常式（I2C Bus Recovery Pattern）
根據 NXP I2C 官方規範，若 Slave 卡在輸出低電位位元的狀態，Master 只要在 **SCL 腳位手動送出最多 9 個 Clock 脈衝**：
- Slave 每收到一個 SCL 脈衝，就會移出一個位元。
- 一個 I2C Byte 最多 8 個資料位元 + 1 個 ACK 位元，共 9 個時脈。
- 在 9 個脈衝之內，Slave 必然會將最後一個位元移出，並釋放 SDA（由外部上拉電阻將 SDA 拉回 High）！
- Master 隨後送出一個 STOP 訊號，強制 Slave 的硬體狀態機回到 Idle 待命狀態！

---

#### 3. 核心程式碼實作（開機前的 GPIO Bit-Banging 復位法）
在初始化 MCU 硬體 I2C 之前，先將 SCL 與 SDA 配置為一般 GPIO 開漏輸出（Open-Drain）執行解鎖：

```c
void I2C_Bus_Recovery(void) {
    // 1. 將 SCL 與 SDA 暫時配置為標準 GPIO Output (Open-Drain, Pull-up)
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_SCL | GPIO_PIN_SDA;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    // 2. 檢查 SDA 是否被 Slave 釘在 Low
    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_SDA) == GPIO_PIN_RESET) {
        // 3. 連續發出 9 個 SCL Clock 脈衝，迫使 Slave 移出未傳完的 Byte
        for (int i = 0; i < 9; i++) {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_SCL, GPIO_PIN_RESET);
            Delay_us(5);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_SCL, GPIO_PIN_SET);
            Delay_us(5);

            // 若 Slave 提早釋放 SDA（回到 High），即可提前跳出
            if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_SDA) == GPIO_PIN_SET) {
                break;
            }
        }

        // 4. 手動產生一個標準的 STOP 條件（SCL High 時，SDA 由 Low 轉 High）
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_SDA, GPIO_PIN_RESET);
        Delay_us(5);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_SCL, GPIO_PIN_SET);
        Delay_us(5);
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_SDA, GPIO_PIN_SET);
        Delay_us(5);
    }

    // 5. 解鎖完成！切換回硬體 I2C 周邊模式進行正規初始化
    I2C_Hardware_Peripheral_Init();
}
```

---

#### 4. 面試應答精華（Phison & 電子五哥頂級亮點）
- **面試官問**：「你的自走車上有 IMU 陀螺儀，遇到過 I2C 通訊異常或死鎖嗎？怎麼排解的？」
- **標準滿分應答**：
  > 「遇到過！當 MCU 發生重啟或遇到干擾時，若剛好卡在 Slave 正在傳送資料並把 SDA 拉為低電位，重開機後的 MCU 會因為 SDA 為 Low 判定匯流排 Busy 而永久卡死，即使熱重啟 MCU 也無法復原。
  > 我們在系統初始化前實作了 **I2C 匯流排復位機制（I2C Bus Recovery Pattern）**：先將 SCL/SDA 切換為 GPIO 開漏模式，若偵測到 SDA 被拉低，主動在 SCL 發送 **9 個 Clock 脈衝**迫使 Slave 釋放 SDA，最後手動送出 STOP 條件讓 Slave 狀態機復位，最後才切回硬體 I2C 初始化。這項機制徹底解決了車載感測器偶發性匯流排死鎖，大幅提升了車體嵌入式韌體的強健度！」
