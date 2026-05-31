#ifndef __I2C2_H__
#define __I2C2_H__

#include "hal_data.h"

void I2C2_Init(void);

void I2C2_ByteWrite(unsigned char address, unsigned char byte);

void I2C2_BufferRead(unsigned char* ptr_read,unsigned char address,unsigned char byte);


#endif

