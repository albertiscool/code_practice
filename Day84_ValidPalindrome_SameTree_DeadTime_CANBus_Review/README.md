# Day 84: 驗證迴文字串、相同的樹與全真面試實戰模擬 (Inverter死區時間/指針退化/CAN Bus顯隱性與終端電阻)

今天維持「九月研替衝刺：全真零提示白板手寫 + 技術主管口試實戰」高效率訓練節奏。在零提示狀態下，攻克了 2 大經典白板題（驗證迴文字串 [首尾雙指針對撞 + isalnum/tolower 忽略符號大小寫]、相同的樹 [雙樹同步遞迴 3 步守門員]），並在全真模擬面試中深度拆解車載與電控核心難題（車用 Traction Inverter 死區時間 Dead-Time 防直通短路、C 語言陣列傳參退化 Decay 與 32/64-bit 指針大小、車用 CAN Bus 顯隱性無損仲裁與 120 歐姆終端電阻反射抑制）！

---

## 🏆 今日 2 大全真白板手寫題精華 (Whiteboard Coding)

### 1. 題目 1【驗證迴文字串 (Valid Palindrome - LeetCode #125)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠白板高頻題)
* **原始碼檔案**：[`p1.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day84_ValidPalindrome_SameTree_DeadTime_CANBus_Review/p1.c)
* **演算法**：首尾對撞雙指針 (Two Pointers)，時間 $O(N)$，空間 $O(1)$。
* **核心邏輯**：
  * 左指針 `first = 0`，右指針 `last = strlen(s) - 1`。
  * 當 `first < last` 時：
    * 利用 `while (first < last && !isalnum(s[first])) first++;` 跳過非英數字元。
    * 利用 `while (first < last && !isalnum(s[last])) last--;` 跳過非英數字元。
    * 核心比對：`if (tolower(s[first]) != tolower(s[last])) return false;`（注意：`tolower` 不會原地修改字串，必須比對其回傳值）。
    * 比對成功後，雙指針同步向中間收縮：`first++; last--;`。
  * 走訪完畢皆無衝突，回傳 `true`。

---

### 2. 題目 2【相同的樹 (Same Tree - LeetCode #100)】
* **難度**：🟢 **Easy** | 出題頻率：🔥🔥🔥🔥🔥 (各大科技廠二元樹必考題)
* **原始碼檔案**：[`p2.c`](file:///c:/Users/a0907/Desktop/%E7%A8%8B%E5%BC%8F%E8%A8%93%E7%B7%B4/Day84_ValidPalindrome_SameTree_DeadTime_CANBus_Review/p2.c)
* **演算法**：DFS 雙樹同步深度優先遞迴，時間 $O(\min(N, M))$，空間 $O(H)$。
* **核心邏輯（經典 3 步守門員）**：
  1. `if (p == NULL && q == NULL) return true;` ➔ 兩者皆為空，結構相同。
  2. `if (p == NULL || q == NULL) return false;` ➔ 其中一者為空另一者不為空，結構不同（同時防範 NULL 指標取值 Segfault！）。
  3. `if (p->val != q->val) return false;` ➔ 數值不同。
  4. 同步遞迴驗證左右子樹：`return isSameTree(p->left, q->left) && isSameTree(p->right, q->right);`。

---

## 🎙️ 今日全真技術面試模擬口試題 (Mock Technical Interview)

### 🗣️ 題 1【車用電控/逆變器：死區時間 (Dead-Time) 與直通短路】
* **物理根因**：功率開關元件 (MOSFET / IGBT / SiC) 具有閘極充放電電容，**關斷時間 (Turn-Off) 通常慢於開啟時間 (Turn-On)**。
* **直通短路 (Shoot-Through)**：若互補 PWM 同時切換而未加延遲，上臂尚未完全關斷時下臂已開啟，高壓直流母線 (400V/800V) 會直接對地短路，瞬間產生數百安培毀滅性電流炸毀功率模組。
* **Dead-Time 保護機制**：在互補切換瞬間插入微秒級死區時間，**強制讓上下臂同時處於關斷 (OFF) 狀態**，確保一顆徹底熄火後才准開啟另一顆。

---

### 🗣️ 題 2【0x10 底層：陣列名 vs 陣列指標退化 (Decay)】
```c
void func(int arr[10]) {
    printf("%zu\n", sizeof(arr)); // 32-bit 印出 4，64-bit 印出 8 (指標大小)
}
int main() {
    int a[10];
    printf("%zu\n", sizeof(a));   // 印出 40 (完整陣列 10 * 4 Bytes)
    func(a);
}
```
* **指標退化 (Array Decay)**：C 語言為了防止函式傳參時整塊記憶體拷貝造成 Stack 溢位與效能低落，強制將陣列參數退化為「指向首元素的指標 (`int *`)」。
* **架構差異**：32-bit 架構下指標佔 4 Bytes；64-bit 架構下指標佔 8 Bytes。

---

### 🗣️ 題 3【車用與工控：CAN Bus 顯性/隱性電位與 120 歐姆終端電阻】
* **顯性 (Dominant) vs 隱性 (Recessive)**：
  * **顯性 (Dominant) ＝ 邏輯 0**（差動電壓 $\Delta V \approx 2\text{V}$，驅動線路）。
  * **隱性 (Recessive) ＝ 邏輯 1**（差動電壓 $\Delta V \approx 0\text{V}$，高阻抗懸空）。
  * **仲裁優先級**：**顯性 (0) 壓過 隱性 (1)**（線與 Wired-AND 原理）。誰先發送 0，誰就奪得匯流排發送權（CAN ID 越小優先級越高）。
* **120 歐姆終端電阻 (Terminal Resistor) 的物理作用**：
  1. **阻抗匹配消除訊號反射**：CAN 雙絞線之特性阻抗約為 $120\ \Omega$。若兩端開路不接電阻，高頻數位方波抵達纜線末端時會產生嚴重的 **訊號反射 (Signal Reflection)**，反射波與入射波疊加引發劇烈震盪 (Ringing) 與過衝，造成採樣錯誤與全網癱瘓。
  2. **快速放電**：在顯性切換回隱性時，提供差動電壓快速放電回 0V 的迴路。
  3. **等效阻抗**：兩端各並聯一顆 $120\ \Omega$，整條匯流排等效直流電阻為 $60\ \Omega$。
