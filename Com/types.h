#ifndef __TYPES_H__
#define __TYPES_H__

#include "hal_data.h"

// 机械臂运动状态枚举
typedef enum
{
    ARM_ZERO ,       //零点状态
    ARM_PRESET ,     //夹药品预设状态
    ARM_ADJUST ,     //夹药品调整状态
    ARM_RELEASE ,    //放药品状态
}Arm_Status;

//小车运动状态枚举
typedef enum
{
    CAR_STOP ,       //停止状态
    CAR_PID  ,       //PID控制状态
    CAR_TURN ,       //转向状态
    CAR_ADJUST ,     //前后调整状态
}Car_Status;

//达妙四个关节电机控制参数
typedef struct
{
    float pos1;    //位置设置值1
    float vel1;    //速度设置值1
    float pos2;    //位置设置值2
    float vel2;    //速度设置值2
    float pos3;    //位置设置值3
    float vel3;    //速度设置值3
    float pos4;    //位置设置值4
    float vel4;    //速度设置值4
}dm4310_ctrl_4;

//达妙四关节实时位置结构体
typedef struct
{
    float pos1;    //位置1
    float pos2;    //位置2
    float pos3;    //位置3
    float pos4;    //位置4
}dm4310_pos_4;

// 陀螺仪数据结构体
typedef struct
{
    int16_t x;    //结构体体成员  x轴
    int16_t y;    //结构体体成员  y轴
    int16_t z;    //结构体体成员  z轴
}Gyro_Struct;
// 加速度数据结构体
typedef struct
{
    int16_t x;    //结构体体成员  x轴
    int16_t y;    //结构体体成员  y轴
    int16_t z;    //结构体体成员  z轴
}Acc_Struct;
// 姿态角度结构体
typedef struct
{
    Acc_Struct acc;      //结构体体成员  加速度
    Gyro_Struct gyro;    //结构体体成员  陀螺仪
}Gyro_Acc_Struct;
// 欧拉角结构体
typedef struct
{
    float pitch;    //结构体体成员  俯仰角度
    float roll;     //结构体体成员  横滚角度
    float yaw;      //结构体体成员  偏航角度
}EulerAngle_Struct;


#endif