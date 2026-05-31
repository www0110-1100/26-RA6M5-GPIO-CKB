#ifndef __UART5_H__
#define __UART5_H__

#include "hal_data.h"
#include "stdio.h"
#include <stdbool.h>

// UART5字符串接收缓冲区大小
#define UART5_RX_BUFFER_SIZE 32

// UART5指令标志位
extern uint8_t U5_D_flag;  // 夹爪继续前伸


// 函数声明
void UART5_Init(void);
void UART5_Receive_Char(void);
bool UART5_IsDataReady(void);
void UART5_GetReceivedFloat(void);
void UART5_ResetBuffer(void);

#endif



// #ifndef __UART5_H__
// #define __UART5_H__

// #include "hal_data.h"
// #include "stdio.h"
// #include <stdbool.h>
// // UART5字符串接收缓冲区大小
// #define UART5_RX_BUFFER_SIZE 32
// // 浮点数数据范围
// #define UART5_MIN_VALUE -180.00f
// #define UART5_MAX_VALUE 180.00f
// // 函数声明
// void UART5_Init(void);
// void UART5_Receive_Char(void);
// bool UART5_IsDataReady(void);
// float UART5_GetReceivedFloat(void);
// void UART5_ResetBuffer(void);





// #endif


