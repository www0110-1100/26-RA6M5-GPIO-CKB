#ifndef __PID__
#define __PID__

#include "types.h"
#include "hal_data.h"

typedef struct
{
    float kp;         // 比例增益  恢复是否及时  过大会震荡 过小恢复慢
    float ki;         // 积分增益  消除稳态误差
    float kd;         // 微分增益  消除过调
    float desire;     // 目标值
    float measure;    // 测量值
    float integral;   // 误差积分
    float last_error; // 上一次的误差
    float output;
} PID_Struct;

/* PID的计算就是拿常量系数加目标值和测量值 计算得到输出值 */
void Com_PID_Calculate(PID_Struct *pid, float dt);

void Com_PID_Cascade(PID_Struct *outter, PID_Struct *inner, float dt);

#endif 


