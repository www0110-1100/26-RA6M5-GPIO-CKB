#ifndef __INT_MPU6500_H__
#define __INT_MPU6500_H__


#include "hal_data.h"
#include "i2c2.h"
#include "types.h"

#define MPU_IIC_ADDR 0x68 

#define MPU_SAMPLE_RATE_REG     0X19    //采样频率分频器

#define MPU_CFG_REG             0X1A    //配置寄存器
#define MPU_GYRO_CFG_REG        0X1B    //陀螺仪配置寄存器
#define MPU_ACCEL_CFG_REG       0X1C    //加速度计配置寄存器

#define MPU_FIFO_EN_REG         0X23    //FIFO使能寄存器
#define MPU_I2CMST_CTRL_REG     0X24    //IIC主机控制寄存器

#define MPU_I2CMST_STA_REG      0X36    //IIC主机状态寄存器
#define MPU_INTBP_CFG_REG       0X37    //中断/旁路设置寄存器
#define MPU_INT_EN_REG          0X38    //中断使能寄存器
#define MPU_INT_STA_REG         0X3A    //中断状态寄存器

#define MPU_ACCEL_XOUTH_REG     0X3B    //加速度值,X轴高8位寄存器
#define MPU_ACCEL_XOUTL_REG     0X3C    //加速度值,X轴低8位寄存器
#define MPU_ACCEL_YOUTH_REG     0X3D    //加速度值,Y轴高8位寄存器
#define MPU_ACCEL_YOUTL_REG     0X3E    //加速度值,Y轴低8位寄存器
#define MPU_ACCEL_ZOUTH_REG     0X3F    //加速度值,Z轴高8位寄存器
#define MPU_ACCEL_ZOUTL_REG     0X40    //加速度值,Z轴低8位寄存器
#define MPU_TEMP_OUTH_REG       0X41    //温度值高八位寄存器
#define MPU_TEMP_OUTL_REG       0X42    //温度值低8位寄存器
#define MPU_GYRO_XOUTH_REG      0X43    //陀螺仪值,X轴高8位寄存器
#define MPU_GYRO_XOUTL_REG      0X44    //陀螺仪值,X轴低8位寄存器
#define MPU_GYRO_YOUTH_REG      0X45    //陀螺仪值,Y轴高8位寄存器
#define MPU_GYRO_YOUTL_REG      0X46    //陀螺仪值,Y轴低8位寄存器
#define MPU_GYRO_ZOUTH_REG      0X47    //陀螺仪值,Z轴高8位寄存器
#define MPU_GYRO_ZOUTL_REG      0X48    //陀螺仪值,Z轴低8位寄存器

#define MPU_PWR_MGMT1_REG       0X6B    //电源管理寄存器1
#define MPU_PWR_MGMT2_REG       0X6C    //电源管理寄存器2 
#define MPU_FIFO_CNTH_REG       0X72    //FIFO计数寄存器高八位
#define MPU_FIFO_CNTL_REG       0X73    //FIFO计数寄存器低八位
#define MPU_FIFO_RW_REG         0X74    //FIFO读写寄存器
#define MPU_DEVICE_ID_REG       0X75    //器件ID寄存器


void MPU6500_Init(void);

void MPU6500_Get_Gyro(Gyro_Struct *gyro);

void MPU6500_Get_Acce(Acc_Struct *acc);

void MPU6500_Read(Gyro_Acc_Struct *gyro_acc);



#endif

