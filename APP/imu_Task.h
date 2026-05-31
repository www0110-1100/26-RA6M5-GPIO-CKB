#ifndef __IMU_TASK_H__
#define __IMU_TASK_H__

#include "hal_data.h"
#include "Int_mpu6500.h"
#include "IMU.h"
#include "kalman.h"
#include "types.h"

// extern float yaw_offset;

// 陀螺仪加速度结构体
extern Gyro_Acc_Struct Gyro_Acc;
// 欧拉角结构体
extern EulerAngle_Struct EulerAngle;
extern float yaw_offset;
//定义四元数计算间隔
#define FLIGHT_TASK_PERIOD 10

void App_Imu_Task(void);
void App_Flight_Get_Gyro_Acc(Gyro_Acc_Struct *gyro_acc);
void App_Flight_Get_EulerAngle(Gyro_Acc_Struct *gyro_acc, EulerAngle_Struct *euler_angle, float dt);



#endif

