#include "imu_yaw.h"
#include "types.h"
#include "math.h"
#include "hal_data.h"
#include "APP.h"

// 全局变量定义
YawIMU_Struct yaw_imu = {0};

// 常量定义
#define GYRO_Z_SCALE        (2000.0f / 32768.0f)  // 陀螺仪量程2000dps
#define ACCEL_SCALE         (2.0f / 32768.0f)      // 加速度计量程2g
#define DT_DEFAULT          0.01f                  // 默认时间间隔10ms
#define CALIBRATION_SAMPLES  500                   // 校准采样点数
#define RAD_TO_DEG          57.295779513f          // 弧度转角度
#define DEG_TO_RAD          0.01745329252f         // 角度转弧度
#define PI                  3.14159265359f         // 圆周率
/**
 * @brief 初始化Yaw角解算模块
 */
void YawIMU_Init(void)
{
    yaw_imu.yaw = 0.0f;
    yaw_imu.yaw_deg = 0.0f;
    yaw_imu.gyro_z_bias = 0.0f;
    yaw_imu.accel_weight = 0.02f;  // 加速度计权重2%
    yaw_imu.last_time = 0;
    yaw_imu.is_calibrated = 0;
}

/**
 * @brief 陀螺仪零偏校准
 */
void YawIMU_Calibrate(void)
{
    Gyro_Acc_Struct gyro_accel;
    float gyro_sum = 0.0f;
    uint32_t count = 0;
    
    // 采集校准数据
    for (count = 0; count < CALIBRATION_SAMPLES; count++)
    {
        MPU6500_Read(&gyro_accel);
        gyro_sum += (float)gyro_accel.gyro.z * GYRO_Z_SCALE * DEG_TO_RAD;
        R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
    }
    
    // 计算零偏
    yaw_imu.gyro_z_bias = gyro_sum / CALIBRATION_SAMPLES;
    yaw_imu.is_calibrated = 1;
    
    // 重置yaw角
    YawIMU_ResetYaw();
}

/**
 * @brief 更新Yaw角解算
 * @param gyro_accel 陀螺仪和加速度计数据
 * @return 当前yaw角度（弧度）
 */
float YawIMU_Update(Gyro_Acc_Struct *gyro_accel)
{
    static uint32_t last_update_time = 0;
    uint32_t current_time = now;  // 使用全局时间变量
    float dt;
    
    // 计算时间间隔
    if (last_update_time == 0)
    {
        dt = DT_DEFAULT;
    }
    else
    {
        dt = (float)(current_time - last_update_time) * 0.001f;  // 转换为秒
        if (dt > 0.1f) dt = DT_DEFAULT;  // 限制最大时间间隔
    }
    last_update_time = current_time;
    
    if (!yaw_imu.is_calibrated)
    {
        return yaw_imu.yaw;
    }
    
    // 陀螺仪积分（去除零偏）
    float gyro_z = (float)gyro_accel->gyro.z * GYRO_Z_SCALE * DEG_TO_RAD;
    float gyro_yaw = (gyro_z - yaw_imu.gyro_z_bias) * dt;
    
    // 加速度计辅助修正（仅在小角度时有效）
    float accel_yaw_correction = 0.0f;
    
    // 计算加速度计提供的yaw角（通过磁场或水平分量）
    float accel_x = (float)gyro_accel->acc.x * ACCEL_SCALE;
    float accel_y = (float)gyro_accel->acc.y * ACCEL_SCALE;
    float accel_z = (float)gyro_accel->acc.z * ACCEL_SCALE;
    
    // 计算水平加速度模量
    float accel_horizontal = sqrtf(accel_x * accel_x + accel_y * accel_y);
    
    // 如果设备基本水平（Z轴接近1g），使用加速度计修正
    if (fabsf(accel_z - 1.0f) < 0.2f && accel_horizontal > 0.1f)
    {
        // 计算加速度计提供的yaw角（基于水平加速度方向）
        float accel_yaw = atan2f(accel_y, accel_x);
        
        // 简单的互补滤波
        accel_yaw_correction = (accel_yaw - yaw_imu.yaw) * yaw_imu.accel_weight;
    }
    
    // 更新yaw角
    yaw_imu.yaw += gyro_yaw + accel_yaw_correction;
    
    // 角度归一化到[-π, π]
     if (yaw_imu.yaw > PI)
        yaw_imu.yaw -= 2 * PI;
    else if (yaw_imu.yaw < -PI)
        yaw_imu.yaw += 2 * PI;
    
    // 转换为角度
    yaw_imu.yaw_deg = yaw_imu.yaw * RAD_TO_DEG;
    
    return yaw_imu.yaw;
}

/**
 * @brief 获取当前yaw角度（度）
 * @return yaw角度（度）
 */
float YawIMU_GetYawDeg(void)
{
    return yaw_imu.yaw_deg;
}

/**
 * @brief 获取当前yaw角度（弧度）
 * @return yaw角度（弧度）
 */
float YawIMU_GetYawRad(void)
{
    return yaw_imu.yaw;
}

/**
 * @brief 重置yaw角为0
 */
void YawIMU_ResetYaw(void)
{
    yaw_imu.yaw = 0.0f;
    yaw_imu.yaw_deg = 0.0f;
}