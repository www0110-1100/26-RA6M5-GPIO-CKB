#include "display_Task.h"
#include "APP.h"
#include "scl_i2c4.h"
#include <stdio.h>
#include <stdlib.h>
#include "debug_uart7.h"
#include "imu_Task.h"
#include "stepper_Task.h"
#include "dm43x0_Task.h"
#include "spi1.h"
#include "uart3.h"
#include "stepper_Task.h"

// ????
char str_buffer[32];

uint8_t display_tt = 0;

/*   ?????????1???   */
void App_Display_Task(void)
{
    ++display_tt;
    if(display_tt == 1){
    // ??YAW
    snprintf(str_buffer, sizeof(str_buffer), "YAW %.2f", EulerAngle.yaw);
    spi_display_string(10, 10, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); }
    else if(display_tt == 2){
    switch (car_status)
    {
        case CAR_STOP:          // ????
        {
            spi_display_string(10, 50, "STOP      ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
        }
        break;
        case CAR_PID:          // PID??
        {
            // ??PID
            snprintf(str_buffer, sizeof(str_buffer), "PID %.2f", line_angle);
            spi_display_string(10, 50, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); 
        }
        break;
        case CAR_TURN:          // ????
        {
            spi_display_string(10, 50, "TURE      ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
        }
        break;
        default:break;
    }}
    else if(display_tt == 3){
    // ??X
    snprintf(str_buffer, sizeof(str_buffer), "X %.2f", x_drug);
    spi_display_string(10, 90, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); }
    else if(display_tt == 4){
    // ??Y
    snprintf(str_buffer, sizeof(str_buffer), "Y %.2f", y_drug);
    spi_display_string(10, 130, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); }
    else if(display_tt == 5){
    // ??C
    snprintf(str_buffer, sizeof(str_buffer), "C %.2f", claw_distance);
    spi_display_string(10, 170, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); }
    else if(display_tt == 6){
    switch (arm_status)          // ??ARM??
    {
        // ZERO??
        case ARM_ZERO:
        {
            spi_display_string(10, 210, "ZERO      ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
        }
        break;
        // PRESET??
        case ARM_PRESET:
        {
            spi_display_string(10, 210, "PRESET    ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
        }
        break;
        // ADJUST??
        case ARM_ADJUST:
        {
            spi_display_string(10, 210, "ADJUST    ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
        }
        break;
        // RELEASE??
        case ARM_RELEASE:
        {
            spi_display_string(10, 210, "RELEASE   ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
        }
        break;
        default:break;
    }}
    else if(display_tt == 7){
    if(display_flag == 1)
    {
        display_flag = 0;
        spi_display_string(10, 240, display_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE);
    }}
    else if(display_tt == 8){
    snprintf(str_buffer, sizeof(str_buffer), "%.d   %.d  ", speed1, speed2);
    spi_display_string(10, 280, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); }
    else if(display_tt == 9){
    display_tt = 0;
    snprintf(str_buffer, sizeof(str_buffer), "%.d   %.d  ", speed3, speed4);
    spi_display_string(10, 320, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); }
}


//    // ??YAW
//     snprintf(str_buffer, sizeof(str_buffer), "YAW %.2f", EulerAngle.yaw);
//     spi_display_string(10, 10, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); 
//     switch (car_status)
//     {
//         case CAR_STOP:          // ????
//         {
//             spi_display_string(10, 50, "STOP      ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
//         }
//         break;
//         case CAR_PID:          // PID??
//         {
//             // ??PID
//             snprintf(str_buffer, sizeof(str_buffer), "PID %.2f", line_angle);
//             spi_display_string(10, 50, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); 
//         }
//         break;
//         case CAR_TURN:          // ????
//         {
//             spi_display_string(10, 50, "TURE      ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
//         }
//         break;
//         default:break;
//     }
//         // ??X
//         snprintf(str_buffer, sizeof(str_buffer), "X %.2f", x_drug);
//         spi_display_string(10, 90, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); 
//         // ??Y
//         snprintf(str_buffer, sizeof(str_buffer), "Y %.2f", y_drug);
//         spi_display_string(10, 130, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); 
//         // ??C
//         snprintf(str_buffer, sizeof(str_buffer), "C %.2f", claw_pitch);
//         spi_display_string(10, 170, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); 
//         // ??ARM??
//     switch (arm_status)
//     {
//         // ZERO??
//         case ARM_ZERO:
//         {
//             spi_display_string(10, 210, "ZERO      ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
//         }
//         break;
//         // PRESET??
//         case ARM_PRESET:
//         {
//             spi_display_string(10, 210, "PRESET    ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
//         }
//         break;
//         // ADJUST??
//         case ARM_ADJUST:
//         {
//             spi_display_string(10, 210, "ADJUST    ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
//         }
//         break;
//         // RELEASE??
//         case ARM_RELEASE:
//         {
//             spi_display_string(10, 210, "RELEASE   ", LCD_COLOR_GRAY, LCD_COLOR_WHITE);
//         }
//         break;
//         default:break;
//     }
//     if(display_flag == 1)
//     {
//         display_flag = 0;
//         spi_display_string(10, 240, display_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE);
//     }

//     //???speed1,speed2,speed3,speed4
//     snprintf(str_buffer, sizeof(str_buffer), "%.d   %.d  ", speed1, speed2);
//     spi_display_string(10, 280, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); 
//     snprintf(str_buffer, sizeof(str_buffer), "%.d   %.d  ", speed3, speed4);
//     spi_display_string(10, 320, str_buffer, LCD_COLOR_GRAY, LCD_COLOR_WHITE); 







