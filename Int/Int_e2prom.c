#include "Int_e2prom.h"
#include "scl_i2c4.h"
#include <stdio.h>
#include <stdlib.h>
#include "uart5.h"
#include "types.h"

/**
 * @brief EEPROM初始化函数
 */
void EEPROMInit(void)
{
    //1.获取EEPROM设备
    I2CDev *ptEepromDev = EEPROMGetDevice();
    //2.检查设备是否存在
    if(NULL == ptEepromDev) return;
    //3.初始化EEPROM设备
    if(0 != ptEepromDev->Init(ptEepromDev)) return;
}

/**
 * @brief EEPROM写入数据函数
 * @param addr EEPROM地址
 * @param wbuf 写入数据缓冲区指针
 * @param size 写入数据字节数
 */
void EEPROMWrite(uint8_t addr, uint8_t wbuf, uint8_t size)
{
	  //1.获取EEPROM设备
    I2CDev *ptEepromDev = EEPROMGetDevice();
	
    ptEepromDev->Write(ptEepromDev, addr, &wbuf, size);     //写入数据
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
}
/**
 * @brief EEPROM读取数据函数
 * @param addr EEPROM地址
 * @param rbuf 读取数据缓冲区指针
 * @param size 读取数据字节数
 * @return uint16_t 读取数据
 */
uint8_t EEPROMRead(uint8_t addr)
{
	  //1.获取EEPROM设备
    I2CDev *ptEepromDev = EEPROMGetDevice();
	
    uint8_t ucRData = 0;

    ptEepromDev->Read(ptEepromDev, addr, &ucRData, 1);     //读取数据   
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);

    return ucRData;
}

/**
 * @brief EEPROM应用测试函数
 * 
 * 该函数用于测试AT24C02 EEPROM的简单读写操作。
 * 它会随机生成写入数据和地址，然后通过I2C接口写入EEPROM，
 * 并通过I2C接口读取EEPROM中的数据进行验证。
 * 
 * @return void
 */
// void EEPROMAppTest(void)
// {
//     //1.获取EEPROM设备
//     I2CDev *ptEepromDev = EEPROMGetDevice();
//     //2.检查设备是否存在
//     if(NULL == ptEepromDev) return;
//     //3.初始化EEPROM设备
//     if(0 != ptEepromDev->Init(ptEepromDev)) return;

//     //4.测试简单的I2C写入和读取一个字节
//     uint8_t ucCount = 10;                                                        //测试次数
//     printf("Start Test Simple I2C Write/Read a Byte From AT24C02\r\n");          //测试简单的I2C写入和读取一个字节
//     printf("\t|  Write  | Read | Result  | Count |\r\n");                        // 写入数据、读取数据、验证结果、测试次数

//     //5.循环测试写入和读取
//     while(ucCount)
//     {
//         uint8_t ucWData = (uint8_t)rand();
//         uint8_t ucRData = 0;
//         uint8_t addr = (uint8_t)rand();
//         ptEepromDev->Write(ptEepromDev, addr, &ucWData, 1);     //写入数据
//         R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);
//         ptEepromDev->Read(ptEepromDev, addr, &ucRData, 1);     //读取数据   
//         R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MILLISECONDS);

//         printf("\t|   %.3d   | %.3d  |", ucWData, ucRData);
//         if(ucWData == ucRData)
//             printf(" %s |   %.2d  |\r\n", "Success", ucCount);
//         else
//             printf(" %s |  %.3d  |\r\n", "Error", ucCount);
//         ucCount--;
//     }

//     printf("Start Test Simple I2C Write/Read n Bytes From AT24C02\r\n");
//     printf("\t| Address | Size | Result  | Count |\r\n");
//     ucCount = 5;
//     uint8_t wbuf[256];
//     uint8_t rbuf[256];

//     //6.测试简单的I2C写入和读取n个字节
//     while(ucCount)
//     {
//         uint8_t ucAddr = (uint8_t)rand();
//         uint8_t size = (uint8_t)rand();
//         for(uint16_t i=0; i<256; i++)
//         {
//             wbuf[i] = (uint8_t)i;
//         }
//         size = (uint8_t)(((ucAddr+size)>256)?(256-ucAddr):size);
//         printf("\t|   %.3d   | %.3d  |", ucAddr, size);

//         ptEepromDev->Write(ptEepromDev, ucAddr, wbuf, size);
//         R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
//         ptEepromDev->Read(ptEepromDev, ucAddr, rbuf, size);

//         uint16_t err_c = 0;
//         for(uint16_t i=0; i<size; i++)
//         {
//             if(rbuf[i] != wbuf[i])
//             {
//                 err_c++;
//             }
//         }
//         if(0 == err_c)
//             printf(" %s |   %.2d  |\r\n", "Success", ucCount);
//         else
//             printf(" %s |  %.3d  |\r\n", "Error", err_c);
//         ucCount--;
//         R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MILLISECONDS);
//     }
// }









