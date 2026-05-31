#ifndef __INT_MOTOR_H__
#define __INT_MOTOR_H__

#include "canfd1.h"
#include "hal_data.h"
#include <stdbool.h>
#include <stdint.h>

// 8个电机的地址定义
#define MOTOR_ADDR_1    0x01    // 电机1地址
#define MOTOR_ADDR_2    0x02    // 电机2地址
#define MOTOR_ADDR_3    0x03    // 电机3地址
#define MOTOR_ADDR_4    0x04    // 电机4地址
#define MOTOR_ADDR_5    0x05    // 电机5地址
#define MOTOR_ADDR_6    0x06    // 电机6地址
#define MOTOR_ADDR_7    0x07    // 电机7地址
#define MOTOR_ADDR_8    0x08    // 电机8地址

// 电机控制功能码定义
#define EMM_V5_FUNC_VEL_CONTROL    0xF6    // 速度控制功能码
#define EMM_V5_FUNC_POS_CONTROL    0xFD    // 位置控制功能码
#define EMM_V5_FUNC_STOP           0x81    // 急停功能码
#define EMM_V5_FUNC_READ_STATUS    0x9C    // 读取状态功能码

// 方向定义
#define EMM_V5_DIR_CW              0x00    // 顺时针方向
#define EMM_V5_DIR_CCW             0x01    // 逆时针方向

/**
 * @brief 速度模式控制
 * @param addr：电机地址 (0-255)
 * @param dir：方向，0为CW，其余值为CCW
 * @param vel：速度，范围0 - 5000RPM
 * @param acc：加速度，范围0 - 255，注意：0是直接启动
 * @param snF：多机同步标志，false为不启用，true为启用
 * @return 发送成功返回true，失败返回false
 */
bool Emm_V5_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF);

/**
 * @brief 位置模式控制
 * @param addr：电机地址
 * @param dir：方向
 * @param pos：目标位置（脉冲数）
 * @param vel：速度
 * @param acc：加速度
 * @param snF：多机同步标志
 * @return 发送成功返回true，失败返回false
 */
bool Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint32_t pos, uint16_t vel, uint8_t acc, bool snF);

/**
 * @brief 电机急停
 * @param addr：电机地址
 * @return 发送成功返回true，失败返回false
 */
bool Emm_V5_Emergency_Stop(uint8_t addr);

/**
 * @brief 读取电机状态
 * @param addr：电机地址
 * @return 发送成功返回true，失败返回false
 */
bool Emm_V5_Read_Status(uint8_t addr);

/**
 * @brief 初始化CANFD电机控制系统
 * @return 初始化成功返回true，失败返回false
 */
bool Emm_V5_Init(void);

#endif // __EMM_V5_MOTOR_H__
