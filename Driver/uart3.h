#ifndef __UART3_H__
#define __UART3_H__

#include "hal_data.h"
#include "stdio.h"
#include <stdbool.h>
// UART5字符串接收缓冲区大小
#define UART3_RX_BUFFER_SIZE 32

extern uint8_t S_flag;
extern uint8_t T_flag;
extern uint8_t P_flag;
extern uint8_t R_flag;
extern uint8_t L_flag;
extern uint8_t A_flag;
extern uint8_t B_flag;
extern uint8_t display_flag;
extern char display_buffer[32];



// 函数声明
void UART3_Init(void);
void UART3_Receive_Char(void);
bool UART3_IsDataReady(void);
float UART3_GetReceivedFloat(void);



#endif











