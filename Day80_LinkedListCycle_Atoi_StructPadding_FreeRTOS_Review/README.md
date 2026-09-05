# Day 80: 環形鏈結串列、字串轉整數 (atoi) 與全真面試實戰模擬 (馬達反電動勢急煞/Struct對齊/FreeRTOS Mutex vs Semaphore)

今天達成 **Day 80** 重大里程碑！在完全無提示的純白板實戰中，以極高效率攻克了 2 大經典白板題（環形鏈結串列 [Floyd 快慢雙指標判圈法一次 100% 滿分通過]、字串轉整數 [my_atoi 三階段依序掃描]），並在全真模擬面試中連續 3 題全對通關（自駕車 L298N 反電動勢動態煞車機制、結構體 Padding 計算與記憶體重排優化、FreeRTOS Mutex 所有權 vs Semaphore 同步與優先級繼承）！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【環形鏈結串列 (Linked List Cycle - LeetCode #141)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻經典題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day80_LinkedListCycle_Atoi_StructPadding_FreeRTOS_Review/p1.c)
* **演算法**：Floyd 龜兔賽跑快慢雙指標法 (Tortoise and Hare)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 初始化 `slow = head, fast = head`。
  * 迴圈條件：`while (fast != NULL && fast->next != NULL)`。
  * `slow` 每次走 1 步，`fast` 每次走 2 步。若串列中存在環，`fast` 必然會在環內自後方追上並與 `slow` 相遇 (`slow == fast`)，回傳 `true`！
  * 若 `fast` 抵達結尾 `NULL`，代表無環，回傳 `false`。

---

### 2. 題目 2【字串轉換整數 (String to Integer - atoi 核心版)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (系統廠與韌體底層必備)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day80_LinkedListCycle_Atoi_StructPadding_FreeRTOS_Review/p2.c)
* **演算法**：三階段線性循序掃描 (Three-Phase Linear Scan)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * **階段 1**：跳過所有前導空格 `while (s[i] == ' ') i++;`。
  * **階段 2**：檢查單一正負號 `'+'` 或 `'-'`，若為 `'-'` 則記錄 `sign = -1`。
  * **階段 3**：連續讀取數字字元 `while (s[i] >= '0' && s[i] <= '9')`，執行十進位累加：`result = result * 10 + (s[i] - '0');`。一遇到非數字字元或結尾 `\0` 立即停止。
  * 回傳 `sign * result`。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自駕車專題：L298N 馬達急煞與反電動勢 (Back-EMF)】
* **PWM = 0 慣性滑行 (Coasting)**：
  * 當 PWM 設為 0 時，H 橋內部電晶體全部斷開處於高阻抗開路狀態，馬達失去驅動力但會受小車慣性帶動持續滑行長達數十公分，無法應對緊急防撞。
* **動態制動急煞 (Dynamic Braking)**：
  * **控制指令**：`IN1 = LOW, IN2 = LOW, PWM = 255`。
  * **物理機制**：將馬達兩端直接接地短路。旋轉中的馬達化身為發電機，兩端短路形成強大感應電流。依據 **冷次定律 (Lenz's Law)**，感應電流產生巨大的反向電磁煞車力矩，達成瞬間急煞停！

---

### 🗣️ 題 2【0x10 底層：結構體對齊 (Struct Padding) 計算與優化】
```c
struct SensorData {
    char status; // 1 Byte (位址 0)
                 // [Padding 3 Bytes] (位址 1~3，為讓 int 對齊 4 的倍數)
    int id;      // 4 Bytes (位址 4~7)
    short temp;  // 2 Bytes (位址 8~9)
                 // [Padding 2 Bytes] (位址 10~11，因總大小需為最大成員 4 的倍數)
}; // sizeof = 1 + 3 + 4 + 2 + 2 = 12 Bytes
```
* **RAM 優化方案**：成員按照資料型態大小 **「由大到小」** 重新排列：
  ```c
  struct SensorData_Optimized {
      int id;      // 4 Bytes (位址 0~3)
      short temp;  // 2 Bytes (位址 4~5)
      char status; // 1 Byte  (位址 6)
                   // [Padding 1 Byte] (位址 7)
  }; // sizeof = 8 Bytes (節省 33% 記憶體空間！)
  ```

---

### 🗣️ 題 3【FreeRTOS 作業系統：Mutex 互斥鎖 vs Binary Semaphore 二值信號量】
* **核心哲學差異**：
  * **Mutex（互斥鎖 - 資源保護）**：具有 **「所有權 (Ownership)」** 概念（**誰借誰還**）。只有成功 `Take` 獲取 Mutex 的 Task 才有權限執行 `Give` 釋放。
  * **Binary Semaphore（二值信號量 - 任務同步）**：無所有權概念（**誰都能還**）。通常用於一個任務通知另一個任務（例如 ISR 發信號給背景 Task 處理數據）。
* **ISR 中斷釋放**：
  * 只有 Binary Semaphore 支援在中斷中使用 `xSemaphoreGiveFromISR()`。
  * **Mutex 嚴格禁止在中斷 ISR 中使用**（因為 ISR 沒有 Task 身份與優先級概念，無法擁有 Mutex 所有權）。
* **優先級繼承 (Priority Inheritance)**：
  * **只有 Mutex 支援** 優先級繼承機制，防止高優先級任務被無邊界優先級反轉 (Unbounded Priority Inversion) 卡死；Semaphore 則無此機制。
