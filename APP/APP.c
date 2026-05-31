#include "APP.h"
#include "KEY.h"
#include "LED.h"
#include "SysTick.h"
#include "uart5.h"
#include "gpt_pwm.h"
#include "Int_e2prom.h"
#include "scl_i2c4.h"
#include <stdio.h>
#include <stdlib.h>
#include "canfd0.h"
#include "canfd1.h"
#include "Int_motor.h"
#include "debug_uart7.h"
#include "dm4310_drv.h"
#include "dm4310_ctrl.h"
#include "types.h"
#include "Int_electric.h"
#include "Int_mpu6500.h"
#include "i2c2.h"
#include "imu_Task.h"
#include "stepper_Task.h"
#include "dm43x0_Task.h"
#include "robot_arm.h"
#include "spi1.h"
#include "uart3.h"
#include "display_Task.h"

uint32_t last_imu_tick = 0;
uint32_t last_stepper_tick = 0;
uint32_t last_dm_tick = 0;
uint32_t last_display_tick = 0;

volatile uint32_t sys_tick = 0;  // 系统时间戳
uint32_t Y_tick = 0;  // 任务时间戳，用于计算任务执行时间
uint32_t now = 0;
float stepper_dt = 0.0f;
uint16_t yaw_tt = 0;

void app_main(void)
{
    SysTick_Init(1000);    // 初始化SysTick，周期为1ms

    UART7_Init();          // 初始化UART7

    UART5_Init();          // 初始化UART5

    UART3_Init();          // 初始化UART3

    MPU6500_Init();        // 初始化MPU6500

    dm4310_stepper_init(); // 初始化DM43x0

   // 初始化DM43x0电机
    ctrl_enable();  
    dm4310_ctrl4.pos1 = 0.0f;
    dm4310_ctrl4.vel1 = 1.5f;
    dm4310_ctrl4.pos2 = 0.0f;
    dm4310_ctrl4.vel2 = 1.5f;
    dm4310_ctrl4.pos3 = 0.0f;
    dm4310_ctrl4.vel3 = 1.5f;
    dm4310_ctrl4.pos4 = 0.0f;
    dm4310_ctrl4.vel4 = 1.5f;
    dm4310_motor_ctrl(&dm4310_ctrl4);
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_SECONDS);

    // 初始化SPI显示
    fsp_err_t err;
    err = drv_spi_display_init();   
    if(FSP_SUCCESS != err)
    {
        printf ("%s %d\r\n", __FUNCTION__, __LINE__);
        __BKPT();
    }
    spi_display_show_color((uint16_t)LCD_COLOR_WHITE);

    while(1)
    {
      // 获取当前系统时间戳
        now = sys_tick;
        // 执行IMU任务，每10ms执行一次
        if (now - last_imu_tick >= 10) {
            App_Imu_Task();                // 执行IMU任务
         }
        // 执行步进电机任务，每10ms执行一次
        if (now - last_stepper_tick >= 10) {
            stepper_dt = ((float)now - (float)last_stepper_tick) / 100.0f;
            last_stepper_tick = now;
            App_Stepper_Task();
        }
        // 执行DM43x0任务，每10ms执行一次
        if (now - last_dm_tick >= 10) {
            last_dm_tick = now;
            App_Dm43x0_Task();
        }
        // 执行显示任务，每500ms执行一次
        if (now - last_display_tick >= 500) {
            last_display_tick = now;
            App_Display_Task();
        }
	}
}



void SyeTick_tt(void)
{
	sys_tick++;
    // if(car_status == CAR_PID || car_status == CAR_TURN || car_status == CAR_ADJUST)
    // {
    //     if(++yaw_tt >= 1000)
    //     {
    //         yaw_tt = 0;
    //         yaw_offset += 0.1f;
    //     }
    // }
}
