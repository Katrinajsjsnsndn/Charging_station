/*!
 * @file        main.c
 *
 * @brief       Main program body
 *
 * @version     V1.0.2
 *
 * @date        2022-12-31
 *
 * @attention
 *
 *  Copyright (C) 2022-2025 Geehy Semiconductor
 *
 *  You may not use this file except in compliance with the
 *  GEEHY COPYRIGHT NOTICE (GEEHY SOFTWARE PACKAGE LICENSE).
 *
 *  The program is only for reference, which is distributed in the hope
 *  that it will be useful and instructional for customers to develop
 *  their software. Unless required by applicable law or agreed to in
 *  writing, the program is distributed on an "AS IS" BASIS, WITHOUT
 *  ANY WARRANTY OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the GEEHY SOFTWARE PACKAGE LICENSE for the governing permissions
 *  and limitations under the License.
 */

/* Includes */
#include "main.h"
/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "my_init.h"
#include "lcd.h"
#include "test code.h"
#include "charge_control.h"


// 串口接收相关定义
#define UART_RX_BUFFER_SIZE 100
static volatile uint16_t uartRxIndex = 0;
static volatile uint8_t uartDataReceived = 0;

/** @addtogroup Examples
  @{
  */

/** @addtogroup GPIO_Toggle
  @{
  */

/** @defgroup GPIO_Toggle_Functions Functions
  @{
  */



/* Task handle */
static TaskHandle_t xHandleTaskLedToggle = NULL;
static TaskHandle_t xHandleTaskUsartTest = NULL;

/* User create task */
static void UserTaskCreate(void);

/* Led toggle task */
void charging_station_ui_task(void* pvParameters);
/* Usart test task */
void vTaskUsartTest(void* pvParameters);
/*!
 * @brief       Main program
 *
 * @param       None
 *
 * @retval      None
 *
 */
int main(void)
{
    // 系统初始化
    SystemInit();
    
    // GPIO 初始化
    GPIO_INIT();
    APM_MINI_LEDInit(LED2);
    APM_MINI_LEDInit(LED3);
    
    // 串口初始化
    USART_Init();
    IIC_GPIO_Config();



    /* User create task */
    UserTaskCreate();

    /* Startup FreeRTOS */
    vTaskStartScheduler();
    
    // 如果 FreeRTOS 启动失败，进入死循环
    while (1)
    {
        // 错误指示：快速闪烁 LED
        APM_MINI_LEDToggle(LED2);
        APM_MINI_LEDToggle(LED3);
    }
}

/*!
 * @brief       User Create Task
 *
 * @param       None
 *
 * @retval      None
 */
static void UserTaskCreate(void)
{
    xTaskCreate(charging_station_ui_task,
                "charging_station_ui_task",
                256,  // 增加栈空间
                NULL,
                0,
                &xHandleTaskLedToggle);

    xTaskCreate(vTaskUsartTest,
                "TaskUsartTest",
                256,  // 增加栈空间
                NULL,
                2,
                &xHandleTaskUsartTest);
}



/**@} end of group GPIO_Toggle_Functions */
/**@} end of group GPIO_Toggle */
/**@} end of group Examples */


/*!
 * @brief       Led toggle task
 *
 * @param       pvParameters - passed into the task function as the function parameters
 *
 * @retval      None
 */
uint16_t currentCount;
void vTaskUsartTest(void* pvParameters)
{

    
    while (1)
    {
        
        
        
        // 简单的 LED 指示，确认任务在运行
        APM_MINI_LEDToggle(LED3);
        /* Task blocking time Delay */
        vTaskDelay(10);  // 缩短延时，提高轮询频率
    }
}



