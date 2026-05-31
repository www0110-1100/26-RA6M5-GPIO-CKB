#include "canfd1.h"
#include <stdio.h>
#include "uart5.h"
#include "debug_uart7.h"
#include <string.h>
#include "types.h"
#include "dm4310_drv.h"
#include "dm4310_ctrl.h"
/*已改为标准ID模式*/
 /**
  * @brief CANFD1初始化函数
  */
void CANFD1_Init(void)
{
    fsp_err_t err = R_CANFD_Open(&g_canfd1_ctrl, &g_canfd1_cfg);
    if (FSP_SUCCESS != err)
    {
        printf("CANFD1 init no\r\n");
    }
}
/**
 * @brief CANFD1验收过滤器列表(AFL)规则数组
 */
const canfd_afl_entry_t p_canfd1_afl[CANFD_CFG_AFL_CH1_RULE_NUM] =
{
    {
        .id =
        {
            /* 指定要接受的ID、ID类型和帧类型。 */
           .id         = 0x02,               // 标准数据帧ID为101
           .frame_type = CAN_FRAME_TYPE_DATA,      // 数据帧类型
           .id_mode    = CAN_ID_MODE_STANDARD      // 标准ID模式
        },

        .mask =
        {
            /* 这些值屏蔽了过滤邮件时要比较的ID/模式位。 */
           .mask_id         = 0x7FF,              // 标准数据帧ID掩码，0x7FF表示不屏蔽ID位
           .mask_frame_type = 0,                  // 数据帧类型掩码，1表示屏蔽数据帧类型位
           .mask_id_mode    = 0                   // 标准ID模式掩码，1表示屏蔽标准ID模式位
        },

        .destination =
        {
           //如果启用了动态加载内容（DLC）检查功能，那么任何长度小于以下设定值的消息都将被拒绝。
           .minimum_dlc = CANFD_MINIMUM_DLC_0,

           //可选地指定一个接收消息缓冲区（RX MB）来存储已接受的帧。RX MB 不具有中断功能或覆盖保护功能，
           //必须通过 R_CANFD_InfoGet 和 R_CANFD_Read 进行检查。
           .rx_buffer   = CANFD_RX_MB_NONE,

           //指定将过滤后的消息发送到哪个 FIFO（接收消息缓冲区）。可以使用按位或运算符（|）组合多个 FIFO。
           .fifo_select_flags = CANFD_RX_FIFO_0
        }
    },
};

/* CAN 帧 */
//每帧包括ID(身份识别码),ID_MODE(识别模式),TYPE(帧类型)
//data_length_code(数据长度编码),options(选项),data(数据)
can_frame_t canfd1_tx_frame;        //CANFD1发送帧
can_frame_t canfd1_rx_frame;        //CANFD1接收帧

/* 保存帧接收状态信息 */
can_info_t canfd1_rx_info;          //CANFD1接收帧状态信息

/* 要在回调函数中设置的标志 */
volatile bool canfd1_tx_complete_flag = false;                     //CANFD1发送完成标志位
volatile bool canfd1_rx_complete_flag = false;                     //CANFD1接收完成标志位
volatile bool canfd1_err_status_flag = false;                      //CANFD1错误状态标志位
volatile canfd_error_t canfd1_err_status = (canfd_error_t) 0;      //CANFD1错误状态

extern void canfd1_callback(can_callback_args_t * p_args);

uint16_t rec_id;
uint8_t rx_data[8] = {0};
uint8_t can_rx_flag = 0;

/**
 * @brief CANFD1中断回调函数
 * @param p_args 回调参数指针
 */
void canfd1_callback(can_callback_args_t * p_args)
{
    switch (p_args->event)
    {
        case CAN_EVENT_RX_COMPLETE:         //接收完成中断
        {
            canfd1_rx_complete_flag = true; //canfd1接收到数据
            can_rx_flag = 1;
            /* 读取接收帧 */
            memcpy(&canfd1_rx_frame, &(p_args->frame),sizeof(can_frame_t));  //将接收帧数据复制到canfd1_rx_frame，包括ID、ID_MODE、TYPE、DLC、DATA
            break;
        }
        case CAN_EVENT_TX_COMPLETE:     //传输完成中断
        {
            canfd1_tx_complete_flag = true; //canfd1数据发送完成

            break;
        }
        case CAN_EVENT_ERR_WARNING:             //错误警告事件
        case CAN_EVENT_ERR_PASSIVE:             //错误被动事件
        case CAN_EVENT_ERR_BUS_OFF:             //错误总线离线事件
        case CAN_EVENT_BUS_RECOVERY:            //总线恢复错误事件
        case CAN_EVENT_MAILBOX_MESSAGE_LOST:    //邮箱消息丢失事件
        case CAN_EVENT_ERR_BUS_LOCK:            //总线锁检测事件(32个连续的 dominant 位)
        case CAN_EVENT_ERR_CHANNEL:             //通道错误事件
        case CAN_EVENT_TX_ABORTED:              //发送中止事件
        case CAN_EVENT_ERR_GLOBAL:              //全局错误事件
        case CAN_EVENT_TX_FIFO_EMPTY:           //发送FIFO为空事件
        {
            canfd1_err_status_flag = true;  //设置标志位

            /* 获取错误状态 */
            canfd1_err_status = (canfd_error_t) p_args->error;

            break;
        }
        default:
        {
            break;
        }
    }
}

