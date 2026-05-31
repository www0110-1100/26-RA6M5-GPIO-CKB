#ifndef __STEPPER_TASK_H__
#define __STEPPER_TASK_H__

#include "types.h"
#include "hal_data.h"

extern Car_Status car_status;

extern Car_Status car_status;
extern float line_angle;          //Ä¿±êYaw½Ç¶È


void App_Stepper_Task(void);

void APP_Car_Stop(void);

void APP_Car_PID(void);

void APP_Car_Turn(void);

// void Stepper_PID(Gyro_Acc_Struct *gyro_acc, EulerAngle_Struct *euler_angle, float yaw_Ideal, float dt);

void Stepper_PID(float line_ang, float line_Id, float dt);

// void Stepper_PID(EulerAngle_Struct *euler_angle, float yaw_Ideal, float dt);
void UART3_Command(void);

void APP_Car_Adjust(void);

extern int16_t speed1;
extern int16_t speed2;
extern int16_t speed3;
extern int16_t speed4;


#endif



