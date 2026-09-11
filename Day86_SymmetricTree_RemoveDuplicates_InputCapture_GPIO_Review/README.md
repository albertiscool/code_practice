# Day 86: 對稱二元樹、刪除排序重複項與微處理機核心硬體周邊 (超音波輸入捕獲/指標陷阱/GPIO推挽與開漏)

今天維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」高效率訓練節奏。題 3 正式全面切換為**【方案 A：微處理機核心硬體周邊】**，將訓練重心深度對齊微處理機助教與硬體整合強項！今天攻克了 2 大高頻白板題（對稱二元樹 [DFS 鏡像外側對外側、內側對內側]、刪除排序陣列重複項 [單層 Fast-Slow 快慢雙指針原地覆寫]），並在口試中深入剖析了超音波定時器輸入捕獲 0% CPU 原理、C 語言指標陣列優先級陷阱、以及 I2C Open-Drain 開漏輸出防短路與線與機制！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【對稱二元樹 (Symmetric Tree - LeetCode #101)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (Google、聯發科、瑞昱高頻經典題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day86_SymmetricTree_RemoveDuplicates_InputCapture_GPIO_Review/p1.c)
* **演算法**：DFS 雙節點鏡像走訪，時間 $O(N)$，空間 $O(H)$。
* **核心邏輯**：
  * **主函式防禦**：`if (root == NULL) return true;`
  * **輔助函式 4 步守門員 (`checkMirror(t1, t2)`)**：
    1. 雙空：`if (t1 == NULL && t2 == NULL) return true;`
    2. 單空：`if (t1 == NULL || t2 == NULL) return false;`
    3. 數值異：`if (t1->val != t2->val) return false;`
    4. 照鏡子遞迴：外側與外側對齊，內側與內側對齊：
       ```c
       return checkMirror(t1->left, t2->right) && checkMirror(t1->right, t2->left);
       ```
* **面試加分點**：注意到題目限制條件為節點數 $\ge 1$，在白板上可主動向主管說明「雖然題意保證非空，但安全韌體實務上會加上 `root == NULL` 防護」，展現深厚工程素養。

---

### 2. 題目 2【刪除排序陣列中的重複項 (Remove Duplicates from Sorted Array - LeetCode #26)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠必考原地陣列操作)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day86_SymmetricTree_RemoveDuplicates_InputCapture_GPIO_Review/p2.c)
* **演算法**：快慢雙指針（Fast-Slow Pointers），時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * `slow = 0` 記錄已確認的不重複序列尾端。
  * `fast` 從 1 開始單向向後掃描：
    ```c
    for (int fast = 1; fast < numsSize; fast++) {
        if (nums[fast] != nums[slow]) {
            slow++;
            nums[slow] = nums[fast]; // 發現新元素，搬移至 slow 下一個位置
        }
    }
    return slow + 1;
    ```
* **⚠️ 實作盲點複習**：
  * 雙指針覆寫切勿使用巢狀 `while`。若內層 `while (nums[slow] == nums[fast])` 缺少 `fast < numsSize` 邊界檢查，在全重複陣列（如 `[1, 1, 1]`）時會導致 `fast` 衝出陣列引發 Buffer Overflow 越界讀取，甚至覆寫未知垃圾記憶體。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自走車專案實務：超音波避障測距與 Timer Input Capture】
* **工作原理**：
  * MCU 對 Trig 給予 $10\,\mu\text{s}$ 脈衝，模組發射 8 個週期的 40 kHz 超音波。
  * 發射瞬間 Echo 拉高（**上升緣代表開始出發**），收到回波瞬間 Echo 拉低（**下降緣代表反射接收**）。
* **Polling 死等缺陷**：無障礙物時 Echo 可維持達 30ms，若用 `while` 輪詢會導致 CPU 完全凍結，小車盲開 3 公分，錯失馬達 PID 與 UART 通訊。
* **Timer Input Capture（輸入捕獲 - 0% CPU 與零抖動）**：
  * 硬體邊緣檢測器在上升緣時瞬間將 Timer `CNT` 鎖存至 `CCR1`；下降緣時鎖存至 `CCR2` 並觸發單次中斷。
  * 全程純硬體數位邏輯捕捉，完全不依賴 CPU 執行軟體，**零中斷延遲（Zero Jitter）**！
* **除以 58 的整數優化**：
  * 聲速 $340\text{ m/s} = 0.034\text{ cm}/\mu\text{s}$，來回需除以 2。
  * $\text{Distance (cm)} = \frac{\Delta t}{2 / 0.034} = \frac{\Delta t}{58.82} \approx \frac{\Delta t}{58}$。
  * 在無硬體 FPU 的單晶片上，用純整數除以 58 能大幅節省 CPU 運算週期！

---

### 🗣️ 題 2【0x10 C / 底層記憶體：指標陣列、陣列指標與函式指標應用】
* **記憶口訣**：「括號優先看，沒括號看右邊（`[]` 大於 `*`）」。
  1. `int *p[5];` ➔ **指標陣列**（Array of Pointers）：長度為 5 的陣列，每個元素存放一個 `int*` 指標。
  2. `int (*p)[5];` ➔ **陣列指標**（Pointer to an Array）：一個單一指標，指向長度為 5 的整數陣列。
  3. `int (*p)(int, int);` ➔ **函式指標**（Function Pointer）：指向接收兩 int 回傳 int 函式的指標。
* **實務關鍵場景**：
  * **Callback 回呼與 HAL 驅動抽象**：如 Linux 核心的 `struct file_operations`（`.open`, `.read`, `.write` 全為函式指標），實現驅動與 OS 解耦。
  * **FSM 狀態機轉移表**：用函式指標陣列替代巨大臃腫的 `switch-case`，實現 $O(1)$ 快速狀態分發。

---

### 🗣️ 題 3【🎯 微處理機核心硬體周邊：GPIO Push-Pull vs. Open-Drain 深度對決】
* **MOSFET 底層結構**：
  * **Push-Pull（推挽）**：上臂 PMOS + 下臂 NMOS。輸出 1 時 PMOS 導通主動推向 VDD，輸出 0 時 NMOS 導通主動拉向 GND。
  * **Open-Drain（開漏）**：僅有下臂 NMOS（漏極懸空開路）。只能主動拉低（0V），輸出 1 時呈高阻抗浮接（High-Z）。
* **為什麼 Open-Drain 一定需要上拉電阻？**
  * 因為缺乏上臂 PMOS，NMOS 關閉時引腳浮空無電位，必須靠外部上拉電阻（如 $4.7\,\text{k}\Omega$）溫和拉上高電位。
* **為什麼 I2C 強制只能用 Open-Drain？**
  1. **防止總線短路衝突（Bus Contention）**：若使用推挽，當 Master 送 1（連 VDD）而 Slave 送 0（連 GND）時，電源與地線直接短路，瞬間大電流會燒毀晶片引腳！
  2. **實現「線與（Wired-AND）」**：只要任一節點下拉，全線即為 0。這賦予了 I2C 支援**多主機無損仲裁**與**時鐘延展（Clock Stretching，從機拉低 SCL 暫停通訊）**的強大物理特性！
