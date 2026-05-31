#ifndef __GPT_PWM_H__
#define __GPT_PWM_H__

#include "hal_data.h"

//void GptPWMDrvInit(void);

//void GptPWMSetDuty(uint8_t duty);

void GPT_PWM_Init(void);

void GPT_PWM6_SetDuty(uint8_t duty);

void GPT_PWM6_SetFreq(uint32_t freq);

void GPT_PWM0_SetDuty(uint8_t duty);

void GPT_PWM0_SetFreq(uint32_t freq);


#endif