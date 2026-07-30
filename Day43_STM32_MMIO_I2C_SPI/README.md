# Day 43: 總複習 Part 3 - STM32 硬體周邊 (MMIO、I2C vs SPI 物理對比與 NVIC/ISR 規範)

今天是 10 天特訓總複習的第三天，我們深入複習了單晶片 Memory-Mapped I/O (MMIO) 暫存器偏移量算術、I2C 與 SPI 兩大匯流排的硬體物理對比，以及 NVIC 中斷服務函式 (ISR) 的清旗標與時間規範。

---

## 📝 實戰考題與詳細解答 (Mock Interview Questions)

### 題目 1【硬體對映與暫存器操作：MMIO 結構體映射】
* **原問題**：說明 `GPIOA->ODR` 為何等於基底 `0x48000000 + 0x14`，並寫出拉高 PA5 腳位的單行程式碼。
* **解析與解答**：
  1. **偏移量對齊 0x14**：每個 `volatile uint32_t` 佔 4 Bytes (`0x04`)。成員依序為 MODER(0x00)、OTYPER(0x04)、OSPEEDR(0x08)、PUPDR(0x0C)、IDR(0x10) 到 ODR(0x14)，基底相加剛好為 `0x48000014`。
  2. **拉高 PA5 (Bit 5)**：`GPIOA->ODR |= (1 << 5);` （控制輸出資料暫存器 ODR）。

### 題目 2【通訊協定物理對決：I2C vs SPI】
* **原問題**：SPI 4 腳位功能？I2C 為什麼需要 Pull-up 電阻？I2C 與 SPI 哪一個更快且支援全雙工？
* **解析與解答**：
  1. **SPI 4 腳位**：
     * `MISO` (Master In Slave Out)：主接收/從發送。
     * `MOSI` (Master Out Slave In)：主發送/從接收。
     * `SCLK` (Serial Clock)：時脈同步線。
     * `CS / SS` (Chip Select)：從端晶片選擇線。
  2. **I2C 上拉電阻 (Pull-up Resistor)**：I2C 採用開漏極 (Open-Drain) 腳位架構，內部 MOS 電晶體僅能將線路強制下拉至 0V。若無外部上拉電阻至 3.3V (VCC)，電位將無法彈回 3.3V，導致無法傳送高電位 `1`。
  3. **SPI 傳輸勝出**：**SPI** 具備獨立 MOSI/MISO 線支援全雙工 (Full-Duplex)，且推挽驅動速度可達 10~80 MHz（顯著快於半雙工、最高僅 400kHz~3.4MHz 的 I2C）。

### 題目 3【硬體中斷機制：NVIC 與 ISR 規範】
* **原問題**：ISR 內部讀完資料後為什麼必須手動清空中斷旗標？為什麼不能呼叫 `delay_ms(1000)`？
* **解析與解答**：
  1. **手動清旗標**：如果不手動清零中斷旗標，當 ISR 執行結束退出時，硬件 NVIC 檢測到旗標仍為 1，會以為「又有新中斷傳入」，瞬間再次跳回 ISR，導致 CPU 卡死在無窮中斷死迴圈中。
  2. **禁止長 Delay**：ISR 執行期間 CPU 會關閉其他中斷或壓制其他任務，長 Delay 會凍結作業系統排程器與時鐘滴答，導致其他任務與硬體中斷全數餓死死機。

---

## ⚙️ 核心硬體觀念複習：IDR vs ODR
* **`IDR` (Input Data Register)**：唯讀暫存器，用於腳位設為輸入時，讀取外界高低電位。
* **`ODR` (Output Data Register)**：可讀寫暫存器，用於腳位設為輸出時，主動控制腳位吐出 3.3V 或 0V。
