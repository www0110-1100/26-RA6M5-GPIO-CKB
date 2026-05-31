#ifndef __KEY_H__
#define __KEY_H__


#include "hal_data.h"

#define KEY  BSP_IO_PORT_00_PIN_00
#define KEY1  BSP_IO_PORT_03_PIN_12
#define KEY2  BSP_IO_PORT_02_PIN_00
#define KEY3  BSP_IO_PORT_03_PIN_11
#define KEY4  BSP_IO_PORT_09_PIN_07
#define KEY5  BSP_IO_PORT_09_PIN_00
#define KEY_ON 1
#define KEY_OFF 0


uint32_t KeyScan(bsp_io_port_pin_t KEY);





#endif



