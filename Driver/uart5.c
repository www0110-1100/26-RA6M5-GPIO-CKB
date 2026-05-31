#include "uart5.h"
#include "types.h"
#include "dm43x0_Task.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// 定义UART5发送完成标志位
volatile uint8_t uart5_send_complete_flag = false;
// UART5字符串接收缓冲区相关变量
volatile char uart5_rx_buffer[UART5_RX_BUFFER_SIZE];        // UART5接收缓冲区
volatile uint16_t uart5_rx_index = 0;                       // 接收缓冲区索引
volatile bool uart5_data_ready = false;                     // 数据接收完成标志位
volatile float uart5_received_float = 0.0f;                 // 接收的浮点数

// UART5指令标志位
uint8_t U5_D_flag = 0;  // 夹爪继续前伸



/**
 * @brief 初始化UART5
 */
void UART5_Init(void)
{
    fsp_err_t err = FSP_SUCCESS;   // 错误码
    R_SCI_UART_Open(&g_uart5_ctrl, &g_uart5_cfg);
    assert(FSP_SUCCESS == err);   // 断言错误码为FSP_SUCCESS
}

/**
 * @brief 处理指令字符
 * @param cmd_char 指令字符
 */
static void UART5_ProcessCommand(char cmd_char)
{
    switch(cmd_char)
    {
        case 'D':  // 夹爪继续前伸
            U5_D_flag = 1;
            break;
        default:
            break;
    }
}

/**
 * @brief UART5回调函数
 * @param p_args 回调参数指针
 */
void uart5_callback(uart_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        // 发送完成事件
        case UART_EVENT_TX_COMPLETE:
        {
            uart5_send_complete_flag = true;
        }
        break;
        
        // 接收字符事件
        case UART_EVENT_RX_CHAR:
        {
            char received_char = (char)(p_args->data & 0xFF);
            
            // 检查是否为指令字符（单字符指令）
            if(received_char == 'D' )
            {
                UART5_ProcessCommand(received_char);
                uart5_rx_index = 0;
            }
            // 检查是否为回车符（表示字符串结束）
            else if (received_char == '\r' || received_char == '\n')
            {
                if (uart5_rx_index > 0)
                {
                    // 字符串结束，添加结束符
                    uart5_rx_buffer[uart5_rx_index] = '\0';
                    uart5_data_ready = true;
                    uart5_rx_index = 0;
                }
            }
            // 处理普通字符（数据字符串）
            else if (uart5_rx_index < UART5_RX_BUFFER_SIZE - 1)
            {
                uart5_rx_buffer[uart5_rx_index++] = received_char;
            }
            else
            {
                // 缓冲区溢出，重置
                uart5_rx_index = 0;
            }
        }
        break;
        default:break;
    }
}

/**
 * @brief 检查是否有新数据可用
 * @return true: 有新数据, false: 无新数据
 */
bool UART5_IsDataReady(void)
{
    return uart5_data_ready;
}

/**
 * @brief 获取接收到的浮点数数据
 * @return 转换后的浮点数，如果转换失败返回0.0f
 */
void UART5_GetReceivedFloat(void)
{
    if (!uart5_data_ready)
    {
        return;
    }
    // 创建临时非volatile副本以避免类型限定符冲突
    char temp_buffer[UART5_RX_BUFFER_SIZE];
    strncpy(temp_buffer, (const char *)uart5_rx_buffer, UART5_RX_BUFFER_SIZE);
    temp_buffer[UART5_RX_BUFFER_SIZE - 1] = '\0'; // 确保字符串终止
    
    char prefix = temp_buffer[0];
    const char *num_str = temp_buffer + 1; // 跳过前缀字符
    
    switch(prefix)
    {
        case 'Z':  // Y轴坐标
            y_drug = (float)atof(num_str);
            break;
        case 'Y':  // 夹爪俯仰角度
            claw_distance = (float)atof(num_str);
            break;
        case 'X':  // X轴坐标
            x_drug = (float)atof(num_str);
            break;
        default:
            break;
    }

    // // 转换成功
    uart5_data_ready = false;
}

/**
 * @brief 重置接收缓冲区
 */
void UART5_ResetBuffer(void)
{
    uart5_rx_index = 0;
    uart5_data_ready = false;
    memset((void*)uart5_rx_buffer, 0, UART5_RX_BUFFER_SIZE);
}




// #include "uart5.h"
// #include "types.h"
// #include "dm43x0_Task.h"
// #include <string.h>
// #include <stdlib.h>

// // 定义UART5发送完成标志位
// volatile uint8_t uart5_send_complete_flag = false;
// // UART5字符串接收缓冲区相关变量
// volatile char uart5_rx_buffer[UART5_RX_BUFFER_SIZE];
// volatile uint16_t uart5_rx_index = 0;
// volatile bool uart5_data_ready = false;
// volatile float uart5_received_float = 0.0f;

