#include "spi1.h"
#include <stdio.h>

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define LCD_DC_PIN              BSP_IO_PORT_01_PIN_04
#define LCD_RESET_PIN           BSP_IO_PORT_01_PIN_05
#define LCD_PWM_PIN             BSP_IO_PORT_06_PIN_08

#define SPI_SEND_DATA           BSP_IO_LEVEL_HIGH
#define SPI_SEND_CMD            BSP_IO_LEVEL_LOW

/* ST7796S部分寄存器定义 */
#define LCD_DISPLAY_CMD_RAMCTRL           0xb0 // RAM Control
#define LCD_DISPLAY_CMD_CASET             0x2a // Column address set
#define LCD_DISPLAY_CMD_RASET             0x2b // Row address set
#define LCD_DISPLAY_CMD_RAMWR             0x2c // Memory write

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void spi1_wait_for_tx(void);
static void spi_display_init(void);

static fsp_err_t spi_send_data_cmd(uint8_t * uc_data, bsp_io_level_t uc_cmd, uint32_t len);
static fsp_err_t spi_display_backlight_opt(bsp_io_level_t opt);
static fsp_err_t spi_display_reset(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
/* Event flags for master */
static volatile spi_event_t g_master_event_flag;    // Master Transfer Event completion flag

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
/**
 * @brief 初始化SPI显示
 * @return 初始化成功返回true，失败返回false
 */
fsp_err_t drv_spi_display_init(void)
{
    fsp_err_t err;

    /* 初始化I2C驱动 */
    err = g_spi1.p_api->open(&g_spi1_ctrl, &g_spi1_cfg);
    if (FSP_SUCCESS != err)
    {
        printf ("%s %d\r\n", __FUNCTION__, __LINE__);
        return err;
    }

    spi_display_init();

    return err;
}

/**
 * @brief 设置SPI显示窗口
 * @param x1 窗口左上角X坐标
 * @param y1 窗口左上角Y坐标
 * @param x2 窗口右下角X坐标
 * @param y2 窗口右下角Y坐标
 * @return 无
 */
void spi_display_set_window(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t caset[4];
    uint8_t raset[4];

    caset[0] = (uint8_t)(x1 >> 8) & 0xFF;
    caset[1] = (uint8_t)(x1 & 0xff);
    caset[2] = (uint8_t)(x2 >> 8) & 0xFF;
    caset[3] = (uint8_t)(x2 & 0xff) ;

    raset[0] = (uint8_t)(y1 >> 8) & 0xFF;
    raset[1] = (uint8_t)(y1 & 0xff);
    raset[2] = (uint8_t)(y2 >> 8) & 0xFF;
    raset[3] = (uint8_t)(y2 & 0xff);

    spi_send_data_cmd((uint8_t []){LCD_DISPLAY_CMD_CASET}, SPI_SEND_CMD, 1); // Horiz
    spi_send_data_cmd(caset, SPI_SEND_DATA, 4);
    spi_send_data_cmd((uint8_t []){LCD_DISPLAY_CMD_RASET}, SPI_SEND_CMD, 1); // Vert
    spi_send_data_cmd(raset, SPI_SEND_DATA, 4);
    spi_send_data_cmd((uint8_t []){LCD_DISPLAY_CMD_RAMWR}, SPI_SEND_CMD, 1); // Memory write
}

/**
 * @brief 刷新SPI显示数据
 * @param data 数据指针
 * @param len 数据长度
 * @return 刷新成功返回true，失败返回false
 */
fsp_err_t drv_spi_display_flush_data(uint8_t * data, uint32_t len)
{
    fsp_err_t err;

    err = spi_send_data_cmd(data, SPI_SEND_DATA, len);
    if (FSP_SUCCESS != err)
    {
        printf ("%s %d\r\n", __FUNCTION__, __LINE__);
        return err;
    }

    return err;
}

/**
 * @brief SPI回调函数
 * @param p_args 回调参数
 * @return 无
 */
void spi1_callback(spi_callback_args_t *p_args)
{
    /* 判断是否是发送完成触发的中断 */
    /* 如果是的话就将发送完成标志位置1 */
    if (SPI_EVENT_TRANSFER_COMPLETE == p_args->event)
    {
        g_master_event_flag = SPI_EVENT_TRANSFER_COMPLETE;
    }
    else
    {
        g_master_event_flag = SPI_EVENT_TRANSFER_ABORTED;
    }
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
/**
 * @brief 等待SPI发送完成
 * @return 无
 */
 static void spi1_wait_for_tx(void)
{
    while(!g_master_event_flag);
    g_master_event_flag = false;
}

/**
 * @brief 初始化SPI显示
 * @return 初始化成功返回true，失败返回false
 */
static void spi_display_init(void)
{
    spi_display_reset();
    spi_display_backlight_opt(BSP_IO_LEVEL_HIGH); // backlight on

#if 1
    spi_send_data_cmd((uint8_t []){0x11}, SPI_SEND_CMD, 1);     // Sleep out
    spi_send_data_cmd((uint8_t []){0x20}, SPI_SEND_CMD, 1);     // 关闭显示反转
    spi_send_data_cmd((uint8_t []){0x36}, SPI_SEND_CMD, 1);     // 内存数据访问控制设置
    spi_send_data_cmd((uint8_t []){0x48}, SPI_SEND_DATA, 1);    // 显示方向：左->右，上->下(不旋转); BGR

    spi_send_data_cmd((uint8_t []){0x3a}, SPI_SEND_CMD, 1);     // 设置像素格式(bpp)
    spi_send_data_cmd((uint8_t []){0x55}, SPI_SEND_DATA, 1);    // RGB接口颜色格式：16bit/pixel；控制接口的颜色格式：16bit/pixel

    spi_send_data_cmd((uint8_t []){0x13}, SPI_SEND_CMD, 1);     // 普通显示模式
    spi_send_data_cmd((uint8_t []){0x29}, SPI_SEND_CMD, 1);     // 开启显示
#elif
    spi_send_data_cmd((uint8_t []){0x11}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x00}, SPI_SEND_DATA, 1);
    R_BSP_SoftwareDelay(120, BSP_DELAY_UNITS_MILLISECONDS);     //延时120ms

    spi_send_data_cmd((uint8_t []){0xf0}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0xc3}, SPI_SEND_DATA, 1);
    spi_send_data_cmd((uint8_t []){0xf0}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x96}, SPI_SEND_DATA, 1);
    spi_send_data_cmd((uint8_t []){0x36}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x48}, SPI_SEND_DATA, 1);    // RGB
    spi_send_data_cmd((uint8_t []){0xb4}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x01}, SPI_SEND_DATA, 1);
    spi_send_data_cmd((uint8_t []){0xb7}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0xc6}, SPI_SEND_DATA, 1);

    spi_send_data_cmd((uint8_t []){0xe8}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x40, 0x8A, 0x00, 0x00, 0x29, 0x19, 0xA5, 0x33}, SPI_SEND_DATA, 8);

    spi_send_data_cmd((uint8_t []){0xc1}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x06}, SPI_SEND_DATA, 1);
    spi_send_data_cmd((uint8_t []){0xc2}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0xa7}, SPI_SEND_DATA, 1);
    spi_send_data_cmd((uint8_t []){0xc5}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x18}, SPI_SEND_DATA, 1);

    spi_send_data_cmd((uint8_t []){0xe0}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0xF0, 0x09, 0x0B, 0x06, 0x04, 0x15, 0x2F, 0x54, 0x42, 0x3C, 0x17, 0x14, 0x18, 0x1B}, SPI_SEND_DATA, 14);

    spi_send_data_cmd((uint8_t []){0xe1}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0xF0, 0x09, 0x0B, 0x06, 0x04, 0x03, 0x2D, 0x43, 0x42, 0x3B, 0x16, 0x14, 0x17, 0x1B}, SPI_SEND_DATA, 14);

    spi_send_data_cmd((uint8_t []){0xf0}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x3c}, SPI_SEND_DATA, 1);
    spi_send_data_cmd((uint8_t []){0xf0}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x69}, SPI_SEND_DATA, 1);
    spi_send_data_cmd((uint8_t []){0x3a}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x55}, SPI_SEND_DATA, 1);
    R_BSP_SoftwareDelay(120, BSP_DELAY_UNITS_MILLISECONDS);     //延时120ms

    spi_send_data_cmd((uint8_t []){0x29}, SPI_SEND_CMD, 1);

    /*rotation*/
    spi_send_data_cmd((uint8_t []){0x36}, SPI_SEND_CMD, 1);
    spi_send_data_cmd((uint8_t []){0x48}, SPI_SEND_DATA, 1);    // 0
