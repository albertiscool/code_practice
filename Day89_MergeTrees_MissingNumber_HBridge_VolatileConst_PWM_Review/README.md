# Day 89: 合併二元樹、遺失的數字與微處理機核心硬體周邊 (馬達H橋反電動勢急停/const volatile/Timer PWM解析度計算)

今天維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」高效率訓練節奏。今天攻克了 2 大高頻白板題（合併二元樹 [DFS 原地指標嫁接]、遺失的數字 [神級 XOR 消除律零溢位秒殺]），並在口試中深入拆解了馬達 H 橋反電動勢能耗制動（Dynamic Braking）、C 語言 `const volatile` 在硬體唯讀暫存器的本質應用、以及 STM32 Timer 預分頻與 20kHz PWM 3600 階解析度精密推導！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【合併二元樹 (Merge Two Binary Trees - LeetCode #617)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (聯發科、瑞昱、群聯二元樹遞迴經典題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day89_MergeTrees_MissingNumber_HBridge_VolatileConst_PWM_Review/p1.c)
* **演算法**：DFS 雙樹深度優先走訪，時間 $O(\min(N, M))$，空間 $O(\min(H_1, H_2))$。
* **核心邏輯（原地嫁接法）**：
  * 若 `root1 == NULL` 直接回傳 `root2`（整枝子樹直接嫁接，不需額外配置記憶體）。
  * 若 `root2 == NULL` 直接回傳 `root1`。
  * 兩者皆非空：`root1->val += root2->val;`。
  * 左右指標接軌：`root1->left = mergeTrees(root1->left, root2->left);` 與 `root1->right = mergeTrees(root1->right, root2->right);`。
* **⚠️ 實作盲點複習**：
  * 切勿只檢查 `root1 == NULL && root2 == NULL`，否則在形狀不對稱時直接取 `root1->val` 會引發致命 Segfault 崩潰！
  * 遞迴呼叫時必須將回傳值重新掛載回 `root1->left/right`，否則 Tree 2 的子節點無法嫁接至 Tree 1。

---

### 2. 題目 2【遺失的數字 (Missing Number - LeetCode #268)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大 IC 設計廠位元運算/數論必考題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/程式訓練/Day89_MergeTrees_MissingNumber_HBridge_VolatileConst_PWM_Review/p2.c)
* **演算法**：XOR 互斥或消除律，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 利用 `a ^ a = 0` 與 `a ^ 0 = a` 特性：
    ```c
    int res = numsSize; // 先放入最大邊界 n
    for (int i = 0; i < numsSize; i++) {
        res ^= (i ^ nums[i]); // 同時 XOR 下標 i 與元素 nums[i]
    }
    return res;
    ```
  * 出現兩次的數字全部自動抵消歸零，最後唯一落單留下的就是遺失數字。
* **⚠️ 實作盲點複習**：
  * 高斯求和公式 $\frac{n(n+1)}{2}$ 項數為 $n+1$ 項，在 $n$ 很大時存在 32-bit 整數溢位風險，因此韌體與晶片面試首選「XOR 位元消除法」。
  * 初始值必須設為 `res = numsSize`，因為迴圈下標只跑到 `numsSize - 1`，若設為 0 會少放最大數字 $n$ 導致計算失真；且迴圈不能跑到 `numsSize`，否則讀取 `nums[numsSize]` 會引發陣列越界 Buffer Overflow。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【自走車專案實務：馬達 H 橋驅動 (L298N) 與動態能耗制動 (Dynamic Braking)】
* **H 橋 4 大開關狀態**：
  1. 正轉：Q1 (上左) + Q4 (下右) ON。
  2. 反轉：Q3 (上右) + Q2 (下左) ON。
  3. 滑行（Coast）：全關（斷路懸空，靠機械摩擦力慢慢滑行）。
  4. 急停（Brake）：Q2 + Q4 同時 ON（馬達兩端同時接地短路）。
* **反電動勢能耗制動原理**：
  * 電源切斷後，旋轉慣性使馬達變身「發電機」，切割磁力線產生反電動勢（Back-EMF）。
  * 兩端接地短路形成極低電阻迴路，瞬間爆發巨大反向短路電流。
  * 根據**楞次定律**，感應電流產生強大的反向電磁阻尼力矩（Braking Torque），在數毫秒內將動能轉化為線圈焦耳熱，實現瞬間急停。

---

### 🗣️ 題 2【0x10 C / 底層記憶體：`volatile` 與 `const` 混合修飾詞的深層意義】
1. `const int a;`：唯讀變數，軟體不可修改。
2. `volatile int b;`：易變變數，禁止編譯器優化緩存至暫存器，每次強制從實體記憶體讀取。
3. `const volatile int c;`：**軟體只准唯讀不能改，但外部硬體會隨時更動**！
   * 經典場景：**硬體唯讀狀態暫存器**（如 `UART_SR` 接收旗標、`GPIO_IDR` 按鈕引腳電位、`RTC_TR` 即時時鐘）。
4. `volatile const int * const p;`：位址固定不可變的常數指標（`* const p`），指向軟體唯讀但硬體會隨時更新的暫存器記憶體（`volatile const int *`），為 ARM CMSIS 標準周邊暫存器定義語法。

---

### 🗣️ 題 3【🎯 微處理機核心硬體周邊：Timer 預分頻器 (PSC) 與 PWM 頻率/佔空比解析度計算】
* **題目設定**：系統時脈 $72\,\text{MHz}$，目標輸出頻率 $20\,\text{kHz}$，`PSC = 0`。
* **1. ARR 自動重載暫存器計算**：
  $$\text{PWM 頻率} = \frac{f_{\text{CLK}}}{(\text{PSC} + 1) \times (\text{ARR} + 1)}$$
  $$20\,\text{kHz} = \frac{72\,\text{MHz}}{(0 + 1) \times (\text{ARR} + 1)} \implies \text{ARR} + 1 = 3600 \implies \mathbf{ARR = 3599}$$
* **2. 佔空比解析度與 CCR 寫入值**：
  * **解析度階數**：$\text{ARR} + 1 = \mathbf{3600\ \text{階}}$（約 $11.8\ \text{bits}$，最小微調刻度 $\frac{1}{3600} \approx 0.0278\%$）。
  * **25% 佔空比對應 CCR**：
    $$\text{CCR} = (\text{ARR} + 1) \times 25\% = 3600 \times 0.25 = \mathbf{900}$$
