# Day 76: 2 的冪次方、二元搜尋樹第 K 小元素 (Medium 中序走訪)、無重複最長子字串 (Medium 滑動窗口) 與 0x10 volatile/BASEPRI 深度實戰

今天順利完成了 3 大經典題目（2 的冪次方 [Brian Kernighan 位元抹除神技]、BST 第 K 小元素 [Medium 中序單調遞增走訪]、無重複字元的最長子字串 [Medium 滑動窗口與跳躍更新]），並全面覆蓋了 0x10 嵌入式 C 語言與微控制器核心命題（`volatile` 硬體輪詢防編譯器死迴圈優化、FreeRTOS `BASEPRI` 臨界區中斷遮蔽 vs `PRIMASK`、I2C 多主機開漏線與仲裁機制）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【2 的冪次方 (Power of Two - LeetCode #231)】
* **難度**：🟢 Easy | 出題頻率：🔥🔥🔥🔥🔥 (IC 廠經典位元題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day76_PowerOfTwo_KthBST_LongestSubstring_0x10_Review/p1.c)
* **演算法**：Brian Kernighan 位元消除法，時間 $O(1)$，空間 $O(1)$。
* **核心邏輯**：
  * 2 的冪次方二進位表示中「恰好只有 1 個 1」（如 $4 = 100_2, 8 = 1000_2$）。
  * $n - 1$ 會將該位元的 1 變成 0，並將其右側所有 0 變成 1。
  * 核心表達式：`(n > 0) && ((n & (n - 1)) == 0)`（注意 `==` 優先級高於 `&` 必須加括號）。

---

### 2. 題目 2【二元搜尋樹中第 K 小的元素 (Kth Smallest Element in a BST - LeetCode #230)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥 (BST 單調性經典神題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day76_PowerOfTwo_KthBST_LongestSubstring_0x10_Review/p2.c)
* **演算法**：BST 中序走訪 (In-Order Traversal)，時間 $O(H + k)$，空間 $O(H)$。
* **核心邏輯**：
  * BST 中序走訪（左 ➔ 中 ➔ 右）輸出保證由小到大嚴格遞增。
  * 遞迴走訪到當前節點時執行 `(*count)++`，當 `*count == k` 時當前節點數值即為答案，提前剪枝返回。

---

### 3. 題目 3【無重複字元的最長子字串 (Longest Substring Without Repeating Characters - LeetCode #3)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥 (全球大廠面試 Top 3 超級神題)
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day76_PowerOfTwo_KthBST_LongestSubstring_0x10_Review/p3.c)
* **演算法**：滑動窗口 + 上次出現位置陣列 (Sliding Window with Last-Seen Array)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 開一個長度 128 的陣列 `last_pos[128]` 記錄 ASCII 字元上次出現的索引。
  * 右邊界 `right` 逐一擴展窗口；若遇到重複字元且在當前窗口內（`last_pos[c] >= left`），左邊界 `left` 瞬間跳躍至 `last_pos[c] + 1`！
  * 單次走訪 $O(N)$ 原地秒殺。

---

## 📚 今日 0x10 & 硬體底層面試精華 (Embedded C & Hardware Architecture)

### 1. `volatile` 在暫存器輪詢中的不可或缺性
* **編譯器優化陷阱**：未加 `volatile` 時，編譯器（`-O2`/`-O3`）發現迴圈內沒有改寫 `*status_reg`，會將其值預先讀入 CPU 暫存器（如 R0），產生 `LDR R0, [status_reg]` 後直接進行 `CMP R0, #0; BEQ loop` 的**無窮死迴圈**，再也不會從實體硬體暫存器讀取最新狀態！
* **`volatile` 救贖**：強迫 CPU 每次進入迴圈都必須「老老實實重新發出硬體匯流排讀取指令」。

### 2. ARM Cortex-M 中斷遮蔽暫存器：`BASEPRI` vs `PRIMASK`
* **`PRIMASK` (`__disable_irq()`)**：暴力關閉除 NMI 和 HardFault 以外的「所有可遮蔽中斷」，會破壞硬即時性。
* **`BASEPRI` (`taskENTER_CRITICAL()`)**：僅遮蔽優先級低於或等於 `configMAX_SYSCALL_INTERRUPT_PRIORITY` 的中斷。
* **優勢**：保留超高優先級的硬即時中斷（如馬達緊急煞車、過流保護 ISR）在進入作業系統臨界區時**依然能即刻被響應**！

### 3. I2C 多主機仲裁 (Multi-Master Arbitration) 與線與 (Wired-AND)
* **線與特性**：開漏 (Open-Drain) 輸出下，只要有任一設備輸出 LOW (0)，整條 SDA 線即被拉為 LOW。
* **仲裁判定**：主機在發送 SDA 同時會「即時回讀 SDA 線上的電位」。當 Master A 嘗試發送 HIGH (1) 卻讀到 LOW (0) 時，代表有另一個 Master B 正在發送 LOW。
* **退讓機制**：Master A 在該 bit 判定自己仲裁失敗，立即關閉 SDA 輸出驅動，退回從機監聽狀態，無縫避免資料碰撞！
