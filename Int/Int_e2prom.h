#ifndef __INT_E2PROM_H__
#define __INT_E2PROM_H__

#include "hal_data.h"

//初始化EEPROM设备
void EEPROMInit(void);
//写入数据
void EEPROMWrite(uint8_t addr, uint8_t wbuf, uint8_t size);
//读取数据
uint8_t EEPROMRead(uint8_t addr);



// void EEPROMAppTest(void);


#endif

