#include "key_Task.h"
#include "hal_data.h"
#include "KEY.h"
#include "Int_electric.h"
/**
 * @brief 按键任务函数
 * @details 该函数用于处理按键事件，包括读取按键状态、处理按键动作等。
 */
void key_Task(void)
{
    if(KeyScan(KEY) == KEY_ON)
    {
       stepper_motor_stop();
    }
    else if(KeyScan(KEY1) == KEY_ON)
    {
       stepper_motor_stop();
    }
    else if(KeyScan(KEY2) == KEY_ON)
    {
        stepper_motor_stop();
    }
    else if(KeyScan(KEY3) == KEY_ON)
    {
        stepper_motor_stop();
    }
    else if(KeyScan(KEY4) == KEY_ON)
    {
        stepper_motor_stop();
    }
    else if(KeyScan(KEY5) == KEY_ON)
    {
        stepper_motor_stop();
    }
}