/**
 * @brief 发送CANFD1命令到电机
 * @param id：CAN ID
 * @param data：命令数据数组
 * @param length：数据长度
 * @return 发送成功返回true，失败返回false
 */
bool canfd1_send(uint16_t id, uint8_t *p_data, uint8_t length)
{
    can_frame_t tx_frame;                           //CAN帧结构体
    uint32_t time_out = CANFD1_WAIT_TIME;           //等待时间
    fsp_err_t err;
    
    // 初始化CAN帧
    memset(&tx_frame, 0, sizeof(can_frame_t));
    
    tx_frame.id = id & 0x7FF;  // 确保标准帧ID只使用低11位
    // //打印出tx_frame.id
    // printf("CANFD1 write id:0x%08X\r\n", tx_frame.id);
    tx_frame.id_mode = CAN_ID_MODE_STANDARD;             // 标准ID模式
    tx_frame.type = CAN_FRAME_TYPE_DATA;                 // 数据帧类型
    tx_frame.data_length_code = length;                  // 数据长度
    tx_frame.options = 0;                                // 选项设置为0

    // 复制命令数据到CAN帧
    memcpy(tx_frame.data, p_data, length);
    
    // 发送CAN帧（使用你现有的CANFD发送函数）
    // g_canfd1_ctrl : CANFD1控制块指针   CAN_MAILBOX_NUMBER_0 : 邮箱0  tx_frame : 发送帧指针
    err = R_CANFD_Write(&g_canfd1_ctrl, CAN_MAILBOX_NUMBER_0, &tx_frame);
    if (FSP_SUCCESS != err)
    {
        // printf("CANFD1 write no\r\n");
        // printf("%d\r\n", err);
        return false;
    }
    // printf("CANFD1 write ok\r\n");

    // 等待发送完成
    while ((true != canfd1_tx_complete_flag) && (--time_out))          //等待发送完成
    {
        //延迟1ms
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
    }
    canfd1_tx_complete_flag = false;                                    //发送完成标志位复位

    if (0 == time_out)                                                  //如果等待时间为0
    {
        // printf("CANFD1 write time on\r\n");
        return false;
    }
    // printf("CANFD1 write time ok\r\n");

    // //打印出data
    // printf("CANFD1 write data:");
    // for (uint8_t i = 0; i < length; i++)
    // {
    //     printf("%02X ", p_data[i]);
    // }
    // printf("\r\n");

    return true;
}

/**
* @brief CANFD通道1接收数据
* @param ptDev CANFD通道1设备结构体指针
* @param frame 接收数据缓存帧指针
* @return int 接收结果，0表示成功，非0表示失败
*/
void canfd1_read(void)
{
    if (true == canfd1_rx_complete_flag)
    {
        canfd1_rx_complete_flag = false; // 清除接收完成标志位
        rec_id =  (uint16_t)(canfd1_rx_frame.id & 0x7FF);
        memcpy(rx_data, canfd1_rx_frame.data, canfd1_rx_frame.data_length_code); 
        if(rec_id == 0x01)
        {
            dm4310_fbdata(&motor[Motor1], rx_data); 
        }
        if(rec_id == 0x02)
        {
            dm4310_fbdata(&motor[Motor2], rx_data); 
        }
        if(rec_id == 0x03)
        {
            dm4310_fbdata(&motor[Motor3], rx_data); 
        }
        if(rec_id == 0x04)
        {
            dm4310_fbdata(&motor[Motor4], rx_data); 
        }
    }
}


// switch (rec_id)
// {
//     case 0x01: dm4310_fbdata(&motor[Motor1], rx_data); 
//     break;
//     case 0x02: dm4310_fbdata(&motor[Motor2], rx_data); 
//     break;
//     case 0x03: dm4310_fbdata(&motor[Motor3], rx_data); 
//     break;
//     case 0x04: dm4310_fbdata(&motor[Motor4], rx_data); 
//     break;
//     default:
//         break;
// }
