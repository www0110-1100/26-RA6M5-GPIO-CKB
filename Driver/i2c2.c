#include "i2c2.h"
#include "debug_uart7.h"
#include <stdio.h>
#include "types.h"

unsigned int timeout_ms = 500;

i2c_master_event_t g_i2c_callback_event;        //I2C2回调事件
static volatile bool iic_complete = false;      //I2C2完成标志位

/**
* @brief I2C2 回调函数
* @arg p_args:回调参数指针
*/
void i2c2_callback (i2c_master_callback_args_t * p_args)
{
    g_i2c_callback_event = p_args->event;
}

/**
* @brief I2C EEPROM初始化
*/
void I2C2_Init(void)
{
    //1. 重置回调事件
    g_i2c_callback_event = I2C_MASTER_EVENT_ABORTED;

    //2.I2C2初始化
    fsp_err_t err = R_IIC_MASTER_Open(&g_i2c_master2_ctrl, &g_i2c_master2_cfg);         
    if(err != FSP_SUCCESS)
    {
        printf("I2C2_Init no\r\n");
        return;
    }
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);        
    printf("I2C2_Init ok\r\n");
}

/**
* @brief 以单字节的方式到I2C2中
* @arg address:写地址
* @arg byte:写的数据
*/
void I2C2_ByteWrite(unsigned char address, unsigned char byte)
{
    iic_complete = false;
    unsigned char send_buffer[2] = {};
    send_buffer[0] = address;
    send_buffer[1] = byte;

    fsp_err_t err = R_IIC_MASTER_Write(&g_i2c_master2_ctrl, &send_buffer[0], 2, false);
     if(err != FSP_SUCCESS)
     {
        printf("I2C2_Write no\r\n");
         return;
     }
    printf("I2C2_Write ok\r\n");
    
    while ((I2C_MASTER_EVENT_TX_COMPLETE != g_i2c_callback_event) && timeout_ms)
    {
        R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);
        timeout_ms--;
        if(timeout_ms == 0)
        {
            printf("I2C2_Write time no\r\n");
            timeout_ms = 500;
            return;
        }
    }
    printf("I2C2_Write time ok\r\n");
    timeout_ms = 500;
}

/**
* @brief I2C2 读取数据
* @arg ptr_read:读取缓冲区指针
* @arg address:地址
* @arg byte:读取的字节数
*/
void I2C2_BufferRead(unsigned char* ptr_read,unsigned char address,unsigned char byte)
{
    unsigned char send_buffer[2] = {};
//    unsigned char read_buffer[1] = {};
    send_buffer[0] = address;

    fsp_err_t err = R_IIC_MASTER_Write(&g_i2c_master2_ctrl, &send_buffer[0], 1, true);
    if(err != FSP_SUCCESS)
    {
        printf("I2C2_read Write no\r\n");
        return;
    }
    printf("I2C2_read Write ok\r\n");

    while ((I2C_MASTER_EVENT_TX_COMPLETE != g_i2c_callback_event) && timeout_ms)
    {
        R_BSP_SoftwareDelay(400U, BSP_DELAY_UNITS_MICROSECONDS);
        timeout_ms--;
         if(timeout_ms == 0)
        {
            printf("I2C2_read Write time no\r\n");
            timeout_ms = 500;
            return;
        }
    }
    timeout_ms = 500;

    R_BSP_SoftwareDelay(250U, BSP_DELAY_UNITS_MICROSECONDS);

    err = R_IIC_MASTER_Read(&g_i2c_master2_ctrl, ptr_read, byte, false);
    if(err != FSP_SUCCESS)
    {
        printf("I2C2_read Read no\r\n");
        return;
    }
    printf("I2C2_read Read ok\r\n");
}



