#include "uart3.h"
#include <string.h>
#include <stdlib.h>

// 定义UART3发送完成标志位
volatile uint8_t uart3_send_complete_flag = false;
// UART3字符串接收缓冲区相关变量
volatile char uart3_rx_buffer[UART3_RX_BUFFER_SIZE];  // UART3接收缓冲区
volatile uint16_t uart3_rx_index = 0;                 //  UART3接收缓冲区索引
volatile bool uart3_data_ready = false;               // UART3数据接收完成标志位
volatile float uart3_received_float = 0.0f;           //  UART3接收的浮点数
uint8_t S_flag = 0;
uint8_t T_flag = 0;
uint8_t R_flag = 0;
uint8_t L_flag = 0;
uint8_t P_flag = 0;
uint8_t A_flag = 0;
uint8_t B_flag = 0;
// 药品显示标识位
uint8_t display_flag = 0;
// 药品显示缓冲区
char display_buffer[UART3_RX_BUFFER_SIZE];


/**
 * @brief 初始化UART3
 */
void UART3_Init(void)
{
    fsp_err_t err = FSP_SUCCESS;   // 错误码

    R_SCI_UART_Open(&g_uart3_ctrl, &g_uart3_cfg);
    // 断言错误码为FSP_SUCCESS
    assert(FSP_SUCCESS == err);
}

/**
 * @brief UART3回调函数
 * @param p_args 回调参数指针
 */
void uart3_callback(uart_callback_args_t *p_args) 
{
    switch (p_args->event)
    {
        // 发送完成事件
        case UART_EVENT_TX_COMPLETE:
        {
            uart3_send_complete_flag = true;
        }
        break;
        // 接收字符事件
        case UART_EVENT_RX_CHAR:
        {
            char received_char = (char)(p_args->data & 0xFF);
            
            if(received_char == 'A' )
            {
                A_flag = 1;
                uart3_rx_index = 0;
            }
            else if(received_char == 'B' )
            {
                B_flag = 1;
                uart3_rx_index = 0;
            }
            else if(received_char == 'S' )
            {
                S_flag = 1;
                uart3_rx_index = 0;
            }
            else if(received_char == 'T' )
            {
                T_flag = 1;
                uart3_rx_index = 0;
            }
            else if(received_char == 'R' )
            {
                R_flag = 1;
                uart3_rx_index = 0;
            }
            else if(received_char == 'L' )
            {
                L_flag = 1;
                uart3_rx_index = 0;
            }
            else if(received_char == 'F' )
            {
                P_flag = 1;
                uart3_rx_index = 0;
            }
            // 检查是否为回车符（表示字符串结束）
            else if (received_char == '\r' || received_char == '\n')
            {
                if (uart3_rx_index > 0)
                {
                    // 字符串结束，添加结束符
                    uart3_rx_buffer[uart3_rx_index] = '\0';
                    uart3_data_ready = true;
                    uart3_rx_index = 0;
                }
            }
            else if(received_char == '*')
            {
                display_flag = 1;
                uart3_rx_buffer[uart3_rx_index] = '\0';
                // 复制到显示缓冲区
                strncpy(display_buffer, (const char *)uart3_rx_buffer, UART3_RX_BUFFER_SIZE);
                uart3_rx_index = 0;
            }
            // 处理普通字符
            else if (uart3_rx_index < UART3_RX_BUFFER_SIZE - 1)
            {
                uart3_rx_buffer[uart3_rx_index++] = received_char;
            }
            else
            {
                // 缓冲区溢出，重置
                uart3_rx_index = 0;
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
bool UART3_IsDataReady(void)
{
    return uart3_data_ready;
}
/**
 * @brief 获取接收到的浮点数数据
 * @return 转换后的浮点数，如果转换失败返回0.0f
 */
float UART3_GetReceivedFloat(void)
{
    if (!uart3_data_ready)
    {
        return 160.0f;
    }
    
    // 将字符串转换为浮点数
    char *endptr;
    // float result = strtof(uart5_rx_buffer, &endptr);
    float result = strtof((const char *)uart3_rx_buffer, &endptr);
    
    // 检查转换是否成功
    if (endptr == uart3_rx_buffer || *endptr != '\0')
    {
        // // 转换失败
        // char error_msg[64];
        // snprintf(error_msg, sizeof(error_msg), "Convert error: %s\r\n", uart3_rx_buffer);
        // R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)error_msg, strlen(error_msg));
        uart3_data_ready = false;
        return 160.0f;
    }
    // 转换成功
    uart3_received_float = result;
    uart3_data_ready = false;
    return result;
}
/**
 * @brief 重置接收缓冲区
 */
void UART3_ResetBuffer(void)
{
    uart3_rx_index = 0;
    uart3_data_ready = false;
    memset((void*)uart3_rx_buffer, 0, UART3_RX_BUFFER_SIZE);
}