// /**
//  * @brief 初始化UART5
//  */
// void UART5_Init(void)
// {
//     fsp_err_t err = FSP_SUCCESS;   // 错误码

//     R_SCI_UART_Open(&g_uart5_ctrl, &g_uart5_cfg);

//     assert(FSP_SUCCESS == err);   // 断言错误码为FSP_SUCCESS
// }

// /**
//  * @brief UART5回调函数
//  * @param p_args 回调参数指针
//  */
// void uart5_callback(uart_callback_args_t *p_args)
// {
//     switch (p_args->event)
//     {
//         // 发送完成事件
//         case UART_EVENT_TX_COMPLETE:
//         {
//             uart5_send_complete_flag = true;
//         }
//         break;
//         // 接收字符事件
//         case UART_EVENT_RX_CHAR:
//         {
//             char received_char = (char)(p_args->data & 0xFF);
            
//             // 检查是否为回车符（表示字符串结束）
//             if (received_char == '\r' || received_char == '\n')
//             {
//                 if (uart5_rx_index > 0)
//                 {
//                     // 字符串结束，添加结束符
//                     uart5_rx_buffer[uart5_rx_index] = '\0';
//                     uart5_data_ready = true;
                    
//                     // 调试输出：将接收到的字符串发送到UART7
//                     char debug_msg[64];
//                     snprintf(debug_msg, sizeof(debug_msg), "Recv str: %s\r\n", uart5_rx_buffer);
//                     R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)debug_msg, strlen(debug_msg));
                    
//                     uart5_rx_index = 0;
//                 }
//             }
//             // 处理普通字符
//             else if (uart5_rx_index < UART5_RX_BUFFER_SIZE - 1)
//             {
//                 uart5_rx_buffer[uart5_rx_index++] = received_char;
                
//                 // 实时回显接收到的字符
//                 R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)&received_char, 1);
//             }
//             else
//             {
//                 // 缓冲区溢出，重置
//                 uart5_rx_index = 0;
                
//                 // 发送溢出警告
//                 const char *overflow_msg = "Buffer overflow!\r\n";
//                 R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)overflow_msg, strlen(overflow_msg));
//             }
//         }
//         break;
        
//         default:break;
//     }
// }
// /**
//  * @brief 检查是否有新数据可用
//  * @return true: 有新数据, false: 无新数据
//  */
// bool UART5_IsDataReady(void)
// {
//     return uart5_data_ready;
// }
// /**
//  * @brief 获取接收到的浮点数数据
//  * @return 转换后的浮点数，如果转换失败返回0.0f
//  */
// float UART5_GetReceivedFloat(void)
// {
//     if (!uart5_data_ready)
//     {
//         return 0.0f;
//     }
    
//     // 将字符串转换为浮点数
//     char *endptr;
//     // float result = strtof(uart5_rx_buffer, &endptr);
//     float result = strtof((const char *)uart5_rx_buffer, &endptr);
    
//     // 检查转换是否成功
//     if (endptr == uart5_rx_buffer || *endptr != '\0')
//     {
//         // 转换失败
//         char error_msg[64];
//         snprintf(error_msg, sizeof(error_msg), "Convert error: %s\r\n", uart5_rx_buffer);
//         R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)error_msg, strlen(error_msg));
        
//         uart5_data_ready = false;
//         return 0.0f;
//     }
    
//     // 数据范围检查
//     if (result < UART5_MIN_VALUE || result > UART5_MAX_VALUE)
//     {
//         char range_msg[64];
//         snprintf(range_msg, sizeof(range_msg), "Out of range: %.2f\r\n", result);
//         R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)range_msg, strlen(range_msg));
        
//         uart5_data_ready = false;
//         return 0.0f;
//     }
    
//     // 转换成功
//     char success_msg[64];
//     snprintf(success_msg, sizeof(success_msg), "Convert success: %.2f\r\n", result);
//     R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)success_msg, strlen(success_msg));
    
//     uart5_received_float = result;
//     uart5_data_ready = false;
//     return result;
// }
// /**
//  * @brief 重置接收缓冲区
//  */
// void UART5_ResetBuffer(void)
// {
//     uart5_rx_index = 0;
//     uart5_data_ready = false;
//     memset((void*)uart5_rx_buffer, 0, UART5_RX_BUFFER_SIZE);
// }
// /**
//  * @brief 接收字符处理函数（兼容旧版本）
//  */
// void UART5_Receive_Char(void)
// {
//     // 如果有完整的数据，进行浮点数转换
//     if (uart5_data_ready)
//     {
//         float received_value = UART5_GetReceivedFloat();
        
//         // 可以在这里添加进一步的处理逻辑
//         // 例如：将浮点数用于控制算法等
//         (void)received_value; // 消除未使用变量警告
//     }
// }





