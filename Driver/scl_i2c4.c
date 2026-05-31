#include "scl_i2c4.h"
#include <stdio.h>
#include "types.h"

/*****************************************************条件创造区******************************************************************/
 // I2C4等待发送完成
static void I2C4WaitTxCplt(void);
// I2C4等待接收完成
static void I2C4WaitRxCplt(void);
 
 // EEPROM驱动函数声明
static int EEPROMDrvInit(struct I2CDev* ptDev);
static int EEPROMDrvWriteBuff(struct I2CDev *ptDev, unsigned char ucAddr, unsigned char const *wBuf, unsigned int dwSize);
static int EEPROMDrvRead(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char* rbuf, unsigned int dwSize);
 
static int  EEPROMDrvWriteByte(struct I2CDev* ptDev, \
                               unsigned char ucAddr, \
                               unsigned char const ucData);
static int  EEPROMDrvWritePage(struct I2CDev* ptDev, \
                               unsigned char ucAddr, \
                               unsigned char const* wbuf, \
                               unsigned int dwSize);
															 
															 
 
 //创建I2C4 EEPROM设备
 static I2CDev gEepromDev = {
                              .name  = "EEPROM",
                              .Init  = EEPROMDrvInit,
                              .Write = EEPROMDrvWriteBuff,
                              .Read  = EEPROMDrvRead
 };

 //获取I2C4 EEPROM设备指针
 struct I2CDev* EEPROMGetDevice(void)
 {
     return &gEepromDev;
 }

 
static volatile bool gI2CTxCplt = false;   //发送完成标志位
static volatile bool gI2CRxCplt = false;   //接收完成标志位
extern void sci_i2c4_master_callback(i2c_master_callback_args_t * p_args);

 /*****************************************************函数编写区******************************************************************/
 /**
  * @brief I2C4主模式回调函数
  * @param p_args 回调参数指针
  */
 void sci_i2c_master_callback(i2c_master_callback_args_t * p_args)
 {
     //1.发送完成置位
     if(I2C_MASTER_EVENT_TX_COMPLETE == p_args->event)
     {
         gI2CTxCplt = true;
     }
     //2.接收完成置位
     else if(I2C_MASTER_EVENT_RX_COMPLETE == p_args->event)
     {
         gI2CRxCplt = true;
     }
 }

  /**
  * @brief 等待I2C4发送完成或者超时
  */
 static void I2C4WaitTxCplt(void)
 {
     //1.定义等待时间
     uint32_t timeout = 50;

     //2.等待发送完成或者超时
     while(!gI2CTxCplt && timeout)
     {
         timeout--;
         R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
     }

     //3.复位发送完成标志位
     gI2CTxCplt = false;
 }

 /**
  * @brief 等待I2C4接收完成或者超时
  */
 static void I2C4WaitRxCplt(void)
 {
     //1.定义等待时间
     uint32_t timeout = 50;

     //2.等待接收完成或者超时
     while(!gI2CRxCplt && timeout)
     {
         timeout--;
         R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);
     }

     //3.复位接收完成标志位
     gI2CRxCplt = false;
 }
 
 /**
  * @brief 初始化I2C4 EEPROM设备
  * @param ptDev I2C4 EEPROM设备指针
  * @return int 初始化状态码
  */
 static int EEPROMDrvInit(struct I2CDev* ptDev)
 {
     //1.检查设备名称是否存在
     if(NULL == ptDev->name)    return -1;

     //2.打开I2C4设备
     fsp_err_t err = g_i2c4.p_api->open(g_i2c4.p_ctrl, g_i2c4.p_cfg);

     //3.检查打开状态
     if(FSP_SUCCESS == err)
     {
         printf("Success to open device: i2c4!\r\n");
         return 0;
     }
     else
     {
         printf("Failed to open device: i2c4!\r\n");
     }
     return -1;
 }

 /**
  * @brief 向I2C4 EEPROM写入一字节数据
  * @param ptDev I2C4 EEPROM设备指针
  * @param ucAddr EEPROM的目标地址
  * @param ucData 写入数据
  * @return int 写入状态码
  */
 static int EEPROMDrvWriteByte(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char const ucData)
 {
    //1.检查设备名称是否存在
     if(NULL == ptDev->name)    return -1;

    //2.将地址和数据合并为16位数据
    uint16_t wData = (uint16_t)((ucData<<8) | ucAddr);
   
    //3.发送数据
    g_i2c4.p_api->write(g_i2c4.p_ctrl, (unsigned char*)&wData, (unsigned int)2, false);
    I2C4WaitTxCplt();

     return 0;
 }

 /**
  * @brief 向I2C4 EEPROM写入一页数据
  * @param ptDev I2C4 EEPROM设备指针
  * @param ucAddr EEPROM的目标地址
  * @param wbuf 写入数据缓冲区
  * @param dwSize 写入数据大小
  * @return int 写入状态码
  */
 static int EEPROMDrvWritePage(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char const* wbuf, unsigned int dwSize)
{
    //1.检查设备名称是否存在
    if(NULL == ptDev->name)    return -1;

    //2.定义数据缓冲区
    unsigned char data[9] = {0};
    data[0] = ucAddr;

    //3.将数据复制到缓冲区
    for(unsigned char i=0; i<dwSize; i++)
    {
        data[i+1] = wbuf[i];
    }

    //4.发送数据
    g_i2c4.p_api->write(g_i2c4.p_ctrl, (unsigned char*)data, (unsigned int)(dwSize+1), false);
    I2C4WaitTxCplt();
    R_BSP_SoftwareDelay((dwSize+1)*5, BSP_DELAY_UNITS_MILLISECONDS);
    return 0;
}

