# Day 77: 手寫字串原地反轉、主要元素 (摩爾投票法)、找出不同字元 (XOR 異或法) 與 0x10 Watchdog/PWM 深度實戰

今天執行「手感極佳、科技廠與韌體面試命中率 100% 的經典題」訓練節奏。順利攻克了 3 大經典白板題（手寫字串原地反轉 [首尾雙指標對撞 swap]、主要元素 [Boyer-Moore 摩爾投票演算法]、找出字串的不同字元 [全域 XOR 異或消去法]），並 100% 全對通關了 0x10 嵌入式 C 語言與微控制器核心命題（`const` 指標 3 大修飾順序解讀、Watchdog 看門狗硬體 Reset 原理與為何嚴禁在 Timer ISR 餵狗、馬達 PWM 20kHz 超音波靜音機制）！

---

## 🏆 今日 3 大白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【手寫字串原地反轉 (In-Place String Reversal - 韌體經典題)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (系統廠與韌體面試必考底層題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day77_InPlaceStrrev_MajorityElement_FindTheDifference_0x10_Review/p1.c)
* **演算法**：首尾雙指標原地對撞法，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 加入 `s == NULL || *s == '\0'` 防呆守門員。
  * 自力走訪計算字串長度 `len`（不依賴標準庫）。
  * 設 `left = 0, right = len - 1`，當 `left < right` 時原地交換字元並向中間收縮。

---

### 2. 題目 2【主要元素 (Majority Element - LeetCode #169)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠經典演算法)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day77_InPlaceStrrev_MajorityElement_FindTheDifference_0x10_Review/p2.c)
* **演算法**：摩爾投票演算法 (Boyer-Moore Voting Algorithm)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 利用主要元素佔比超過一半（$> 50\%$）的特性，進行「一對一同歸於盡」抵消大亂鬥。
  * 擂台空了 (`count == 0`) 換新候選人 (`candidate = nums[i], count = 1`)。
  * 同陣營 `count++`，不同陣營 `count--` 互相抵消。最後留在擂台上的必為主要元素！

---

### 3. 題目 3【找出字串的不同字元 (Find the Difference - LeetCode #389)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (IC 廠經典異或消除題)
* **原始碼檔案**：[`p3.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day77_InPlaceStrrev_MajorityElement_FindTheDifference_0x10_Review/p3.c)
* **演算法**：全域 XOR 異或消去法 ($a \oplus a = 0$)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 初始化 `result = 0`。
  * 將字串 `s` 與 `t` 的所有字元全部 `^=` 異或在一起。
  * 所有原本成對的字元全部兩兩抵消為 0，最後留下的就是多加進來的那顆唯一字元！

---

## 📚 今日 0x10 & 硬體底層面試精華 (Embedded C & MCU Architecture)

### 1. `const` 指標修飾順序解讀
* **`const char *p;`**：指向常數字元的指標。指向的內容 `*p` 不能改，指標變數位址 `p` 可以改。
* **`char * const p;`**：常數指標。指標變數位址 `p` 固定不能改，指向的內容 `*p` 可以改。
* **`const char * const p;`**：指向常數字元的常數指標。指標位址與指向內容皆不可改！

### 2. Watchdog Timer (看門狗計時器 - WDT) 原理與 ISR 餵狗禁忌
* **硬體原理**：看門狗是 MCU 內部獨立運作的倒數計時器。正常程式必須定時「餵狗 (Reload)」將計數器加滿；若程式當機或死鎖導致計時器歸零 (Timeout)，看門狗硬體會直接拉低晶片內部的 RESET 腳位強制系統重置。
* **ISR 餵狗致命禁忌**：若將餵狗指令放在定時中斷 Timer ISR 內部，當主任務 `while(1)` 或 RTOS 核心死鎖時，Timer ISR 依然會在背景正常觸發並默默餵狗，導致**看門狗永遠不會逾時，MCU 永遠無法重啟**，徹底失去保護作用！
* **正確做法**：在最底層的主迴圈中餵狗，或由健康檢查任務監控所有關鍵任務全部正常打卡後才執行餵狗。

### 3. 馬達 PWM 頻率與 20kHz 超音波靜音機制
* **人耳聽覺極限**：人耳可聽頻率範圍約為 $20 \text{ Hz} \sim 20 \text{ kHz}$。
* **高頻噪音成因**：當 PWM 頻率設定在 $1 \text{ kHz} \sim 2 \text{ kHz}$ 時，馬達電磁線圈隨之產生每秒數千次的微小物理振動，落入人類敏感聽覺區間產生刺耳嗡嗡聲。
* **20kHz 靜音方案**：將 PWM 頻率提高至 $20 \text{ kHz}$ 以上，使振動頻率進入超音波頻段，超越人耳聽覺上限，達成極致靜音運轉。
