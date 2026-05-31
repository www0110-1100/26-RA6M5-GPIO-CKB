#ifndef __LED_H__
#define __LED_H__


#include "hal_data.h"

#define LED_ON   R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_00, BSP_IO_LEVEL_LOW)
#define LED_OFF  R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_04_PIN_00, BSP_IO_LEVEL_HIGH)





#endif


