#include "gpt_pwm.h"
#include "uart5.h"
#include "types.h"

uint32_t period_counts6 = 100000; 
uint32_t period_counts0 = 100000; 


/**
 * @brief 初始化GPT模块
 */
void GPT_PWM_Init(void)
{
    //1. 初始化 GPT 模块 
    R_GPT_Open(&g_timer6_ctrl, &g_timer6_cfg);
    R_GPT_Open(&g_timer0_ctrl, &g_timer0_cfg);

    //2. 启动 GPT 定时器 
    R_GPT_Start(&g_timer6_ctrl);
    R_GPT_Start(&g_timer0_ctrl);

	//3. 设置GPT输出频率为1000Hz
	GPT_PWM6_SetFreq(20000);
    GPT_PWM0_SetFreq(5000);

    //3. 设置GPT占空比为50%
    GPT_PWM6_SetDuty(50);
    GPT_PWM0_SetDuty(50);
}

/**
 * @brief 设置GPT6输出频率
 * @param freq 频率，单位：Hz
 */
void GPT_PWM6_SetFreq(uint32_t freq)
{		
    //1.定义变量               
    uint32_t CLK_freq;
	
    // 2. 频率范围限制
    if (freq > 20000) freq = 20000;  // 最高频率限制
    else if (freq < 500) freq = 500;      // 最低频率限制

    //3.获取PCLKD的时钟频率
    CLK_freq = 100000000;          // PCLKD的时钟频率为100MHz

    //4. 计算周期计数值
    period_counts6 = CLK_freq / freq;

    //5. 设置GPT输出频率
    R_GPT_PeriodSet(&g_timer6_ctrl, period_counts6);
}

/**
 * @brief 设置GPT0输出频率
 * @param freq 频率，单位：Hz
 */
void GPT_PWM0_SetFreq(uint32_t freq)
{		
    //1.定义变量               
    uint32_t CLK_freq;
	
    // 2. 频率范围限制
    if (freq > 20000) freq = 20000;  // 最高频率限制
    else if (freq < 500) freq = 500;      // 最低频率限制

    //3.获取PCLKD的时钟频率
    CLK_freq = 100000000;          // PCLKD的时钟频率为100MHz

    //4. 计算周期计数值
    period_counts0 = CLK_freq / freq;

    //5. 设置GPT输出频率
    R_GPT_PeriodSet(&g_timer0_ctrl, period_counts0);
}


/**
 * @brief 设置GPT6占空比
 * @param duty 占空比，范围0~100
 */
void GPT_PWM6_SetDuty(uint8_t duty)
{
    timer_info_t info;                // 定义一个结构体变量info，用于存储GPT的信息
    uint32_t current_period_counts;   // 定义一个变量current_period_counts，用于存储GPT一个周期需要的计数次数
    uint32_t duty_cycle_counts;       // 定义一个变量duty_cycle_counts，用于存储GPT占空比对应的计数次数

    if (duty > 100)
        duty = 100; //限制占空比范围：0~100

    /* 获得GPT的信息 */
    R_GPT_InfoGet(&g_timer6_ctrl, &info);

    /* 获得计时器一个周期需要的计数次数 */
    current_period_counts = period_counts6;

    /* 根据占空比和一个周期的计数次数计算GTCCR寄存器的值 */
    duty_cycle_counts = (uint32_t)(((uint64_t) current_period_counts * duty) / 100);

    /* 最后调用FSP库函数设置占空比 */
    R_GPT_DutyCycleSet(&g_timer6_ctrl, duty_cycle_counts, GPT_IO_PIN_GTIOCA);
}

/**
 * @brief 设置GPT0占空比
 * @param duty 占空比，范围0~100
 */
void GPT_PWM0_SetDuty(uint8_t duty)
{
    timer_info_t info;                // 定义一个结构体变量info，用于存储GPT的信息
    uint32_t current_period_counts;   // 定义一个变量current_period_counts，用于存储GPT一个周期需要的计数次数
    uint32_t duty_cycle_counts;       // 定义一个变量duty_cycle_counts，用于存储GPT占空比对应的计数次数

    if (duty > 100)
        duty = 100; //限制占空比范围：0~100

    /* 获得GPT的信息 */
    R_GPT_InfoGet(&g_timer0_ctrl, &info);

    /* 获得计时器一个周期需要的计数次数 */
    current_period_counts = period_counts0;

    /* 根据占空比和一个周期的计数次数计算GTCCR寄存器的值 */
    duty_cycle_counts = (uint32_t)(((uint64_t) current_period_counts * duty) / 100);

    /* 最后调用FSP库函数设置占空比 */
    R_GPT_DutyCycleSet(&g_timer0_ctrl, duty_cycle_counts, GPT_IO_PIN_GTIOCA);
}


