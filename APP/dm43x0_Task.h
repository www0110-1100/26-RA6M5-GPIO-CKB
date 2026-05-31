#ifndef __DM43X0_TASK_H__
#define __DM43X0_TASK_H__

#include "types.h"
#include "hal_data.h"

// 达妙关节电机控制参数结构体
extern dm4310_ctrl_4 dm4310_ctrl4;
extern Arm_Status arm_status;
extern double x;
extern double y;
extern double z;
extern float motor4_pos;
extern float x_drug;
extern float y_drug;
extern uint8_t claw_direction;
extern float claw_distance;
extern uint8_t preset_angle;
extern uint8_t TX_S;
extern uint8_t TX_C;

extern Arm_Status arm_status;


void App_Dm43x0_Task(void);
void preset_coordinate(void);
void adjust_y_coordinate(void);
void UART5_Command(void);
void get_coordinate(void);

#endif
