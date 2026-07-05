typedef struct 
{
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    uint32_t reserved[2];
    volatile uint32_t RTSR;
} EXTI_TypeDef;

//#define EXTI_BASE (*(volatile uint32_t)0x40013C00)
//#define EXTI (EXTI_TypeDef *(EXTI_BASE))

// 1. 基底位址就只是單純的數字 (加上 U 代表 Unsigned，是個好習慣)
#define EXTI_BASE 0x40013C00U 

// 2. 直接把這個數字，強制轉型成「指向結構體的指標」！
#define EXTI ((EXTI_TypeDef *)EXTI_BASE)
