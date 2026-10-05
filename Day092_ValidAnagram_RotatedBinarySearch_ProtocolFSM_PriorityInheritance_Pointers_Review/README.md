# Day 92: Valid Anagram, Search in Rotated Array, 二進位訊框協議, RTOS 優先權繼承, 右左法則與函式指標

## 📚 學習大綱與實作總結

本日完成 2 道科技廠/IC 設計廠高頻白板程式題，並依新配置進行 3 道全真口試實務抽考：雙核心二進位訊框協議與 FSM 防死鎖、FreeRTOS 優先權翻轉與動態優先權繼承、0x10 右左法則進階指標解析與「函式 vs 函式指標 vs 指標函式」本質辨析。

---

## 💻 白板程式題實作與檢討

### 題目 1：有效的字母異位詞 (Valid Anagram - LeetCode #242)
- **檔案**：[`p1.c`](./p1.c)
- **難度**：🟢 Easy | **出題頻率**：🔥🔥🔥🔥🔥
- **核心算法**：哈希頻率表計數（ASCII Hash Counting），時間複雜度 $O(N)$，額外空間複雜度 $O(1)$（固定 26 個整數）。
- **滿分實作亮點**：
  - 開頭防禦性檢查：`s == NULL || t == NULL`。
  - 提前剪枝優化：`strlen(s) != strlen(t)` 快速淘汰。
  - 單趟走訪：在同一迴圈內同時做 `word[s[i] - 'a']++` 與 `word[t[i] - 'a']--`，並在最後驗證是否全為 0。

### 題目 2：搜尋旋轉排序陣列 (Search in Rotated Sorted Array - LeetCode #33)
- **檔案**：[`p2.c`](./p2.c)
- **難度**：🟡 Medium | **出題頻率**：🔥🔥🔥🔥🔥
- **核心算法**：改良型二分搜尋法（Binary Search），時間複雜度 $O(\log N)$，額外空間複雜度 $O(1)$。
- **旋轉陣列二分關鍵性質**：
  - 陣列以 `mid` 切分後，**「左半段」與「右半段」必定至少有一半是嚴格單調遞增有序的**。
  - 若 `nums[left] <= nums[mid]`，表示左半段完全有序；若 `target` 落在 `[nums[left], nums[mid])` 之間，則往左縮小（`right = mid - 1`），否則往右（`left = mid + 1`）。
  - 若右半段完全有序；若 `target` 落在 `(nums[mid], nums[right]]` 之間，則往右縮小（`left = mid + 1`），否則往左（`right = mid - 1`）。
- **盲點檢討**：
  - 終止條件必須為 `while (left <= right)`，若寫成 `while (last >= 0)` 會導致無窮迴圈卡死。
  - 每次計算 `mid` 後應第一時間判定 `if (nums[mid] == target) return mid;`。

---

## 🎙️ 全真面試口試抽考精要

### 題 1：自走車專案實務 - 雙核心通訊二進位訊框協議與 FSM 容錯

#### 1. 為什麼棄用純 ASCII（如 `"1.5,0.8\n"`）？
- **CPU 運算開銷大**：MCU 缺乏硬體 FPU，執行 `sscanf`、`atof` 會消耗大量 CPU 週期與堆疊空間。
- **頻寬浪費與延遲**：二進位 `float` 僅佔 4 Bytes，ASCII 字串需 9~10 Bytes，耗費超過兩倍的 UART 頻寬。
- **標準二進位訊框格式**：
  ```text
  [ Header: 2B ] [ Length: 1B ] [ CmdID: 1B ] [ Payload: N Bytes ] [ CRC16: 2B ] [ Tail: 2B ]
  ```
  - **Header/Tail**：`0xAA 0x55` 雙位元組特徵碼，用於狀態機對齊與邊界防護。
  - **Payload**：純二進位格式打包，接收端可透過指標直接映射至結構體，達到 **零拷貝/零轉換開銷**。

