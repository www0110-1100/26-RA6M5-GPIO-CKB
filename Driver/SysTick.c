#include "SysTick.h"
#include "LED.h"
#include "APP.h"
#include "types.h"

static uint32_t IT_Period;   // 中断周期
static __IO uint32_t IT_Nums; //延时需要的中断次数

// 初始化SysTick定时器
// IT_Freq_Hz：中断频率
void SysTick_Init(uint32_t IT_Freq)
{
    //1. 计算中断周期： 系统时钟周期数 = 系统时钟频率 / 中断频率
    IT_Period = SystemCoreClock / IT_Freq;
    //2. 配置SysTick定时器
    SysTick_Config(IT_Period);
}


extern void SysTick_Handler(void);

void SysTick_Handler(void)
{
   
    SyeTick_tt();

    if(IT_Nums != 0)
    {
        IT_Nums--;
    }
    
}

// 延时函数
// delay：延时时间，单位：系统时钟周期数
void SysTick_Delay(uint32_t delay, Delay_SysTick unit)
{
    //1. 计算延时时间： 延时时间 = 延时时间 * 单位
    uint32_t Sum_Time = delay * unit;
    //2. 计算延时需要的中断次数： 中断次数 = 延时时间 / 中断周期
    IT_Nums = Sum_Time / IT_Period;
    //3. 延时： 等待中断次数为0
    SysTick->VAL = 0UL;

    while(IT_Nums != 0);
}
