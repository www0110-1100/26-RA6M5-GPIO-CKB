#ifndef __APP_H__
#define __APP_H__


#include "hal_data.h"

extern uint32_t now;
extern uint32_t Y_tick;  //预设行进时间
extern uint32_t last_imu_tick;
extern uint32_t last_stepper_tick;
extern uint32_t last_dm_tick;
extern uint32_t last_display_tick;
extern float stepper_dt;

void app_main(void);

void SyeTick_tt(void);






#endif



