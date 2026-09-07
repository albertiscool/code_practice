# Day 82: 合併排序鏈結串列、移動零與全真面試實戰模擬 (ROS視覺伺服/static底層/DMA與Cache無效化)

今天維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」極致流暢訓練節奏。在零提示狀態下，連續攻克 2 大科技廠高頻經典白板題（合併兩個已排序的鏈結串列 [Dummy Head 穿針引線一鍵收尾]、移動零 [快慢雙指標原地覆蓋]），並在全真模擬面試中 3 題全數滿分通關（期末自駕車 ROS 視覺伺服閉迴路追蹤與環視搜尋、`static` 記憶體區段與內部鏈結封裝、DMA 與 Cache Invalidate 資料一致性深層原理）！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【合併兩個已排序的鏈結串列 (Merge Two Sorted Lists - LeetCode #21)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (全台灣所有科技廠/IC 廠白板題霸榜前 3 名)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day82_MergeTwoLists_MoveZeroes_ROS_Static_DMA_Review/p1.c)
* **演算法**：虛擬頭節點 (Dummy Head) 雙指針穿針引線，時間 $O(N + M)$，空間 $O(1)$。
* **核心邏輯**：
  * 防呆守門員：若其中一條為空，直接回傳另一條。
  * 建立區域變數 `struct ListNode dummy = {0, NULL};`，指針 `curr = &dummy;`。
  * 雙指針比較：`while (list1 != NULL && list2 != NULL)`，誰小就先接誰，接完指針後移。
  * **一鍵收尾大招**：迴圈結束後，將剩餘未走完的一整段直接拼接：`curr->next = (list1 != NULL) ? list1 : list2;`。
  * 回傳真實頭節點 `dummy.next`。

---

### 2. 題目 2【移動零 (Move Zeroes - LeetCode #283)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day82_MergeTwoLists_MoveZeroes_ROS_Static_DMA_Review/p2.c)
* **演算法**：快慢雙指標原地覆蓋法 (Fast & Slow Pointers In-Place)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * `slow` 與 `fast` 指標起點皆為 0。
  * 快指標 `fast` 走訪陣列，只要遇到非 0 數字，就直接覆蓋到慢指標位置：`nums[slow] = nums[fast]; slow++;`。
  * 遍歷完畢後，所有非 0 數字已按原本順序緊湊排在 `0 .. slow-1`。
  * 第二個 `while` 迴圈俐落將 `slow` 後面的所有剩餘位置全部補 0：`nums[slow] = 0; slow++;`。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自駕車專題：ROS Visual Servoing 視覺伺服閉迴路追蹤】
* **偏差計算與動態轉向**：
  * YOLO 節點檢測到目標後回傳中心座標 $x$。控制器計算畫面中心偏差：`error = x - center_x`（中心為 320 像素）。
  * 設立容許死區 (Tolerance Deadband, ±40px)：超出死區時暫停前進 (`linear.x = 0`)，輸出角速度 (`angular.z = ±0.3 rad/s`) 旋轉對準；進入死區後直行追蹤 (`linear.x = 0.2 m/s`)。
* **目標遺失容錯搜尋 (In-Place Search Mode)**：
  * 當目標物移出視野 (`len(data) == 0`)，小車自動切換為原地旋轉環視搜尋模式，直到目標重新進入相機畫面。

---

### 🗣️ 題 2【0x10 底層：`static` 關鍵字在 C 語言的三大作用】
1. **修飾區域變數**：
   * 儲存位置由 Stack (堆疊) 改移至 **.data 區（已初始化）或 .bss 區（未初始化/清零）**。
   * 生命週期延長為「整個程式生命週期」，函式結束數值不丟失，但可見範圍 (Scope) 依然安全限制於函式內。
2. **修飾全域變數 / 函式**：
   * 將符號由外部鏈結改為 **「內部鏈結 (Internal Linkage)」**，僅本 `.c` 檔案可見。
   * 其他模組無法 `extern` 存取，杜絕命名衝突 (Multiple Definition Error)，實現模組封裝。

---

### 🗣️ 題 3【硬體與底層驅動：DMA 與 Cache 數據一致性 (Cache Coherency)】
* **讀取舊資料 (Stale Data) 的根因**：
  * DMA 搬移資料直接寫入 SRAM，完全繞過了 CPU 的 D-Cache。
  * 若 CPU 之前已經將該記憶體區段快取至 D-Cache 中，CPU 去讀該變數時會命中 Cache (Hit)，直接讀到未更新的舊資料！
* **解決方案（Invalidate vs Clean）**：
  * **DMA RX (周邊 ➔ RAM ➔ CPU讀)**：在 CPU 讀取前，必須呼叫 **Cache Invalidate（無效化快取）**，強迫清掉 Cache Line 的有效位，逼迫 CPU 重新去 RAM 讀取 DMA 搬來的最新資料！
  * **DMA TX (CPU寫 ➔ RAM ➔ DMA發送)**：在 DMA 啟動傳輸前，必須呼叫 **Cache Clean / Flush（清空/刷寫快取）**，強迫 Cache 將最新資料立刻寫回 RAM，防止 DMA 搬到過期的舊資料！
