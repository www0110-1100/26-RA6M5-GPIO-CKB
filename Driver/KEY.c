#include "KEY.h"
#include "types.h"


/**
 * @brief ??????
 * @param KEY?????
 * @return ?????KEY_ON???????KEY_OFF??????
 */
uint32_t KeyScan(bsp_io_port_pin_t KEY)
{
    bsp_io_level_t pin_value;

	R_IOPORT_PinRead(&g_ioport_ctrl, KEY, &pin_value);
	if(BSP_IO_LEVEL_HIGH == pin_value)
    {
        return KEY_OFF;
    }
    else
    {
        do
        {
            R_IOPORT_PinRead(&g_ioport_ctrl, KEY, &pin_value);
        } 
        while (BSP_IO_LEVEL_LOW == pin_value);  
    }
    return KEY_ON;
}



