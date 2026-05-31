#ifndef __CANFD0_H__
#define __CANFD0_H__

#include "hal_data.h"


#define CAN_MAILBOX_NUMBER_0            (CANFD_TX_MB_0)    //CANFD1????1
#define WAIT_TIME                       (0xFFFF)           //????


void CANFD0_Init(void);
bool canfd0_send(uint32_t id, uint8_t *p_data, uint8_t length);
void canfd0_read(void);



#endif

