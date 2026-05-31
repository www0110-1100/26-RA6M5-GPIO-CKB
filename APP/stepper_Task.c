#include "stepper_Task.h"
#include "PID.h"
#include "imu_Task.h"
#include "dm43x0_Task.h"
#include "types.h"
#include "canfd0.h"
#include "Int_electric.h"
#include "Int_motor.h"
#include "APP.h"
#include "uart3.h"
#include "debug_uart7.h"
#include "APP.h"
#include "Int_motor.h"
// 定义小车运动状态
Car_Status car_status = CAR_STOP;
// 偏航角PID参数
PID_Struct pid_yaw = {.kp = 0.6f, .ki = 0.0f, .kd = 0.75f};      //外环参数 

int16_t speed_base = 75;     //电机基础速度
int16_t speed1 = 0;               //电机1速度
int16_t speed2 = 0;               //电机2速度
int16_t speed3 = 0;               //电机3速度
int16_t speed4 = 0;               //电机4速度
//上一次电机速度
int16_t last_speed1 = 0;
int16_t last_speed2 = 0;
int16_t last_speed3 = 0;
int16_t last_speed4 = 0;

float Yaw_Ideal = 0;          //目标Yaw角度
float yaw_in = 0.0f;          //转向角度
float line_angle = 160.0f;      //寻线角度
float line_ideal = 160.0f;      //寻线角度目标值
//定义事件变量
uint8_t stepper_tt = 0;      //转弯后校准等待
uint8_t stepper_ttt = 0;    //前1秒加速时间标定
//通知机械臂前后校准完毕往前伸到目标位置标识位
uint8_t stepper_adjust_flag = 0;


// uint16_t stepper_tttt = 0;    //前后调整校准等待

/**
 * @description: 步进电机任务
 * @return {*}
 */
void App_Stepper_Task(void) 
{   
    // 处理串口指令
    UART3_Command();
    //小车任务状态机
    switch (car_status)
    {
        case CAR_STOP:          //停止状态
        {
            APP_Car_Stop();
        }
        break;
        case CAR_PID:          //PID控制状态    
        {
            APP_Car_PID();
        }
        break;
        case CAR_TURN:          //转向状态：左转90度
        {
            APP_Car_Turn();
        }
        break;
        case CAR_ADJUST:          //前后调整状态
        {
            APP_Car_Adjust();
        }
        break;
        default:break;
    }
}
/**
 * @description: 处理串口指令
 */
void UART3_Command(void)
{
    if(A_flag == 1)                 //停止并取下A药品标志位
    {
        A_flag = 0;
        car_status = CAR_STOP;
        preset_angle = 1;
        arm_status = ARM_PRESET;     // 机械臂状态机切换到预设状态
        Y_tick = now;               // 记录当前时间
        TX_C = 1;
    }
    else if(B_flag == 1)            //停止并取下B药品标志位
    {
        B_flag = 0;
        car_status = CAR_STOP;
        preset_angle = 2;
        arm_status = ARM_PRESET;     // 机械臂状态机切换到预设状态
        Y_tick = now;               // 记录当前时间
        TX_C = 1;
    }
    if(S_flag == 1)                 //停止标志位
    {
        S_flag = 0;
        car_status = CAR_STOP;
    }
    else if(T_flag == 1)            //左转180向标志位
    {
        T_flag = 0;
        yaw_in = 178.0f;
        // Yaw_Ideal = EulerAngle.yaw + yaw_in;
        Yaw_Ideal = Yaw_Ideal + yaw_in;
        car_status = CAR_TURN;
    }
    else if(R_flag == 1)            //右转90向标志位
    {
        R_flag = 0;
        yaw_in = -88.0f;
        // Yaw_Ideal = EulerAngle.yaw + yaw_in;
        Yaw_Ideal = Yaw_Ideal + yaw_in;
        car_status = CAR_TURN;
    }
    else if(L_flag == 1)            //左转90向标志位
    {
        L_flag = 0;
        yaw_in = 88.0f;
        // Yaw_Ideal = EulerAngle.yaw + yaw_in;
        Yaw_Ideal = Yaw_Ideal + yaw_in;
        car_status = CAR_TURN;
    }
    else if(P_flag == 1)            //寻线标志位
    {
        P_flag = 0;
        stepper_ttt = 0;
        car_status = CAR_PID;
    }
}
/**
 * @description: 车前后调整函数
 */
void APP_Car_Adjust(void)
{
    //1.将四个电机速度设置为0
    speed1 = 0;
    speed2 = 0;
    speed3 = 0;
    speed4 = 0;
    if(x_drug >= 253)
    {
        speed1 = 2;
        speed2 = 2;        
        speed3 = 2; 
        speed4 = 2;
        stepper_motor_ctrl(speed1, speed2, speed3, speed4);
    }
    else if(x_drug <= 227)
    {
        speed1 = -2;
        speed2 = -2;        
        speed3 = -2; 
        speed4 = -2;
        stepper_motor_ctrl(speed1, speed2, speed3, speed4);
    }
    else if((x_drug > 227) && (x_drug < 253))
    {
        speed1 = 0;
        speed2 = 0;
        speed3 = 0;
        speed4 = 0;
        stepper_motor_ctrl(speed1, speed2, speed3, speed4);
        stepper_motor_stop();
    }
}
/**
 * @description: 车停止函数
 * @return {*}
 */