#endif

}

/**
 * @brief 发送SPI数据和命令
 * @param uc_data 数据指针
 * @param uc_cmd 命令
 * @param len 数据长度
 * @return 发送成功返回true，失败返回false
 */
static fsp_err_t spi_send_data_cmd(uint8_t * uc_data, bsp_io_level_t uc_cmd, uint32_t len)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    /* Master send data to device */
    err = g_ioport.p_api->pinWrite(g_ioport.p_ctrl, LCD_DC_PIN, uc_cmd);
    if(FSP_SUCCESS != err)
    {
        printf ("%s %d\r\n", __FUNCTION__, __LINE__);
        return err;
    }

    err = g_spi1.p_api->write(g_spi1.p_ctrl, uc_data, len, SPI_BIT_WIDTH_8_BITS);
    if(FSP_SUCCESS != err)
    {
        printf ("%s %d\r\n", __FUNCTION__, __LINE__);
        return err;
    }

    spi1_wait_for_tx();

    return err;
}

/**
 * @brief 设置SPI显示背光
 * @param opt 背光选项
 * @return 设置成功返回true，失败返回false
 */
static fsp_err_t spi_display_backlight_opt(bsp_io_level_t opt)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    g_ioport.p_api->pinWrite((ioport_ctrl_t * const )&g_ioport.p_ctrl, LCD_PWM_PIN, opt);
    return err;
}

