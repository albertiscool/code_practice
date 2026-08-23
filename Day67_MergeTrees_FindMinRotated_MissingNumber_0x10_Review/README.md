# Day 67: 合併二元樹、旋轉陣列二分搜尋 (Medium)、缺失數字與 0x10 嵌入式 C 語言硬核觀念大滿貫

今天繼續執行「3 題白板手寫 Code + 3 題高頻面試觀念抽考」的黃金訓練節奏。順利攻克了 LeetCode 3 大經典白板題（合併二元樹、尋找旋轉排序陣列最小值 [Medium 中等題]、缺失的數字），並 100% 全對通關了 0x10 嵌入式 C 語言三大核心命題（memcpy vs memmove 記憶體重疊機制、static inline 核心優勢、大小端序與 32-bit 位元組反轉）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【合併兩個二元樹 (Merge Two Binary Trees - LeetCode #617)】
* **難度**：🟢 Easy
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day67_MergeTrees_FindMinRotated_MissingNumber_0x10_Review/p1.c)
* **演算法**：前序遞迴合併法，時間 $O(N)$，空間 $O(H)$。
* **核心邏輯**：
  * Base Cases：若 `root1` 為空回傳 `root2`，若 `root2` 為空回傳 `root1`。
  * 兩節點皆存在：`root1->val += root2->val`，並分別遞迴合併左右子樹接回 `root1->left` 與 `root1->right`。

---

### 2. 題目 2【尋找旋轉排序陣列中的最小值 (Find Min in Rotated Array - LeetCode #153)】
* **難度**：🟡 **Medium (中等題)**
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day67_MergeTrees_FindMinRotated_MissingNumber_0x10_Review/p2.c)
* **演算法**：二分搜尋法 (Binary Search)，時間 $O(\log N)$，空間 $O(1)$。
* **單邊對照定海神針**：
  * 將 `nums[mid]` 與最右邊的 `nums[high]` 進行比較。
  * 若 `nums[mid] > nums[high]` ➡️ 代表左半邊為遞增大數，最小值必在右半邊 (`low = mid + 1`)。
  * 否則 ➡️ 最小值必在 `mid` 本身或左半邊 (`high = mid`)。
  * 迴圈當 `low == high` 時收斂於最小值！

---

### 3. 題目 3【缺失的數字 (Missing Number - LeetCode #268)】
* **難度**：🟢 Easy
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day67_MergeTrees_FindMinRotated_MissingNumber_0x10_Review/p3.c)
* **演算法**：高斯等差級數總和法 / XOR 抵消法，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 理論總和 `sum = n * (n + 1) / 2`，走訪陣列將出現的數字逐一扣除（`sum -= nums[i]`），剩餘差額即為缺號。

---

## 📚 今日 0x10 嵌入式 C 語言面試精華 (Embedded C Core Interview Concepts)

### 1. `memcpy` vs `memmove` 記憶體重疊安全差異
* `memcpy`：假設記憶體不重疊，由左往右單向複製。若 `dst` 重疊於 `src` 後端，會將尚未複製的原始資料提前覆蓋打亂。
* `memmove`：判斷位址高低：若 `dst > src`（`dst` 在後）採 **從後往前 (Back to Front)** 複製；若 `dst < src` 採 **從前往後** 複製，100% 確保重疊記憶體安全。

### 2. `static inline` 內聯函式 3 大硬核優勢
1. **嚴格型態檢查 (Type Safety)**：相比 `#define` 純文字替換，編譯器會進行型態比對與警告。
2. **消除巨集副作用 (Zero Side Effect)**：參數先求值後傳入，解決如 `MIN(*p++, b)` 導致指標重複遞增跳頁問題。
3. **支援 GDB 斷點除錯**：保留符號資訊，可在偵錯工具中設定斷點與單步執行。
4. **`static` 作用**：防止多個 `.c` include 該標頭檔時引發連結器 `multiple definition` 重複定義錯誤。

### 3. 小端序 (Little-Endian) 與 32-bit 位元組反轉巨集 `SWAP32`
* **小端序存放順序**：`0x12345678` 於記憶體低位址至高位址依序為 **`0x78 0x56 0x34 0x12`** (小端：低位元組放低位址)。
* **32-Bit 位元組反轉公式**：
  ```c
  #define SWAP32(x) ( ((x) & 0xFF000000U) >> 24 | \
                      ((x) & 0x00FF0000U) >> 8  | \
                      ((x) & 0x0000FF00U) << 8  | \
                      ((x) & 0x000000FFU) << 24 )
  ```
