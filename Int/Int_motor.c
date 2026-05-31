#include "Int_motor.h"
#include "canfd0.h"
#include <string.h>
#include "canfd1.h"
#include "debug_uart7.h"
#include "types.h"


/**
 * @brief 速度模式控制
 * @param addr：电机地址
 * @param dir：方向（0：正转，1：反转）
 * @param vel：速度（RPM）
 * @param acc：加速度
 * @param snF：多机同步标志（true：同步，false：异步）
 * @return 控制成功返回true，失败返回false
 */
bool Emm_V5_Vel_Control(uint8_t addr, uint8_t dir, uint16_t vel, uint8_t acc, bool snF)
{
    uint8_t cmd[16] = {0};
    uint32_t id = (uint32_t)(addr << 8);
    
    // 装载命令
//    cmd[0] = addr;                       // 地址
    cmd[0] = EMM_V5_FUNC_VEL_CONTROL;    // 功能码
    cmd[1] = dir;                        // 方向
    cmd[2] = (uint8_t)(vel >> 8);        // 速度(RPM)高8位字节
    cmd[3] = (uint8_t)(vel >> 0);        // 速度(RPM)低8位字节
    cmd[4] = acc;                        // 加速度，注意：0是直接启动
    cmd[5] = snF;                        // 多机同步运动标志
    cmd[6] = 0x6B;                       // 校验字节（根据实际协议计算）
    
	canfd0_send(id, cmd, 7);
		
    // 发送命令
    return 0;
}

/**
 * @brief 位置模式控制
 * @param addr：电机地址
 * @param dir：方向（0：正转，1：反转）
 * @param pos：位置（单位：根据电机协议）
 * @param vel：速度（单位：根据电机协议）
 * @param acc：加速度
 * @param snF：多机同步标志（true：同步，false：异步）
 * @return 控制成功返回true，失败返回false
 */
bool Emm_V5_Pos_Control(uint8_t addr, uint8_t dir, uint32_t pos, uint16_t vel, uint8_t acc, bool snF)
{
   uint8_t cmd[12] = {0};
   uint32_t id = (uint32_t)(addr << 8);
   
   // 装载命令
   // cmd[0] = addr;                       // 地址
   cmd[0] = EMM_V5_FUNC_POS_CONTROL;    // 功能码
   cmd[1] = dir;                        // 方向
   cmd[2] = (uint8_t)(vel >> 8);        // 速度高8位
   cmd[3] = (uint8_t)(vel >> 0);        // 速度低8位
   cmd[4] = acc;                        // 加速度

   cmd[5] = (uint8_t)(pos >> 24);       // 位置高8位
   cmd[6] = (uint8_t)(pos >> 16);
   cmd[7] = (uint8_t)(pos >> 8);        // 位置低8位
   cmd[8] = (uint8_t)(pos >> 0);        

   cmd[9] = snF;                        // 多机同步标志
   cmd[10] = 0x6B;                      // 校验字节
   
   canfd0_send(id, cmd, 11);
		
   return 0;
}

/**
 * @brief 电机急停
 * @param addr：电机地址
 * @return 控制成功返回true，失败返回false
 */
bool Emm_V5_Emergency_Stop(uint8_t addr)
{
    uint8_t cmd[5] = {0};
    uint32_t id = (uint32_t)(addr << 8);
    
    cmd[0] = 0xFE;           
    cmd[1] = 0x98;
    cmd[2] = 0x00;
    cmd[3] = 0x6B;                     

    canfd0_send(id, cmd, 4);
		
    return 0;
}

/**
 * @brief 读取电机状态
 * @param addr：电机地址
 * @return 控制成功返回true，失败返回false
 */
bool Emm_V5_Read_Status(uint8_t addr)
{
    uint8_t cmd[3] = {0};
    uint32_t id = (uint32_t)(addr << 8);
    
    // cmd[0] = addr;                       // 地址
    cmd[0] = EMM_V5_FUNC_READ_STATUS;    // 读取状态功能码
    cmd[1] = 0x6B;                       // 校验字节
    
	canfd0_send(id, cmd, 2);
		
    return 0;
}

/**
 * @brief 初始化CANFD电机控制系统
 * @return 初始化成功返回true，失败返回false
 */
bool Emm_V5_Init(void)
{
    // 初始化CANFD0（使用你现有的初始化函数）
    CANFD0_Init();
    
    // 这里可以添加其他电机控制相关的初始化代码
    return true;
}
