#ifndef __INT_ELECTRIC_H__
#define __INT_ELECTRIC_H__

#include "hal_data.h"
#include "types.h"

#define ID1 1
#define ID2 2
#define ID3 3
#define ID4 4

//初始化两电机
void dm4310_stepper_init(void);

// 达妙关节电机控制函数
void dm4310_motor_ctrl(dm4310_ctrl_4 *dm4310_ctrl);

// 张大头步进电机控制函数
void stepper_motor_ctrl(int16_t vel1, int16_t vel2, int16_t vel3, int16_t vel4);

// 张大头步进电机急停
void stepper_motor_stop(void);



#endif


