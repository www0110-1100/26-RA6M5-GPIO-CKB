#ifndef __SYSTICK_H__
#define __SYSTICK_H__

#include "hal_data.h"
 
typedef enum
{
    Delay_s  = 200000000,
    Delay_ms = 200000,
    Delay_us = 200,
} Delay_SysTick;

void SysTick_Init(uint32_t IT_Freq_Hz);

void SysTick_Delay(uint32_t delay, Delay_SysTick unit);




#endif


