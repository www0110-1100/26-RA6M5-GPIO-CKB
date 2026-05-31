#ifndef IMU_YAW_H
#define IMU_YAW_H

#include "types.h"
#include "math.h"
#include "hal_data.h"

// Yaw角解算参数结构体
typedef struct 
{
    float yaw;              // 当前yaw角度（弧度）
    float yaw_deg;          // 当前yaw角度（度）
    float gyro_z_bias;      // Z轴陀螺仪零偏
    float accel_weight;     // 加速度计融合权重
    uint32_t last_time;     // 上次更新时间
    uint8_t is_calibrated;  // 是否已完成校准
} YawIMU_Struct;

// 外部变量声明
extern YawIMU_Struct yaw_imu;

// 函数声明
void YawIMU_Init(void);
void YawIMU_Calibrate(void);
float YawIMU_Update(Gyro_Acc_Struct *gyro_accel);
float YawIMU_GetYawDeg(void);
float YawIMU_GetYawRad(void);
void YawIMU_ResetYaw(void);

#endif