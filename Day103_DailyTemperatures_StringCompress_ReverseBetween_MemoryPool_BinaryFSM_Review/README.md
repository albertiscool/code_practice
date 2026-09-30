# Day 103: 一線 IC 設計廠「感測時序堆疊 × 通訊封包原地壓縮 × 區間頭插反轉 × Memory Pool 記憶體池」戰場模式

## 📑 今日訓練概覽
- **P1 (單調堆疊與感測器時序)**: LeetCode #739: Daily Temperatures (Medium) - 單調遞減堆疊 O(N) 結算天數
- **P2 (通訊協定與雙指標原地壓縮)**: LeetCode #443: String Compression (Medium) - 原地遊程編碼 (RLE) 讀寫雙指標與逆向數值翻轉
- **P3 (鏈結串列與區間反轉)**: LeetCode #92: Reverse Linked List II (Medium) - 哨兵節點 (Dummy Node) 與精準 (left-1) 步頭插法 (Head Insertion)
- **架構特輯 (底層記憶體管理)**: [`memory_pool.c`](./memory_pool.c) - 工業級高效率零開銷 O(1) Memory Pool (Free-List 鏈結串列雙重身分 Union 實作)
- **自走車專案實務科普 (第 103 講)**: 跨晶片通訊為什麼「傳字串」會死人？二進位幀狀態機 (Binary Frame FSM) + 0xAA 0x55 同步頭 + 1-Byte 對齊 + XOR Checksum (已落實於 [`car/protocol/`](../car/protocol/))
- **0x10 韌體架構情境題**:
  1. 跨晶片通訊結構體記憶體對齊大雷：64-bit ARM Linux (12 Bytes) vs 8-bit AVR (7 Bytes)、未對齊匯流排拆分兩次存取 (Split Bus Transaction) 與撕裂讀取 (Torn Read)、`#pragma pack(push, 1)` 與 C11 `static_assert` 編譯期自動防禦。
  2. 中斷服務常式 (ISR) 的天條禁忌：為什麼不能拿 Mutex（無 TCB 無法休眠、引發永久死鎖 Deadlock）、為什麼不能 `malloc`（不可重入性 Non-Reentrant 踩爛 Heap 鏈結串列、破壞硬即時確定性）、FreeRTOS 正統 `FromISR` API 與 Queue / Task Notification 設計模式。

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Daily Temperatures (#739 - Medium)
- **盲點 1 (`sizeof(指標)` 陷阱)**: `int* result = malloc(...); memset(result, 0, sizeof(result));` 在 64-bit 系統上 `sizeof(result)` 永遠固定只有 8 Bytes，導致僅清空前 2 個 int，其餘數萬個元素全是未初始化記憶體垃圾！正解：推薦直接使用 `calloc(N, sizeof(int))` 一步到位歸零。
- **盲點 2 (`fast = slow + 1` 死迴圈)**: 迴圈內部無條件重置指針導致原地踏步死迴圈。
- **盲點 3 (時間複雜度)**: 暴力法雙層迴圈為 $O(N^2)$，在 $N = 10^5$ 時高達 $10^{10}$ 次操作，必然觸發 TLE。
- **核心武器 (單調遞減堆疊)**: 堆疊內儲存「還在等待更熱天氣的天數下標 (index)」，遇更高溫天數一口氣結算彈出，整體複雜度精確收斂為 $O(N)$。

### P2: String Compression (#443 - Medium)
- **盲點 1 (ASCII 偏移量遺失)**: 數值轉字元 `(char)(count % 10)` 在 C 語言中得到的是控制字元 `\x01` 而非字元 `'1'`，必須補上 `'0'` (ASCII 48) 偏移量。
- **盲點 2 (陣列越界與結尾遺漏)**: `chars[fast]` 缺乏邊界保護造成未定義存取，且結尾重複字元無法進入結算。
- **盲點 3 (全域記帳 vs 連續壓縮)**: 本題是遊程編碼 (Run-Length Encoding, RLE)，只需統計連續相同字元。
- **核心架構 (讀寫雙指標)**: `read` 指針探測長度，`write` 指針原地覆寫。數值拆解時倒著寫，再利用雙指標原地翻轉回正常順序，達成嚴格 $O(1)$ 額外空間。

### P3: Reverse Linked List II (#92 - Medium)
- **4 行核心頭插法 (Head Insertion)**:
  ```c
  struct ListNode* next_node = curr->next;
  curr->next = next_node->next;
  next_node->next = pre->next;
  pre->next = next_node;
  ```
- **盲點 (指針定位步數偏差)**: `pre` 從 `&dummy` 出發，只需走 `left - 1` 步即可停在錨點節點，若走 `left` 步會多走一步導致反轉區間向後平移。

---

## 🏊 Memory Pool (固定區塊配置器) 核心解析
- **本質**: 一條預先在 `.bss` 靜態配置好的 Free-List 單向鏈結串列。
- **借出 (Alloc)**: 鏈結串列 **Pop Head**（`s_free_head = s_free_head->next;`），耗時 2 個 CPU 指令，$O(1)$ 確定性。
- **歸還 (Free)**: 鏈結串列 **Push Head 頭插法**（`block->next = s_free_head; s_free_head = block;`），耗時 2 個 CPU 指令，$O(1)$ 確定性。
- **零開銷 Union 技巧**:
  ```c
  typedef union Block {
      union Block* next;           // 空閒時充當鏈結串列 next 指標
      uint8_t data[BLOCK_SIZE];    // 借出時整塊充當使用者資料緩衝區
  } Block_t;
  ```
  在空閒與借出之間共用記憶體，每區塊額外指標開銷為 **0 Byte**！

---

## 🛰️ 自走車二進位幀協議與狀態機 (已上線)
- 檔案：[`car/protocol/packet_protocol.h`](../car/protocol/packet_protocol.h), [`fsm_parser.c`](../car/protocol/fsm_parser.c), [`arduino_binary_receiver_demo.ino`](../car/protocol/arduino_binary_receiver_demo.ino), [`rpi_binary_sender_demo.py`](../car/protocol/rpi_binary_sender_demo.py)
- 架構手冊：[`car/Binary_Protocol_FSM_Guide.md`](../car/Binary_Protocol_FSM_Guide.md)