void APP_Car_Stop(void)
{
    //3.急停电机
    stepper_motor_stop();
}
/**
 * @description: 车PID控制函数
 * @return {*}
 */
void APP_Car_PID(void)
{
    //1.将四个电机基础速度设置为100
    speed1 = speed_base;
    speed2 = speed_base;
    speed3 = speed_base;
    speed4 = speed_base;
    if(stepper_ttt == 0)
    {
        last_speed1 = 50;
        last_speed2 = 50;
        last_speed3 = 50;
        last_speed4 = 50;
    }
    // 检查是否有新数据
    if(UART3_IsDataReady())
    {
        line_angle = UART3_GetReceivedFloat();
    }
    // else
    // {
    //     line_angle = 160.0f;
    // }
   //2.进行PID计算
   Stepper_PID(line_angle, line_ideal, stepper_dt);

   //3.设置四个步进电机速度
   speed1 = speed1 + (int16_t)pid_yaw.output;
   speed2 = speed2 - (int16_t)pid_yaw.output;
   speed3 = speed3 + (int16_t)pid_yaw.output;
   speed4 = speed4 - (int16_t)pid_yaw.output;
   //前1秒对速度进行低通滤波
   if(++stepper_ttt <= 100)
   {
        speed1 = (int16_t)(speed1 * 60 + last_speed1 * 40) / 100;
        speed2 = (int16_t)(speed2 * 60 + last_speed2 * 40) / 100;
        speed3 = (int16_t)(speed3 * 60 + last_speed3 * 40) / 100;
        speed4 = (int16_t)(speed4 * 60 + last_speed4 * 40) / 100;
        last_speed1 = speed1;
        last_speed2 = speed2;
        last_speed3 = speed3;
        last_speed4 = speed4;
   }
   //4.将速度赋值给每个电机
   stepper_motor_ctrl(speed1, speed2, speed3, speed4);
}
/**
 * @description: 步进电机PID控制
 * @param {float} line_ang 寻线角度
 * @param {float} line_Id 寻线角度目标值
 * @param {float} dt 时间间隔
 * @return {*}
 */
void Stepper_PID(float line_ang, float line_Id, float dt)
{
    // 偏航角
    pid_yaw.desire = line_Id;                      //外环目标值
    pid_yaw.measure = line_ang;					   //外环测量值
    Com_PID_Calculate(&pid_yaw, dt);
}

/**
 * @description: 车转函数
 * @param {int}  大于0左转指定角度，小于0右转指定角度
 * @return {*}
 */
void APP_Car_Turn(void)
{
    //2.计算四电机速度
    if(yaw_in > 0)
    {
        //2.1.1判断是否到达目标角度
        if((EulerAngle.yaw <= (Yaw_Ideal - 2.0f)))  //未到达目标角度，继续左转
        {
            speed1 = -80;
            speed2 =  80;
            speed3 = -80;
            speed4 =  80;
        }
        else if((EulerAngle.yaw >= (Yaw_Ideal + 2.0f)))  //转弯过度，右转补偿
        {
            speed1 =  20;
            speed2 = -20;
            speed3 =  20;
            speed4 = -20;
        }
        else if((EulerAngle.yaw >= (Yaw_Ideal - 2.0f)) && (EulerAngle.yaw <= (Yaw_Ideal + 2.0f)))  //到达目标角度
        {
            speed1 = 0;
            speed2 = 0;
            speed3 = 0;
            speed4 = 0;
            if(++stepper_tt >= 100)
            {
                stepper_tt = 0;
                car_status = CAR_STOP;   //到达目标角度，切换到PID控制状态
            }
        }
    }
    else if(yaw_in < 0)
    {
        //2.2.1判断是否到达目标角度
        if((EulerAngle.yaw >= (Yaw_Ideal + 2.0f)))  //未到达目标角度，继续右转
        {
            speed1 =  80;
            speed2 = -80;
            speed3 =  80;
            speed4 = -80;
        }
        else if((EulerAngle.yaw <= (Yaw_Ideal - 2.0f)))  //转弯过度，左转补偿
        {
            speed1 = -20;
            speed2 =  20;
            speed3 = -20;
            speed4 =  20;
        }
        else if((EulerAngle.yaw <= (Yaw_Ideal + 2.0f)) && (EulerAngle.yaw >= (Yaw_Ideal - 2.0f)))  //到达目标角度
        {
            speed1 = 0;
            speed2 = 0;
            speed3 = 0;
            speed4 = 0;
            if(++stepper_tt >= 100)
            {
                stepper_tt = 0;
                car_status = CAR_STOP;   //到达目标角度，切换到PID控制状态
            }
        }               
    }
    //3.将速度赋值给每个电机
    stepper_motor_ctrl(speed1, speed2, speed3, speed4);
}




// /**
//  * @description: 步进电机PID控制
//  * @param {Gyro_Acc_Struct} *gyro_acc 陀螺仪加速度结构体
//  * @param {EulerAngle_Struct} *euler_angle 欧拉角结构体
//  * @param {uint16_t} Yaw_Ideal 目标Yaw角度
//  * @param {float} dt 时间间隔
//  * @return {*}
//  */
// void Stepper_PID(EulerAngle_Struct *euler_angle, float yaw_Ideal, float dt)
// {
//     // 偏航角
//     pid_yaw.desire = yaw_Ideal;                            //外环目标值
//     pid_yaw.measure = euler_angle->yaw;					   //外环测量值
//     Com_PID_Calculate(&pid_yaw, dt);
// }