/**
 * @brief EEPROM写数据函数
 * @param ptDev I2C设备指针
 * @param ucAddr EEPROM内部地址
 * @param wBuf 要写入的数据缓冲区
 * @param dwSize 数据大小
 * @return 实际写入的字节数
 */
static int EEPROMDrvWriteBuff(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char const* wbuf, unsigned int dwSize)
{
    //1.检查设备名称是否存在
    if(NULL == ptDev->name)    return -1;

    //2.检查数据大小是否为1字节
    if(dwSize == 1)
        return EEPROMDrvWriteByte(ptDev, ucAddr, *wbuf);

    //3.检查地址是否超出范围
    if((ucAddr+dwSize) >= 256)      return -1;

    //4.定义当前地址变量
    unsigned char nAddr = ucAddr;

    //5.检查是否需要写满当前页
    /* 不管从何处开始，都将从起始地址开始把所在页写满 */
    /* 或者不会写满的情况下，从起始位置开始写size个字节 */
    unsigned char ucSize;  /* 当前写入的数据个数 */
    if( (ucAddr==0 || (ucAddr/EE_PAGE_SIZE)!=0) && (dwSize<=EE_PAGE_SIZE))
        ucSize = (unsigned char)dwSize;
    else
        ucSize = EE_PAGE_SIZE - (ucAddr % EE_PAGE_SIZE);
    EEPROMDrvWritePage(ptDev, nAddr, wbuf, ucSize);

    //6.写满起始位置开始的那一页之后，要将位置、数据地址和个数进行偏移计算
    nAddr += ucSize;
    wbuf += ucSize;
    dwSize -= ucSize;

    //7.如果写满起始地址所在页后还有数据，就进行下一步处理
    while(dwSize != 0)
    {
        /* 如果剩余数据个数大于一页的个数，就写满下一页 */
        /* 否则将剩余数据全部写到下一页 */
        if(dwSize <= EE_PAGE_SIZE)
            ucSize = (unsigned char)dwSize;
        else
            ucSize = EE_PAGE_SIZE;

        EEPROMDrvWritePage(ptDev, nAddr, wbuf, ucSize);
        /* 继续偏移 */
        nAddr += ucSize;
        wbuf += ucSize;
        dwSize -= ucSize;
    }

    return 0;
}

 /**
  * @brief 从I2C4 EEPROM读取一字节数据
  * @param ptDev I2C4 EEPROM设备指针
  * @param ucAddr EEPROM的目标地址
  * @param rbuf 读取数据指针
  * @param dwSize 读取数据大小
  * @return int 读取状态码
  */
 static int EEPROMDrvRead(struct I2CDev* ptDev, unsigned char ucAddr, unsigned char* rbuf, unsigned int dwSize)
 {
    //1.检查设备名称是否存在
     if(NULL == ptDev->name)    return -1;

     //2.发送一个字节的地址数据
     g_i2c4.p_api->write(g_i2c4.p_ctrl, (unsigned char*)&ucAddr, (unsigned int)1, true);
     I2C4WaitTxCplt();
     
     //3.读取该地址的一个字节数据
     R_BSP_SoftwareDelay(300, BSP_DELAY_UNITS_MICROSECONDS);
     g_i2c4.p_api->read(g_i2c4.p_ctrl, (unsigned char*)rbuf, (unsigned int)dwSize, false);
     I2C4WaitRxCplt();
     return 0;
 }
