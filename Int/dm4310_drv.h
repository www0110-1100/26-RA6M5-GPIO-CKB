#ifndef __DM4310_DRV_H__
#define __DM4310_DRV_H__
#include "hal_data.h"
#include "canfd1.h"

#define MIT_MODE 		0x000
#define POS_MODE		0x100
#define SPEED_MODE		0x200


#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -30.0f
#define V_MAX 30.0f
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -10.0f
#define T_MAX 10.0f

// 电机回传信息结构体
typedef struct 
{
	int id;				// 电机ID
	int state;			// 电机状态
	int p_int;			// 位置积分值
	int v_int;			// 速度积分值
	int t_int;			// 力矩积分值
	int kp_int;			// 位置环比例系数积分值
	int kd_int;			// 位置环微分系数积分值
	float pos;			// 位置值
	float vel;			// 速度值
	float tor;			// 力矩值
	float Kp;			// 位置环比例系数
	float Kd;			// 位置环微分系数
	float Tmos;			// 电机最大输出力矩
	float Tcoil;		// 电机线圈最大输出力矩
}motor_fbpara_t;

// 电机参数设置结构体
typedef struct 
{
	int8_t mode;		// 0: MIT模式   1: 位置速度模式   2: 速度模式
	float pos_set;		// 位置设置值
	float vel_set;		// 速度设置值
	float tor_set;		// 力矩设置值
	float kp_set;		// 位置环比例系数
	float kd_set;		// 位置环微分系数
}motor_ctrl_t;

typedef struct
{
	uint16_t id;			// 电机ID
	uint8_t start_flag;		// 电机启动标志
	motor_fbpara_t para;	// 电机回传参数
	motor_ctrl_t ctrl;		// 电机控制参数
	motor_ctrl_t cmd;		// 电机命令参数
}motor_t;

float uint_to_float(int x_int, float x_min, float x_max, int bits);
uint16_t float_to_uint(float x_float, float x_min, float x_max, int bits);
void dm4310_ctrl_send(motor_t *motor);
void dm4310_enable(motor_t *motor);
void dm4310_disable(motor_t *motor);
void dm4310_set(motor_t *motor);
void dm4310_clear_para(motor_t *motor);
void dm4310_clear_err(motor_t *motor);
void dm4310_fbdata(motor_t *motor, uint8_t *rx_data);

void enable_motor_mode(uint16_t motor_id, uint16_t mode_id);
void disable_motor_mode(uint16_t motor_id, uint16_t mode_id);
void mit_ctrl(uint16_t motor_id, float pos, float vel,float kp, float kd, float torq);
void pos_speed_ctrl(uint16_t motor_id, float pos, float vel);
void speed_ctrl(uint16_t motor_id, float _vel);
void save_pos_zero(uint16_t motor_id, uint16_t mode_id);
void clear_err(uint16_t motor_id, uint16_t mode_id);

#endif /* __DM4310_DRV_H__ */

