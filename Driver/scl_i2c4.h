#ifndef __SCL_I2C4_H__
#define __SCL_I2C4_H__

#include "hal_data.h"

// EEPROM参数定义
#define EE_PAGE_SIZE    (8)
#define EE_PAGE_NUM     (32)


 //定义I2C4设备结构体
 typedef struct I2CDev{
     char    *name;
     int     (*Init)(struct I2CDev *ptDev);
     int     (*Write)(struct I2CDev *ptDev, unsigned char ucAddr, unsigned char const *wBuf, unsigned int dwSize);
     int     (*Read)(struct I2CDev *ptDev, unsigned char ucAddr,  unsigned char *rBuf, unsigned int dwSize);
 }I2CDev, *PI2CDev;

struct I2CDev* EEPROMGetDevice(void);

#endif


