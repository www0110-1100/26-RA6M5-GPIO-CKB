#include "PID.h"

/**
 * @description: PID计算函数
 * @param {PID_Struct} *pid PID参数
 * @param {float} dt 时间间隔 
 * @return {*}
 */
void Com_PID_Calculate(PID_Struct *pid, float dt)
{
    // 计算角度误差
    float error = pid->measure - pid->desire;    //测量值-目标值

    // 计算积分部分
    pid->integral += error * dt;

    // 计算微分部分
    float der = (error - pid->last_error) / dt;
    pid->last_error = error;

    pid->output = pid->kp * error + pid->ki * pid->integral + pid->kd * der;
}
/**
 * @description: 级联PID计算函数
 * @param {PID_Struct} *outter 外环PID参数
 * @param {PID_Struct} *inner 内环PID参数
 * @param {float} dt 时间间隔
 * @return {*}
 */
void Com_PID_Cascade(PID_Struct *outter, PID_Struct *inner, float dt)
{
    // 需要先计算外环
    Com_PID_Calculate(outter, dt);
    // 外环的输出值等于内环的目标值
    inner->desire = outter->output;
    // 再计算内环
    Com_PID_Calculate(inner, dt);
}