/**
 * @brief 初始化SPI显示
 * @return 初始化成功返回true，失败返回false
 */
static fsp_err_t spi_display_reset(void)
{
    fsp_err_t err = FSP_SUCCESS;     // Error status

    g_ioport.p_api->pinWrite((ioport_ctrl_t * const )&g_ioport.p_ctrl, LCD_RESET_PIN, BSP_IO_LEVEL_LOW);
    R_BSP_SoftwareDelay(120, BSP_DELAY_UNITS_MILLISECONDS); //延时120ms
    g_ioport.p_api->pinWrite((ioport_ctrl_t * const )&g_ioport.p_ctrl, LCD_RESET_PIN, BSP_IO_LEVEL_HIGH);
    R_BSP_SoftwareDelay(120, BSP_DELAY_UNITS_MILLISECONDS); //延时120ms

    return err;
}

/**
 * @brief 在SPI显示上显示指定颜色
 * @param color_le 要显示的颜色（小端序）
 */
void spi_display_show_color(uint16_t color_le)
{
    uint8_t color_be[2];
    color_be [0] = (uint8_t)((color_le & 0xff00) >> 8);
    color_be [1] = (uint8_t)(color_le & 0xff);

    spi_display_set_window(0, 0, LCD_SCREEN_WIDTH, LCD_SCREEN_HEIGHT);

    for(uint16_t x = 0; x < LCD_SCREEN_WIDTH; x++)
        for(uint16_t y = 0; y < LCD_SCREEN_HEIGHT; y++)
            drv_spi_display_flush_data(color_be, 2);
}


// 在 spi1.c 中实现字库和绘制函数

/**********************************************************************************************************************
 * 8x8 点阵字体定义 (仅包含常用大写字母、数字和空格，可自行扩充)
 * 每个字符8字节，字节高位对应像素左端
 **********************************************************************************************************************/