#### 2. 校驗失敗處置與狀態機防死鎖（FSM Error Recovery）
- **校驗碼不匹配（Checksum Mismatch）**：
  - 實時運動控制遵循「時效性原則」：**直接拋棄該錯誤封包，不要求重傳**，避免 ACK/NACK 引入延遲抖動（Latency Jitter），等待下一週期最新指令。
- **位元組間超時定時器（Inter-byte Timeout）**：
  - 每次收到 Byte 刷新定時器，若逾時（如 $5\text{ms}$）未收到下一個 Byte，立即判定訊框殘損，**強制重置狀態機回到 `WAIT_HEADER` 並清空緩衝區**，杜絕狀態機卡死。

---

### 題 2：0x10 C 語言底層核心 / RTOS - 優先權翻轉與優先權繼承

#### 1. 優先權翻轉（Priority Inversion）成因
- Task L（低）持有共享資源互斥鎖（Mutex）。
- Task H（高）搶佔 CPU 並試圖獲取該 Mutex，因鎖被持有而進入阻塞狀態（Blocked）。
- 不相關的 Task M（中）甦醒，因優先權 $M > L$ 搶佔了 Task L 的 CPU 執行權。
- **結果**：Task L 無法執行而無法釋放鎖，導致最高優先權的 Task H 被中優先權的 Task M 間接無限期卡死。

#### 2. FreeRTOS 動態優先權繼承（Priority Inheritance）
- **提升時機**：只有當高優先權 Task H 試圖獲取 Mutex 並被阻塞的瞬間，排程器將持有鎖的 Task L 優先權**暫時提升至與 Task H 相同（或排隊等待者中的最高優先權 $\max$）**。
- **降回時機**：當 Task L 呼叫 `xSemaphoreGive()` 釋放鎖時，排程器立即將 Task L 降回原原本的 Base Priority。

#### 3. Mutex vs Binary Semaphore 本質差異
- **Mutex（擁有權 Ownership）**：具有「誰借誰還」的擁有權約束，排程器明確記錄持有者 TCB，故能精確實作優先權繼承。
- **Binary Semaphore（無擁有權 / 任務同步 Signaling）**：通常由中斷 ISR `Give`、任務 `Take`，無特定持有者，無法實作優先權繼承。

---

### 題 3：🔥 0x10 進階指標解析、右左法則與宣告手寫

#### 1. 「函式 vs 函式指標 vs 指標函式」本質辨析
- **一般函式 (Function)**：`int add(int a, int b);` $\to$ 呼叫執行程式碼。
- **指標函式 (Pointer-returning Function)**：`int* get_ptr(void);` $\to$ **本質是函式**，回傳一個記憶體位址（指標）。
- **函式指標 (Function Pointer)**：`int (*p_func)(int, int);` $\to$ **本質是指標變數**，儲存函式的入口位址。

#### 2. 右左法則（Right-Left Rule）宣告解析
- ① `int (*func_ptr)(int, int);` $\to$ **函式指標**（指向接收兩個 `int` 並回傳 `int` 的函式）。
- ② `int (*handler_table[5])(void);` $\to$ **函式指標陣列**（包含 5 個無參數且回傳 `int` 的函式指標之陣列）。
- ③ `int *(*p)[10];` $\to$ **指向指標陣列的指標**（指向包含 10 個 `int*` 指標之陣列的指標）。
- ④ `void (*signal(int sig, void (*func)(int)))(int);` $\to$ **函式**，接收 `sig` 與處理常式函式指標，並回傳舊的信號處理函式指標 `void (*)(int)`。

#### 3. 手寫宣告標準解答
- **A**（指向包含 8 個 float 陣列之指標）：
  `float (*p_arr)[8];`
- **B**（包含 4 個接收 `void*` 且回傳 `bool` 的函式指標陣列）：
  `bool (*action_list[4])(void*);`
