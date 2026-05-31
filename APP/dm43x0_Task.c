#include "dm43x0_Task.h"
#include "debug_uart7.h"
#include "dm4310_drv.h"
#include "dm4310_ctrl.h"
#include "imu_Task.h"
#include "APP.h"
#include "types.h"
#include "robot_arm.h"
#include "uart5.h"
#include "Int_electric.h"
#include "stepper_Task.h"
//机械臂运动状态
Arm_Status arm_status = ARM_ZERO;
//达妙关节电机控制参数结构体
dm4310_ctrl_4 dm4310_ctrl4;
//定义坐标变量,单位:mm
double y = 150.0;
double z = 70.0;
//预设角度编号
uint8_t preset_angle = 0;
//定义机械爪到药品距离变量
float claw_distance = 0.0f;
//定义获取到药品的坐标
float x_drug = 240;
float y_drug = 175;
/* 机械爪运动方向 */
// 0: 朝前
// 1: 朝下
uint8_t claw_direction = 0;
uint8_t TX_S = 0;
uint8_t TX_C = 0;
uint8_t Car_Adjust_tt = 0;    //前后调整校准等待时间

/**
 * @brief 达妙关节电机任务
 */
void App_Dm43x0_Task(void)
{
    // 处理串口指令
    UART5_Command();
    //状态机
    switch (arm_status)
    {
        // 零点状态
        case ARM_ZERO:
        {
            claw_direction = 0;
            dm4310_ctrl4.pos1 = 0.0f;
            dm4310_ctrl4.vel1 = 1.5f;
            dm4310_ctrl4.pos2 = 0.0f;
            dm4310_ctrl4.vel2 = 1.5f;
            dm4310_ctrl4.pos3 = 0.0f;
            dm4310_ctrl4.vel3 = 1.5f;
            dm4310_ctrl4.pos4 = 0.0f;
            dm4310_ctrl4.vel4 = 1.0f;
		    dm4310_motor_ctrl(&dm4310_ctrl4);
        }
        break;
        // 夹药品预设状态
        case ARM_PRESET:
        {
            /* 预设四个药品位置 */
            preset_coordinate();
            // 计算预设角度
            RobotArm_Algorithm(y, z);           
            /* 关节电机执行程序 */
            dm4310_ctrl4.pos1 = theta.theta1;
            dm4310_ctrl4.vel1 = 2.0f;
            dm4310_ctrl4.pos2 = theta.theta2;
            dm4310_ctrl4.vel2 = 2.0f;
            dm4310_ctrl4.pos3 = theta.theta3;
            dm4310_ctrl4.vel3 = 2.0f;
            dm4310_ctrl4.pos4 = theta.theta4;
            dm4310_ctrl4.vel4 = 0.9f;   
		    dm4310_motor_ctrl(&dm4310_ctrl4);
            if(now - Y_tick >= 2000)
            {
                if(TX_C == 1)
                {
                    TX_C = 0;
                    arm_status = ARM_ADJUST;    // 机械臂状态机切换到调整状态
                    car_status = CAR_ADJUST;    // 小车状态机切换到调整状态
                    uint8_t data = 'U';
                    R_SCI_UART_Write(&g_uart5_ctrl, &data, 1);  //告诉机械臂K230开始检测药品
                }
            }
        }
        break;
        // 夹药品调整状态
        case ARM_ADJUST:
        {
            /* 获取XYT坐标 */
            get_coordinate();
            /* 根据y_drug和claw_distance调整y和z坐标 */
            adjust_y_coordinate();
            // 计算预设角度
            RobotArm_Algorithm(y, z);           
            /* 关节电机执行程序 */
            dm4310_ctrl4.pos1 = theta.theta1;
            dm4310_ctrl4.vel1 = 10.0f;
            dm4310_ctrl4.pos2 = theta.theta2;
            dm4310_ctrl4.vel2 = 10.0f;
            dm4310_ctrl4.pos3 = theta.theta3;
            dm4310_ctrl4.vel3 = 10.0f;
            dm4310_ctrl4.pos4 = theta.theta4;
            dm4310_ctrl4.vel4 = 3.0f;   
		    dm4310_motor_ctrl(&dm4310_ctrl4);
        }
        break;
        // 放药品状态
        case ARM_RELEASE:
        {
            if(now - Y_tick >= 7000)
            {
                arm_status = ARM_ZERO;    // 状态机切换到零点状态
                uint8_t data = 'K';
                R_SCI_UART_Write(&g_uart3_ctrl, &data, 1);  //告诉小车继续行走
            }
            if(now - Y_tick >= 5500)
            {
                z = 0;
                y = 0;
            }
            else if(now - Y_tick >= 4500)
            {
                if(TX_S == 1)
                {
                    TX_S = 0;
                    uint8_t data = 'S';
                    R_SCI_UART_Write(&g_uart5_ctrl, &data, 1);  //告诉机械臂K230松开药品
                }
            }
            else if(now - Y_tick >= 2000)
            {
                theta.theta1 = 1.57f; //药框位置
                y = 170;    
                z = 220;
                claw_direction = 1;   // 夹爪朝下
            }
            // 计算放药品角度
            RobotArm_Algorithm(y, z);    
            /* 关节电机执行程序 */
            dm4310_ctrl4.pos1 = theta.theta1;
            dm4310_ctrl4.vel1 = 2.5f;
            dm4310_ctrl4.pos2 = theta.theta2;
            dm4310_ctrl4.vel2 = 2.0f;
            dm4310_ctrl4.pos3 = theta.theta3;
            dm4310_ctrl4.vel3 = 2.0f;   
            dm4310_ctrl4.pos4 = theta.theta4;
            dm4310_ctrl4.vel4 = 1.5f;   
		    dm4310_motor_ctrl(&dm4310_ctrl4);
        }
        break;
        default:break;
    }
}
/**
 * @brief 处理串口指令子程序
 */
