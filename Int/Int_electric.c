#include "Int_electric.h"
#include "dm4310_drv.h"
#include "dm4310_ctrl.h"
#include "canfd0.h"
#include "canfd1.h"
#include <stdio.h>
#include "Int_motor.h"
#include "debug_uart7.h"

/**
 * @brief 初始化CANFD电机控制系统
 * @return 初始化成功返回true，失败返回false
 */
void dm4310_stepper_init(void)
{
    CANFD1_Init();       // 初始化CANFD1
	 
	CANFD0_Init();       // 初始化CANFD0

    dm4310_motor_init(); // 初始化DM4310电机

    ctrl_enable();       // 启用DM4310电机控制
}
/**
 * @brief 达妙关节电机控制函数
 * @param dm4310_ctrl：DM4310电机控制结构体指针
 */
void dm4310_motor_ctrl(dm4310_ctrl_4 *dm4310_ctrl)
{
    // 达妙关节电机1控制
    motor[Motor1].ctrl.pos_set = dm4310_ctrl->pos1;
    motor[Motor1].ctrl.vel_set = dm4310_ctrl->vel1;
    // 达妙关节电机2控制
    motor[Motor2].ctrl.pos_set = dm4310_ctrl->pos2;
    motor[Motor2].ctrl.vel_set = dm4310_ctrl->vel2; 
    // 达妙关节电机3控制
    motor[Motor3].ctrl.pos_set = dm4310_ctrl->pos3;
    motor[Motor3].ctrl.vel_set = dm4310_ctrl->vel3;
    // 达妙关节电机4控制
    motor[Motor4].ctrl.pos_set = dm4310_ctrl->pos4;
    motor[Motor4].ctrl.vel_set = dm4310_ctrl->vel4;
    ctrl_send();         
}
/**
 * @brief 张大头步进电机控制函数
 * @param vel1：电机1速度
 * @param vel2：电机2速度
 * @param vel3：电机3速度
 * @param vel4：电机4速度
 */
void stepper_motor_ctrl(int16_t vel1, int16_t vel2, int16_t vel3, int16_t vel4)
{
    uint16_t vel1_abs = 50;
    uint16_t vel2_abs = 50;
    uint16_t vel3_abs = 50;
    uint16_t vel4_abs = 50;

    // 张大头步进电机1控制
    if(vel1 > 0) 
    {
        vel1_abs = (uint16_t)vel1;
        Emm_V5_Vel_Control(ID1, 0, vel1_abs, 0, false);
    }
    else if(vel1 < 0)  
    {
        vel1_abs = (uint16_t)(-vel1);
        Emm_V5_Vel_Control(ID1, 1, vel1_abs, 0, false);
    }
    else if(vel1 == 0) 
    {
        Emm_V5_Emergency_Stop(ID1);
    }

    // 张大头步进电机2控制
    if(vel2 > 0) 
    {
        vel2_abs = (uint16_t)vel2;
        Emm_V5_Vel_Control(ID2, 1, vel2_abs, 0, false);
    }
    else if(vel2 < 0)  
    {
        vel2_abs = (uint16_t)(-vel2);
        Emm_V5_Vel_Control(ID2, 0, vel2_abs, 0, false);
    }
    else if(vel2 == 0) 
    {
        Emm_V5_Emergency_Stop(ID2);
    }

    // 张大头步进电机3控制
    if(vel3 > 0) 
    {
        vel3_abs = (uint16_t)vel3;
        Emm_V5_Vel_Control(ID3, 0, vel3_abs, 0, false);
    }
    else if(vel3 < 0)  
    {
        vel3_abs = (uint16_t)(-vel3);
        Emm_V5_Vel_Control(ID3, 1, vel3_abs, 0, false);
    }
    else if(vel3 == 0) 
    {
        Emm_V5_Emergency_Stop(ID3);
    }

    // 张大头步进电机4控制
    if(vel4 > 0) 
    {
        vel4_abs = (uint16_t)vel4;
        Emm_V5_Vel_Control(ID4, 1, vel4_abs, 0, false);
    }
    else if(vel4 < 0)  
    {
        vel4_abs = (uint16_t)(-vel4);
        Emm_V5_Vel_Control(ID4, 0, vel4_abs, 0, false);
    }
    else if(vel4 == 0) 
    {
        Emm_V5_Emergency_Stop(ID4);
    }
    // //打印出四个电机数据
    // printf("vel1:%d, vel2:%d, vel3:%d, vel4:%d\n", vel1, vel2, vel3, vel4);
}
/**
 * @brief 张大头步进电机急停
 */
void stepper_motor_stop(void)
{
    Emm_V5_Emergency_Stop(ID1);
    Emm_V5_Emergency_Stop(ID2);
    Emm_V5_Emergency_Stop(ID3);
    Emm_V5_Emergency_Stop(ID4);
}

