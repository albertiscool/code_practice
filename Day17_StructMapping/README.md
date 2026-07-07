# Day 17: 結構體記憶體映射 (Struct Memory Mapping)

今天我們進入了韌體底層開發的「最高藝術」，學習了原廠 SDK (如 STM32 HAL Library 或 Linux Kernel) 是如何優雅、安全地控制硬體的。這完美結合了我們先前學到的「結構體對齊」與「絕對記憶體指標」。

## 1. ❌ 原始手工作業的痛點 (Day 16)
在開發微控制器時，如果晶片有上百個硬體暫存器，我們若一個一個定義絕對位址：
```c
#define GPIOD_MODER (*(volatile uint32_t *)0x40020C00)
#define GPIOD_ODR   (*(volatile uint32_t *)0x40020C14)
```
這不僅容易打錯字引發系統崩潰，而且程式碼會變得極度冗長且難以維護。

## 2. 👑 業界標準解法：結構體映射 (Struct Mapping)
原廠工程師利用 C 語言「結構體在記憶體中會連續排列」的特性，製作了一個完美的記憶體模具：

```c
typedef struct {
    volatile uint32_t MODER;   // Offset 0x00
    volatile uint32_t OTYPER;  // Offset 0x04
    volatile uint32_t OSPEEDR; // Offset 0x08
    volatile uint32_t PUPDR;   // Offset 0x0C
    volatile uint32_t IDR;     // Offset 0x10
    volatile uint32_t ODR;     // Offset 0x14
} GPIO_TypeDef;
```
因為每個變數都是 `uint32_t` (精準的 4 Bytes)，它們的相對位移量 (Offset) 剛好完美對應了硬體手冊上的設計。

接著，我們只需要定義一個「基底位址 (Base Address)」，並將其轉型為指向該結構體的指標：
```c
#define GPIOD_BASE    (0x40020C00)
#define GPIOD         ((GPIO_TypeDef *)GPIOD_BASE)
```
此後，當我們寫出 `GPIOD->ODR` 時，C 編譯器會自動幫我們計算出 `0x40020C00 + 0x14 = 0x40020C14` 的絕對位址！不僅語法優雅如高階語言的物件導向，還徹底消滅了手動計算十六進位的風險。

## 3. 🕳️ 處理硬體記憶體的「空洞 (Reserved)」
如果硬體設計廠在兩個暫存器中間留了一段「保留區 (Reserved)」，例如中間空了 8 個 Bytes，我們絕對不能跳過不管，否則後方的所有暫存器位址都會大錯位。
**破解法：塞入無用的墊片 (Dummy Variable)**
```c
typedef struct {
    volatile uint32_t PUPDR;   // Offset 0x0C
    
    // 塞入 8 Bytes 的陣列墊片，把空間撐開，故意不加 volatile
    uint32_t RESERVED[2];      // Offset 0x10 ~ 0x17
    
    volatile uint32_t IDR;     // Offset 0x18 (成功位移)
} GPIO_TypeDef;
```
這招能精準控制結構體的排版，完全貼合實體晶片。

## 4. 🛡️ 內建 `volatile` 的終極防呆
我們將 `volatile` 關鍵字直接寫死在 `struct` 的內部成員定義上 (`volatile uint32_t IDR;`)。
這確保了未來不管是哪位工程師、呼叫了哪一個變數 (`GPIOD->IDR`)，編譯器都會乖乖地去實體記憶體讀取真實的硬體電壓，絕對不會因為開啟了 `-O3` 優化而將數值快取到 CPU Register 導致程式死當。
