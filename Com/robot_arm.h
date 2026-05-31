#ifndef __ROBOT_ARM_H__
#define __ROBOT_ARM_H__

#include "hal_data.h"

#define PI 3.14159f

// 声明机械臂各自由度弧度结构体
typedef struct
{
    float theta1; // 第一自由度角度(弧度)
    float theta2; // 第二自由度角度(弧度)
    float theta3; // 第三自由度角度(弧度)
    float theta4; // 第四自由度角度(弧度)
} Theta_Struct;

// 声明机械臂各自由度弧度结构体
extern Theta_Struct theta; 

// 机器人手臂逆运算算法
Theta_Struct RobotArm_Algorithm(double target_y, double target_z);



#endif

