#ifndef __CANFD1_H__
#define __CANFD1_H__

#include "hal_data.h"


#define CAN_MAILBOX_NUMBER_0            (CANFD_TX_MB_0)    //CANFD1” œ‰
#define CANFD1_WAIT_TIME                1000           //—”≥Ÿ ±º‰

extern uint8_t can_rx_flag;

void CANFD1_Init(void);
bool canfd1_send(uint16_t id, uint8_t *p_data, uint8_t length);
void canfd1_read(void);



#endif