static const uint8_t font8x8[128][8] = {
    [32] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, // 空格


    ['0'] = {0x3E,0x41,0x41,0x41,0x41,0x41,0x41,0x3E}, // 0
    ['1'] = {0x10,0x30,0x10,0x10,0x10,0x10,0x10,0x30}, // 1 
    ['2'] = {0x7E,0x81,0x02,0x04,0x08,0x10,0x20,0x7F}, // 2
    ['3'] = {0x7E,0x81,0x02,0x0E,0x02,0x02,0x81,0x7E}, // 3
    ['4'] = {0x04,0x0C,0x14,0x24,0x44,0xFF,0x04,0x04}, // 4
    ['5'] = {0x7F,0x80,0x80,0x7E,0x02,0x02,0x83,0x7E}, // 5 
    ['6'] = {0x7C,0x82,0x80,0x7E,0x82,0x82,0x82,0x7E}, // 6 
    ['7'] = {0xFF,0x01,0x02,0x04,0x08,0x10,0x20,0x40}, // 7 
    ['8'] = {0x7E,0x81,0x81,0x7E,0x81,0x81,0x81,0x7E}, // 8
    ['9'] = {0x7E,0x82,0x82,0x7E,0x02,0x04,0x08,0x1C}, // 9


    ['A'] = {0x0C,0x12,0x21,0x21,0x3F,0x21,0x21,0x21}, // A
    ['B'] = {0x3F,0x21,0x21,0x3F,0x21,0x21,0x21,0x3F}, // B
    ['C'] = {0x1E,0x21,0x40,0x40,0x40,0x40,0x21,0x1E}, // C
    ['D'] = {0x3F,0x21,0x21,0x21,0x21,0x21,0x21,0x3F}, // D
    ['E'] = {0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x7F}, // E
    ['F'] = {0x7F,0x40,0x40,0x7F,0x40,0x40,0x40,0x40}, // F
    ['G'] = {0x1E,0x21,0x40,0x40,0x4F,0x21,0x21,0x1E}, // G
    ['H'] = {0x21,0x21,0x21,0x3F,0x21,0x21,0x21,0x21}, // H
    ['I'] = {0x1E,0x04,0x04,0x04,0x04,0x04,0x04,0x1E}, // I
    ['J'] = {0x3C,0x04,0x04,0x04,0x04,0x04,0x04,0x07}, // J
    ['K'] = {0x21,0x22,0x24,0x38,0x24,0x22,0x21,0x20}, // K
    ['L'] = {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7F}, // L
    ['M'] = {0x20,0x30,0x28,0x24,0x22,0x21,0x21,0x21}, // M
    ['N'] = {0x21,0x31,0x29,0x25,0x23,0x21,0x21,0x21}, // N
    ['O'] = {0x1E,0x21,0x41,0x41,0x41,0x41,0x21,0x1E}, // O
    ['P'] = {0x3F,0x21,0x21,0x3F,0x01,0x01,0x01,0x01}, // P
    ['Q'] = {0x1E,0x21,0x41,0x41,0x45,0x29,0x21,0x1F}, // Q
    ['R'] = {0x3F,0x21,0x21,0x3F,0x24,0x22,0x21,0x20}, // R
    ['S'] = {0x1E,0x21,0x01,0x1E,0x40,0x40,0x21,0x1E}, // S
    ['T'] = {0x7F,0x04,0x04,0x04,0x04,0x04,0x04,0x04}, // T
    ['U'] = {0x21,0x21,0x21,0x21,0x21,0x21,0x21,0x3F}, // U
    ['V'] = {0x21,0x21,0x21,0x21,0x21,0x12,0x0C,0x04}, // V
    ['W'] = {0x21,0x21,0x21,0x25,0x25,0x29,0x31,0x21}, // W
    ['X'] = {0x21,0x21,0x12,0x0C,0x0C,0x12,0x21,0x21}, // X
    ['Y'] = {0x21,0x21,0x12,0x0C,0x04,0x04,0x04,0x04}, // Y
    ['Z'] = {0x7F,0x40,0x20,0x10,0x08,0x04,0x02,0x7F}, // Z

     // 小写字母字库
    ['a'] = {0x00,0x00,0x1E,0x20,0x3E,0x21,0x3E,0x00}, // a
    ['b'] = {0x20,0x20,0x3E,0x21,0x21,0x21,0x3E,0x00}, // b
    ['c'] = {0x00,0x00,0x1E,0x20,0x20,0x20,0x1E,0x00}, // c
    ['d'] = {0x01,0x01,0x1F,0x21,0x21,0x21,0x1F,0x00}, // d
    ['e'] = {0x00,0x00,0x1E,0x21,0x3F,0x20,0x1E,0x00}, // e
    ['f'] = {0x0E,0x10,0x3E,0x10,0x10,0x10,0x10,0x00}, // f
    ['g'] = {0x00,0x00,0x1F,0x21,0x21,0x1F,0x01,0x1E}, // g
    ['h'] = {0x20,0x20,0x3E,0x21,0x21,0x21,0x21,0x00}, // h
    ['i'] = {0x08,0x00,0x18,0x08,0x08,0x08,0x1C,0x00}, // i
    ['j'] = {0x04,0x00,0x0C,0x04,0x04,0x04,0x24,0x18}, // j
    ['k'] = {0x20,0x20,0x22,0x24,0x38,0x24,0x22,0x00}, // k
    ['l'] = {0x18,0x08,0x08,0x08,0x08,0x08,0x1C,0x00}, // l
    ['m'] = {0x00,0x00,0x36,0x49,0x49,0x49,0x49,0x00}, // m
    ['n'] = {0x00,0x00,0x3E,0x21,0x21,0x21,0x21,0x00}, // n
    ['o'] = {0x00,0x00,0x1E,0x21,0x21,0x21,0x1E,0x00}, // o
    ['p'] = {0x00,0x00,0x3E,0x21,0x21,0x3E,0x20,0x20}, // p
    ['q'] = {0x00,0x00,0x1F,0x21,0x21,0x1F,0x01,0x01}, // q
    ['r'] = {0x00,0x00,0x2E,0x30,0x20,0x20,0x20,0x00}, // r
    ['s'] = {0x00,0x00,0x1E,0x20,0x1E,0x01,0x3E,0x00}, // s
    ['t'] = {0x10,0x10,0x3E,0x10,0x10,0x10,0x0E,0x00}, // t
    ['u'] = {0x00,0x00,0x21,0x21,0x21,0x21,0x1F,0x00}, // u
    ['v'] = {0x00,0x00,0x21,0x21,0x21,0x12,0x0C,0x00}, // v
    ['w'] = {0x00,0x00,0x49,0x49,0x49,0x49,0x36,0x00}, // w
    ['x'] = {0x00,0x00,0x21,0x12,0x0C,0x12,0x21,0x00}, // x
    ['y'] = {0x00,0x00,0x21,0x21,0x21,0x1F,0x01,0x1E}, // y
    ['z'] = {0x00,0x00,0x3F,0x02,0x0C,0x10,0x3F,0x00}, // z

    ['.'] = {0x00,0x00,0x00,0x00,0x00,0x00,0x18,0x18}, // 小数点
    ['+'] = {0x00,0x08,0x08,0x3E,0x08,0x08,0x00,0x00}, // 加号
    ['-'] = {0x00,0x00,0x00,0x3E,0x00,0x00,0x00,0x00}, // 减号
};

