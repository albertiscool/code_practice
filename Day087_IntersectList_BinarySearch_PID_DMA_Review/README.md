# Day 87: 相交鏈結串列、二分搜尋法與微處理機核心硬體周邊 (馬達PID閉迴路/記憶體對齊陷阱/DMA雙緩衝乒乓機制)

今天維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」高效率訓練節奏。今天攻克了 2 大高頻白板題（相交鏈結串列 [雙指針浪漫相遇法，邊界 NULL 同步對齊]、二分搜尋法 [左閉右閉區間、防止整數溢位 `mid = first + (last - first) / 2`]），並在口試中深入拆解了自走車雙輪差速 PID 閉迴路控制本質、C 語言結構體對齊在 32-bit 與 64-bit 下的經典陷阱、以及 DMA 雙緩衝（Ping-Pong Buffer）HT/TC 無縫流水線原理！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【相交鏈結串列 (Intersection of Two Linked Lists - LeetCode #160)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (聯發科、瑞昱、群聯、台達白板必考經典題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day87_IntersectList_BinarySearch_PID_DMA_Review/p1.c)
* **演算法**：雙指針長度消除 / 浪漫相遇法，時間 $O(m + n)$，空間 $O(1)$。
* **核心邏輯**：
  * 指針 A 走過：$A \to \text{NULL} \to B \to \text{NULL}$；指針 B 走過：$B \to \text{NULL} \to A \to \text{NULL}$。
  * 雙方總走步數皆為 $m + n + 1$：
    ```c
    while (currA != currB) {
        currA = (currA == NULL) ? headB : currA->next;
        currB = (currB == NULL) ? headA : currB->next;
    }
    return currA;
    ```
  * 若兩鏈結串列相交，兩指針會在交點相遇；若不相交，兩指針會**同時抵達 NULL**，安全跳出迴圈回傳 `NULL`。
* **⚠️ 實作盲點複習**：
  * 切勿在 `curr = curr->next;` 變成 NULL 時「立即在同一個迴圈強制換頭」。若當前指針無法以 NULL 的狀態參與 `while (currA != currB)` 比較，兩串列不相交時會陷入**死循環（Infinite Loop / TLE）**！

---

### 2. 題目 2【二分搜尋法 (Binary Search - LeetCode #704)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (一線科技廠/IC設計廠白板手寫最高頻演算法)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day87_IntersectList_BinarySearch_PID_DMA_Review/p2.c)
* **演算法**：二分搜尋（Binary Search），時間 $O(\log N)$，空間 $O(1)$。
* **核心邏輯**：
  * 區間定義：左閉右閉 `[first, last]`（`first = 0, last = numsSize - 1`）。
  * 終止條件：`while (first <= last)`。
  * **防溢位計算**：`int mid = first + (last - first) / 2;`（避免 `(first + last) / 2` 在大數字時產生整數溢位）。
  * 根據 `nums[mid]` 與 `target` 比對，嚴格收縮區間為 `mid + 1` 或 `mid - 1`。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自走車專案實務：雙馬達差速轉向與 PID 閉迴路控制】
* **為什麼相同的 PWM 無法走直線？**
  * 馬達線圈阻抗公差、齒輪箱機械摩擦力差異、左右輪胎磨損與地面阻力不對稱，導致開迴路控制下兩輪轉速必然不同。
* **光電編碼器與閉迴路控制**：
  * 編碼器每秒回傳脈衝數換算為實際轉速 $v_{\text{actual}}$，計算誤差 $e(t) = v_{\text{target}} - v_{\text{actual}}$。
* **P、I、D 的物理角色**：
  1. **P（比例）➔ 看「現在」**：誤差越大踩越重。痛點是無法消除**靜態誤差（Steady-State Error）**，最後殘留的微小力道會被摩擦力吃掉。
  2. **I（積分）➔ 看「過去」**：把歷史微小誤差日積月累疊加，推升 PWM **徹底消滅靜態誤差**，命中目標速度。
  3. **D（微分）➔ 看「未來」**：檢測誤差變化的斜率，在接近目標速度時**提前產生反向阻尼（煞車）**，抑制超調（Overshoot）與震盪。

---

### 🗣️ 題 2【0x10 C / 底層記憶體：記憶體對齊與 64-bit 陷阱】
```c
struct A { char a; int b; short c; }; // 12 Bytes
struct B { int b; short c; char a; }; // 8 Bytes
```
* **對齊計算分析**：
  * `struct A`：`char a` (1) + 3 padding + `int b` (4) + `short c` (2) + 2 padding = **12 Bytes**。
  * `struct B`：`int b` (4) + `short c` (2) + `char a` (1) + 1 padding = **8 Bytes**。
* **⚠️ 64-bit 面試大陷阱**：
  * 64-bit 下這兩個結構體**依然是 12 與 8 Bytes**！因為內部僅有 `char` (1B), `short` (2B), `int` (4B)，完全沒有指標（Pointer）或 `long`，最大成員依然是 4 Bytes 的 `int`，對齊基準完全不變！
* **非對齊存取後果**：
  1. 效能懲罰：跨 Word 邊界需花費兩次 Bus Read Cycle 加上移位拼接，記憶體頻寬腰斬。
  2. 硬體異常：低階 MCU（ARM Cortex-M0）或特定 64-bit / 浮點指令直接引發 **`UsageFault` / `HardFault` 導致系統重開機**！

---

### 🗣️ 題 3【🎯 微處理機核心硬體周邊：DMA 與雙緩衝機制 (Ping-Pong Buffer)】
* **為什麼高頻取樣必須用 DMA？**
  * 在 100 kHz 等高速取樣下，普通中斷（ISR）頻繁壓棧出棧會霸佔 100% CPU。DMA 為獨立硬體控制器，直接在周邊暫存器與 SRAM 間搬運數據，**CPU 負擔為 0%**。
* **雙緩衝（Ping-Pong Buffer）流水線機制**：
  * 配置 $2N$ 連續緩衝區（前半段 Buffer A，後半段 Buffer B）。
  * **半滿觸發 HT 中斷（Half-Transfer）**：DMA 繼續往 Buffer B 寫入，CPU 同時安全處理 Buffer A。
  * **全滿觸發 TC 中斷（Transfer-Complete）**：DMA 繞回 Buffer A 寫入，CPU 同時安全處理 Buffer B。
  * 達成零等待、零複製、無並行衝突（No Race Condition）的即時數據流。
