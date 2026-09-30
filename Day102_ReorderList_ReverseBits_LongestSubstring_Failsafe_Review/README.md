# Day 102: 一線 IC 設計廠「黃金三角均衡分配」戰場模式

## 📑 今日訓練概覽
- **P1 (指標與核心資料結構)**: LeetCode #143: Reorder List (重排鏈結串列 - 找中點 + 反轉後半 + 拉鍊穿插)
- **P2 (位元操作與底層暫存器)**: LeetCode #190: Reverse Bits (顛倒 32 位元無號整數 - 移位法與 O(1) 分治平行交換)
- **P3 (演算法與滑動窗口)**: LeetCode #3: Longest Substring Without Repeating Characters (無重複字元最長子字串 - ASCII 記帳與邊界防倒退)
- **自走車專案實務科普 (第 102 講)**: 雙核心架構之 Failsafe 通訊看門狗 (300ms) 與反電動勢 (Back-EMF) 動態急煞實作
- **0x10 韌體架構情境題**: 
  1. Cortex-M3 BSRR/BRR 暫存器 vs ODR 讀改寫 (RMW) 競態與遺失更新 (Lost Update)
  2. HardFault Stack Frame dump (EXC_RETURN Bit 2 判斷 MSP/PSP) 與 Stack Overflow 踩壞返回位址機制 (FreeRTOS Canary)

---

## 🎯 題目盲點複盤與關鍵收穫

### P1: Reorder List (#143)
- **盲點 1 (NULL 指標解引用)**: `pre = NULL; pre->next = head;` 會直接觸發 Segfault。
- **盲點 2 (未切斷串列)**: 找完中點後必須執行 `slow->next = NULL`，徹底斷開前後兩段，否則後續穿插會互相指涉形成循環迴圈 (Cycle)。
- **盲點 3 (雙指針拉鍊法)**: 兩條獨立鏈結串列交錯合併口訣：「先備份雙方 next -> 交叉接上 -> 雙雙往前進」。
  ```c
  struct ListNode *p1_next = p1->next;
  struct ListNode *p2_next = p2->next;
  p1->next = p2;
  p2->next = p1_next;
  p1 = p1_next;
  p2 = p2_next;
  ```

### P2: Reverse Bits (#190)
- **盲點 (未賦值陳述句)**: 原始寫法 `n >> 1;` 沒有賦值回 `n`，導致在 32 次迴圈中 `n & 1` 永遠只讀到最原本的 Bit 0。必須寫成 `n >>= 1;`。
- **O(1) 分治平行交換 (Divide and Conquer)**:
  5 步摺紙遮罩法交換 16 位元、8 位元 (Byte/Endianness)、4 位元 (Nibble)、2 位元、1 位元，完全無分支 (Branchless)。

### P3: Longest Substring Without Repeating Characters (#3)
- **盲點 1 (陣列初始化陷阱)**: 在 C 語言中，`int arr[256] = {-1};` 只有第 0 格是 -1，其餘 255 格全部是 0！必須使用 `memset(arr, -1, sizeof(arr))`。
- **盲點 2 (更新位置遺失)**: 遇到重複字元時，更新完 `slow` 必須同時將該字元的最新位置 `last_pos[c] = fast` 記下。
- **盲點 3 (abba 倒退陷阱)**: 只有當重複字元出現在「當前窗口內部 (`last_pos[c] >= slow`)」時，`slow` 才可以往前跳，避免 `slow` 倒退走。
- **底層記憶體安全**: 使用 `(unsigned char)s[fast]` 防止 `char` 預設為 signed 導致負數陣列越界存取。

---

## 🚗 自走車 Failsafe 與動態煞車專案實作
- **Arduino 端**: [`car/final/car_controll/car_controll.ino`](../car/final/car_controll/car_controll.ino)
  - 實作 300ms 通訊看門狗 `millis() - lastHeartbeatTime > 300`
  - 實作主動動態電磁煞車 `brakeCar_Dynamic()` (IN1=LOW, IN2=LOW, ENA=255)，將馬達短路利用反電動勢煞停，距離縮短至 8cm (縮減 82%)。
- **樹莓派 ROS 端**: [`car/final/balloon_ws/src/serial_bridge/serial_bridge/serial_bridge_node.py`](../car/final/balloon_ws/src/serial_bridge/serial_bridge/serial_bridge_node.py)
  - 實作 10 Hz 週期心跳產生器 (`heartbeat_timer`) 發送 `'H'`
  - 實作 `shutdown_hook` 安全關閉鉤子 (Ctrl+C 時自動下發動態煞車 `'S'`)
- **架構手冊**: [`car/Failsafe_Dynamic_Braking_Guide.md`](../car/Failsafe_Dynamic_Braking_Guide.md)
