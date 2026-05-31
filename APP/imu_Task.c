#include "imu_Task.h"
#include "Int_mpu6500.h"
#include "IMU.h"
#include "kalman.h"
#include "types.h"
#include "stdio.h"
#include "debug_uart7.h"
#include "APP.h"

//定义上一次的陀螺仪数据
Gyro_Struct last_gyro;  
//定义陀螺仪加速度结构体
Gyro_Acc_Struct Gyro_Acc;
//定义欧拉角结构体
EulerAngle_Struct EulerAngle;
// float yaw_offset = 0.0f;
/**
 * @brief: IMU任务
 */
void App_Imu_Task(void)
{
    // 1.获取陀螺仪加速度数据
    App_Flight_Get_Gyro_Acc(&Gyro_Acc);
    // 2.计算欧拉角
    float dt = ((float)now - (float)last_imu_tick) / 1000.0f;
    App_Flight_Get_EulerAngle(&Gyro_Acc, &EulerAngle, dt);
    // 3.对偏航角进行偏移
    // EulerAngle.yaw -= yaw_offset;
    last_imu_tick = now;          // 记录本次开始时间
}

/**
 * @description: 获取陀螺仪加速度数据
 * @param {Gyro_Acc_Struct} *gyro_acc 指向陀螺仪加速度结构体的指针
 * @return {*}
 */
void App_Flight_Get_Gyro_Acc(Gyro_Acc_Struct *gyro_acc)
{
    // 1.从MPU6500读取陀螺仪加速度数据
	MPU6500_Read(&Gyro_Acc);
    // 2.角速度 使用一阶低通滤波
    gyro_acc->gyro.x = Com_Filter_LowPass(gyro_acc->gyro.x, last_gyro.x);
    gyro_acc->gyro.y = Com_Filter_LowPass(gyro_acc->gyro.y, last_gyro.y);
    gyro_acc->gyro.z = Com_Filter_LowPass(gyro_acc->gyro.z, last_gyro.z);
    last_gyro = gyro_acc->gyro;
    // 3.对加速度 使用卡尔曼滤波
    gyro_acc->acc.x = (int16_t)Com_Filter_KalmanFilter(&kfs[0], gyro_acc->acc.x);
    gyro_acc->acc.y = (int16_t)Com_Filter_KalmanFilter(&kfs[1], gyro_acc->acc.y);
    gyro_acc->acc.z = (int16_t)Com_Filter_KalmanFilter(&kfs[2], gyro_acc->acc.z);
}

/**
 * @description: 获取欧拉角
 * @param {Gyro_Acc_Struct} *gyro_acc 指向陀螺仪加速度结构体的指针
 * @param {EulerAngle_Struct} *euler_angle 指向欧拉角结构体的指针
 * @param {float} dt 时间间隔，单位：秒
 * @return {*}
 */
void App_Flight_Get_EulerAngle(Gyro_Acc_Struct *gyro_acc, EulerAngle_Struct *euler_angle, float dt)
{
    // 使用四元数获取欧拉角
    Com_IMU_GetEulerAngle(gyro_acc, euler_angle, dt); 
}










