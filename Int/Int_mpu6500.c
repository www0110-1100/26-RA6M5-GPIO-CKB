#include "Int_mpu6500.h"
#include "i2c2.h"
#include "types.h"
#include "stdio.h"
#include "debug_uart7.h"
#include "math.h"
#include "IMU.h"
#include "kalman.h"

// 陀螺仪加速度结构体
Gyro_Acc_Struct gyro_acc_bias;

/**
 * @description: 根据采样率设置低通滤波器
 * @param {uint16_t} rate
 * @return {*}
 */
void MPU6500_Set_DLPF_CFG(uint16_t rate)
{
    /* 采样定理： 采样率 >= 2*带宽 才不会失真 ===》 带宽 <= 采样率/2 */
    uint8_t cfg = 0;
    rate = rate / 2;
    if (rate > 188)
    {
        cfg = 1;
    }
    else if (rate > 98)
    {
        cfg = 2;
    }
    else if (rate > 42)
    {
        cfg = 3;
    }
    else if (rate > 20)
    {
        cfg = 4;
    }
    else if (rate > 10)
    {
        cfg = 5;
    }
    else
    {
        cfg = 6;
    }
    I2C2_ByteWrite(MPU_CFG_REG, cfg);
}

/**
 * @description: 设置陀螺仪的采样率
 * @param {uint16_t} rate
 * @return {*}
 */
void MPU6500_SetGyroRate(uint16_t rate)
{
    /* 采样率=输出频率/(1+分频值) ===》 分频值 = (输出频率/采样率) -1 */
    uint8_t sample_div = 0;
    /* 1. 采样率限幅:产品说明书12页，输出频率最小值=4，最大是8k，我们期望最大是1k */
    if (rate < 4)
    {
        rate = 4;
    }
    else if (rate > 1000)
    {
        rate = 1000;
    }

    /* 2. 根据期望的采样率，计算出分频值 */
    sample_div = (uint8_t)1000 / rate - 1;

    /* 3. 将分频值设置到寄存器中 */
    I2C2_ByteWrite(MPU_SAMPLE_RATE_REG, sample_div);

    /* 4. 根据采样率去设置低通滤波器 */
    MPU6500_Set_DLPF_CFG(rate);
}

uint8_t dev_id = 0;

/**
 * @description: 初始化
 * @return {*}
 */
void MPU6500_Init(void)
{
    //1.初始化I2C2 -》延迟一会
    I2C2_Init();
    R_BSP_SoftwareDelay(300, BSP_DELAY_UNITS_MILLISECONDS);
    /* 2 复位 -》延迟一会 -》 唤醒 */
    I2C2_ByteWrite(MPU_PWR_MGMT1_REG, 0x80);
    R_BSP_SoftwareDelay(300, BSP_DELAY_UNITS_MILLISECONDS);
    I2C2_ByteWrite(MPU_PWR_MGMT1_REG, 0x00);

    /* 3 陀螺仪量程 +-2000°/s, fsr=3 */
    I2C2_ByteWrite(MPU_GYRO_CFG_REG, 3 << 3);

    /* 4 加速度量程 */
    I2C2_ByteWrite(MPU_ACCEL_CFG_REG, 0 << 3);
    /* 5.系统时钟源、陀螺仪采样率、低通滤波的设置 */
    /* 配置时钟源之前，确认正常工作，读一下id */
    I2C2_BufferRead(&dev_id, MPU_DEVICE_ID_REG, 1);

    //打印出id
    printf("MPU6500 id:0x%02x\n", dev_id);

    if (dev_id == MPU_IIC_ADDR)
    {
        printf("MPU6500_Init OK!\n");
        /* 6.1 设置时钟源：陀螺仪X轴的时钟，精度更高 */
        I2C2_ByteWrite(MPU_PWR_MGMT1_REG, 0x01);
        /* 6.2 设置陀螺仪采样率、低通滤波 */
        MPU6500_SetGyroRate(100);
        /* 6.3 让两个传感器退出待机模式，进入正常工作状态*/
        I2C2_ByteWrite(MPU_PWR_MGMT2_REG, 0x00);
    }
}

/**
 * @description: 读取陀螺仪角速度的数据，拼接两字节数据且转成带符号的类型
 * @param {short} *gx
 * @param {short} *gy
 * @param {short} *gz
 * @return {*}
 */
void MPU6500_Get_Gyro(Gyro_Struct *gyro)
{
    uint8_t buff[6];
    I2C2_BufferRead(buff, MPU_GYRO_XOUTH_REG, 6);

    /*
        buff[0]:角速度X轴高8位
        buff[1]:角速度X轴低8位
        buff[2]:角速度Y轴高8位
        buff[3]:角速度Y轴低8位
        buff[4]:角速度Z轴高8位
        buff[5]:角速度Z轴低8位
     */
    gyro->x = (int16_t)((buff[0] << 8) | buff[1]);
    gyro->y = (int16_t)((buff[2] << 8) | buff[3]);
    gyro->z = (int16_t)((buff[4] << 8) | buff[5]);
}

/**
 * @description: 读取加速度计的加速度数据，拼接两字节数据且转成带符号的类型
 * @param {short} *ax
 * @param {short} *ay
 * @param {short} *az
 * @return {*}
 */
void MPU6500_Get_Acce(Acc_Struct *acc)
{
    uint8_t buff[6];
    I2C2_BufferRead(buff, MPU_ACCEL_XOUTH_REG, 6);

    /*
        buff[0]:加速度X轴高8位
        buff[1]:加速度X轴低8位
        buff[2]:加速度Y轴高8位
        buff[3]:加速度Y轴低8位
        buff[4]:加速度Z轴高8位
        buff[5]:加速度Z轴低8位
     */
    acc->x = (int16_t)((buff[0] << 8) | buff[1]);
    acc->y = (int16_t)((buff[2] << 8) | buff[3]);
    acc->z = (int16_t)((buff[4] << 8) | buff[5]);
}

short gx, gy, gz;
short ax, ay, az;

/**
 * @description: 读取陀螺仪角速度和加速度数据
 * @param  Gyro_Acc_Struct *gyro_acc 指向陀螺仪加速度结构体的指针
 * @return {*}
 */
void MPU6500_Read(Gyro_Acc_Struct *gyro_acc)
{
	MPU6500_Get_Gyro(&gyro_acc->gyro);
    MPU6500_Get_Acce(&gyro_acc->acc);

    // 使用校准值
    gyro_acc->gyro.x -= gyro_acc_bias.gyro.x;
    gyro_acc->gyro.y -= gyro_acc_bias.gyro.y;
    gyro_acc->gyro.z -= gyro_acc_bias.gyro.z;

    gyro_acc->acc.x -= gyro_acc_bias.acc.x;
    gyro_acc->acc.y -= gyro_acc_bias.acc.y;
    gyro_acc->acc.z -= gyro_acc_bias.acc.z;
}

