#ifndef IMU_H
#define IMU_H

#include "types.h"
#include "math.h"
#include "hal_data.h"

/* 表示四元数的结构体 */
typedef struct
{
    float q0;
    float q1;
    float q2;
    float q3;
} Quaternion_Struct;

extern float RtA;
extern float Gyro_G;
extern float Gyro_Gr;

void Com_IMU_GetEulerAngle(Gyro_Acc_Struct *gyroAccel,
                              EulerAngle_Struct *eulerAngle,
                              float dt);
float Com_IMU_GetNormAccZ(void);

#endif





