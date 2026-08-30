# Day 74: 盛最多水的容器 (Medium 雙指標貪心)、合併排序鏈結串列 (Dummy 穿針引線)、位元計數 (位元 DP) 與 0x10 SPI/FPU 核心深度實戰

今天繼續執行「3 題白板手寫 Code + 3 題高頻面試觀念抽考」的高效訓練節奏。順利攻克了 LeetCode 3 大經典白板題（盛最多水的容器 [Medium 對撞雙指標貪心演算法]、合併兩個已排序鏈結串列 [Dummy Head 穿針引線]、位元計數 [位元轉移 DP 演算法]），並 100% 全對通關了 0x10 嵌入式 C 語言與硬體通訊核心命題（`sizeof(a++)` 編譯期不求值陷阱、ISR 嚴禁浮點運算與 FPU 惰性壓棧/定點數方案、SPI Mode 0 與 Mode 3 CPOL/CPHA 時脈時序判定）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【盛最多水的容器 (Container With Most Water - LeetCode #11)】
* **難度**：🟡 **Medium (中等題)** | 出題頻率：🔥🔥🔥🔥🔥 (大廠雙指標經典題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day74_ContainerWater_MergeLists_CountBits_0x10_Review/p1.c)
* **演算法**：對撞雙指標貪心法 (Two-Pointer Greedy)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 雙指標從首尾 `left = 0`, `right = n - 1` 向中間收縮。
  * 面積計算公式：`area = min(height[left], height[right]) * (right - left)`。
  * 核心貪心策略：**每次移動較矮的那一側指標**（若移動較高側寬度變小且受限於較矮側面積必減小；只有移動較矮側才可能遇到更高的柱子使面積變大！）。

---

### 2. 題目 2【合併兩個已排序鏈結串列 (Merge Two Sorted Lists - LeetCode #21)】
* **難度**：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (白板必考穿針引線題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day74_ContainerWater_MergeLists_CountBits_0x10_Review/p2.c)
* **演算法**：Dummy Head 穿針引線法，時間 $O(N + M)$，空間 $O(1)$。
* **核心邏輯**：
  * 建立 `dummy` 假節點作為錨點，`curr` 指針依序比較 `list1->val` 與 `list2->val`，較小者接在 `curr->next`。
  * `while (list1 != NULL && list2 != NULL)` 迴圈結束後，將剩餘未走完的一整串節點直接接在 `curr->next`，回傳 `dummy.next`。

---

### 3. 題目 3【位元計數 (Counting Bits - LeetCode #338)】
* **難度**：🟢 Easy / 🟡 Medium | 出題頻率：🔥🔥🔥🔥🔥 (IC 廠位元動態轉移題)
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day74_ContainerWater_MergeLists_CountBits_0x10_Review/p3.c)
* **演算法**：位元動態規劃轉移 (Bitwise DP)，時間 $O(N)$ 線性時間，空間 $O(1)$。
* **核心邏輯**：
  * `i >> 1` 代表切掉數字 $i$ 的最後一個 bit。
  * 狀態轉移公式：`ans[i] = ans[i >> 1] + (i & 1)`（即 $i$ 內 1 的總數 ＝ 切掉後剩下的 1 個數 ＋ 最低位是否為 1）。

---

## 📚 今日 0x10 & 硬體底層面試精華 (Embedded C & Hardware Architecture)

### 1. `sizeof` 運算式不求值特性 (Unevaluated Operands)
* `sizeof` 是 **編譯期運算子 (Compile-Time Operator)**，不是執行期函式。
* 編譯器僅檢查括號內運算式的型態並直接替換為常數大小（如 4 Bytes），括號內的 `a++` 運算式在執行期根本不會產生任何機器指令，因此 `a` 的數值保持不變！

### 2. 中斷服務常式 (ISR) 嚴禁浮點數運算
* **無硬體 FPU 晶片**：純軟體模擬浮點運算耗費數百個指令週期，造成嚴重中斷延遲。
* **有硬體 FPU 晶片 (Cortex-M4F/M7)**：觸發硬體「惰性壓棧 (Lazy Stacking)」，強迫額外壓棧 17 個浮點暫存器 (S0~S15, FPSCR)，使中斷延遲與 Stack 記憶體開銷翻倍。
* **解決方案**：採用 **定點數 (Fixed-Point Arithmetic)** 或整數放大法（如 `temp_x100 = 2534`）。

### 3. SPI Mode 0 與 Mode 3 CPOL/CPHA 時脈時序
* **Mode 0 (CPOL=0, CPHA=0)**：時脈線 SCK 閒置為 **LOW (低電位)**，資料在 **第 1 個跳變沿（上升沿 Rising Edge）** 採樣。
* **Mode 3 (CPOL=1, CPHA=1)**：時脈線 SCK 閒置為 **HIGH (高電位)**，資料在 **第 2 個跳變沿（上升沿 Rising Edge）** 採樣。
