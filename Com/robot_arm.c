#include "robot_arm.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "uart5.h"
#include "debug_uart7.h"
#include "dm43x0_Task.h"

//机器人手臂连接杆长度单位：mm
double a = 188.8;
double b = 180.0;
//L12臂初始夹角(弧度)
float L12_theta1 = 0.3067f; 
//坐标夹角弧度
float theta5;
//坐标距零点距离
float c;
// 初始化机械臂各自由度弧度结构体
Theta_Struct theta = {0};

/**
 * @brief 机器人手臂逆运算算法
 * @param y 目标点y坐标
 * @param z 目标点z坐标
 * @return Theta_Struct 机械臂各自由度弧度结构体
 */
Theta_Struct RobotArm_Algorithm(double target_y, double target_z)
{
    // target_y, target_z 限幅处理
    if(target_z < 0)
    {
        target_z = 0.0f;
    }

    // 计算坐标夹角弧度
    theta5 = (float)atan2(target_z, target_y); 

    // 计算theta2
    c = (float)sqrt(target_y * target_y + target_z * target_z);      
    if(c > 360 || c < 8.8)
    {
        return theta;
    }
    theta.theta2 = PI - (float)(acos((c * c + b * b - a * a) / (2 * b * c)) + theta5);

    // 计算theta3
    theta.theta3 = (float)acos((a * a + b * b - c * c) / (2 * a * b)) - L12_theta1;

    //计算theta4
    if(claw_direction == 0)
    {
        theta.theta4 = theta.theta3 - theta.theta2 - 0.1f;
    }
    else if(claw_direction == 1)
    {
        theta.theta4 = theta.theta3 - theta.theta2 + 1.56f;
    }
    // printf("theta2:%.4f, theta3:%.4f, theta4:%.4f\n", theta.theta2, theta.theta3, theta.theta4);

    return theta;
}