/**
 * @brief 在指定位置显示字符串（每个字符30x30像素）
 * @param x 起始X坐标（左上角）
 * @param y 起始Y坐标（左上角）
 * @param str 要显示的字符串（仅支持字体中定义的字符）
 * @param fg_color 前景色（RGB565）
 * @param bg_color 背景色（RGB565）
 */
void spi_display_string(uint16_t x, uint16_t y, const char *str, uint16_t fg_color, uint16_t bg_color)
{
    if (NULL == str) return;

    uint8_t src_bitmap[8];                     // 当前字符的8x8点阵
    uint8_t line_buffer[60];                     // 一行30像素的RGB565数据 (2字节/像素)

    while (*str)
    {
        char c = *str++;
        const uint8_t *src = font8x8[(uint8_t)c];

        // 将8字节点阵拷贝到本地，若字符未定义则全0（空白）
        for (int i = 0; i < 8; i++)
            src_bitmap[i] = (src != NULL) ? src[i] : 0x00;

        // 计算当前字符在屏幕上的左上角坐标
        uint16_t char_x = x;
        x += 30;    // 下一个字符的起始X（字间距0）

        // 逐行绘制该字符 (30行)
        for (uint16_t row = 0; row < 30; row++)
        {
            // 计算对应源点阵的行 (0~7)
            uint8_t src_row = (uint8_t)((row * 8.0) / 30.0);

            // 构建当前行的像素数据
            for (uint16_t col = 0; col < 30; col++)
            {
                // 计算对应源点阵的列 (0~7)
                uint8_t src_col = (uint8_t)((col * 8.0) / 30.0);

                // 获取源点阵该位 (高位对应左侧)
                uint8_t bit_mask = 0x80 >> src_col;
                uint16_t color = (src_bitmap[src_row] & bit_mask) ? fg_color : bg_color;

                // 填充line_buffer: 先高8位，后低8位
                line_buffer[col * 2]     = (color >> 8) & 0xFF;
                line_buffer[col * 2 + 1] = color & 0xFF;
            }

            // 设置窗口为当前字符的当前行
            spi_display_set_window(char_x, y + row, char_x + 29, y + row);

            // 发送该行数据
            drv_spi_display_flush_data(line_buffer, 60);
        }
    }
}


