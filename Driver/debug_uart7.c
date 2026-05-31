#include "debug_uart7.h"
#include "types.h"

/**
 * @brief 初始化UART7
 */
void UART7_Init(void)
{
    fsp_err_t err = FSP_SUCCESS;   // 错误码

    R_SCI_UART_Open(&g_uart7_ctrl, &g_uart7_cfg);

    assert(FSP_SUCCESS == err);   // 断言错误码为FSP_SUCCESS
}

volatile uint8_t uart7_send_complete_flag = false;

/**
 * @brief debug_UART7回调函数
 * @param p_args 回调参数指针
 */
void debug_uart7_callback(uart_callback_args_t *p_args)
{
    switch (p_args->event)
    {
        // 发送完成事件
        case UART_EVENT_TX_COMPLETE:
        {
            uart7_send_complete_flag = true;
        }
        break;
        
        // 接收字符事件
        case UART_EVENT_RX_CHAR:
        {
            R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)&(p_args->data), 1);
        }
        break;
        
        default:break;
    }
}

/* 重定向 printf 输出 */
#if defined __GNUC__ && !defined __clang__
int _write(int fd, char *pBuffer, int size); //防止编译警告
int _write(int fd, char *pBuffer, int size)
{
  (void)fd;
  R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)pBuffer, (uint32_t)size);
  while(uart7_send_complete_flag == false);
  uart7_send_complete_flag = false;

  return size;
}
#else
int fputc(int ch, FILE *f)
{
  (void)f;
  R_SCI_UART_Write(&g_uart7_ctrl, (uint8_t *)&ch, 1);
  while(uart7_send_complete_flag == false);
  uart7_send_complete_flag = false;

  return ch;
}
#endif