void UART5_Command(void)
{
    if(U5_D_flag == 1)              // 夹爪已夹完药，需要抬起收缩后放药
    {
        U5_D_flag = 0;
        arm_status = ARM_RELEASE;    // 状态机切换到放药状态
        y = y - 180;                 //机械臂抬起回缩动作
        z = z + 50;
        Y_tick = now;               // 记录当前时间
        TX_S = 1;               // 标记需要发送S松开指令
    }
}
/**
 * @brief 预设四个坐标子程序
 */
void preset_coordinate(void)
{
    if(preset_angle == 1)          
    {
        preset_angle = 0;
        theta.theta1 = 0.0f;
        y = 35.0f;
        z = 70.0f;
    }
    else if(preset_angle == 2)
    {
        preset_angle = 0;
        theta.theta1 = 0.0f;
        y = 15.0f;
        z = 260.0f;
    }
}
/**
 * @brief 获取XYT坐标子程序
 */
void get_coordinate(void)
{
    if(UART5_IsDataReady())
    {
        UART5_GetReceivedFloat();
        // Car_Adjust_tt = 0;
        // printf("******** x_drug = %f\r\n ***", x_drug);
        // printf("******** y_drug = %f\r\n ***", y_drug);
        // printf("******** claw_distance = %f\r\n ***", claw_distance);
    }
    else
    {
        // claw_distance = 0.0f;
        if(++Car_Adjust_tt >= 50)
        {
            Car_Adjust_tt = 0;
            x_drug = 240;
        }
        y_drug = 170;
    }
}
/**
 * @brief 根据y_drug和claw_distance调整y和z坐标子程序
 */
void adjust_y_coordinate(void)
{

    if(y_drug >= 195)
    {
        z = z + 0.7;
    }
    else if(y_drug >= 177)
    {
        z = z + 0.35;
    }

    if(y_drug <= 145)
    {
        z = z - 0.7;
    }
    else if(y_drug <= 163)
    {
        z = z - 0.35;
    }

    if(claw_distance > 35.0)
    {
        y = y + 0.4;
    }
}



