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


// 串口接收相关定义
#define UART_RX_BUFFER_SIZE 100
static uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
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
void vTaskLedToggle(void* pvParameters);
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
    
    // 串口初始化完成
    LCD_Init();
    LCD_Display_Dir(3);
    LCD_Clear(WHITE);

			main_test("IC:ST7789");		  //测试主页
			Color_Test();								//纯色测试
			FillRec_Test();							//图形测试
			English_Font_test();				//英文测试
			Chinese_Font_test();				//中文测试
			Pic_test();									//图片测试
			Switch_test();							//显示开关测试
			Rotate_Test();							//旋转测试

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
    xTaskCreate(vTaskLedToggle,
                "TaskLedToggle",
                256,  // 增加栈空间
                NULL,
                0,
                &xHandleTaskLedToggle);

    xTaskCreate(vTaskUsartTest,
                "TaskUsartTest",
                512,  // 增加栈空间
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
void vTaskLedToggle(void* pvParameters)
{
	UNUSED(pvParameters);
    while (1)
    {
        /* Toggle LED2 */
        APM_MINI_LEDToggle(LED2);
        /* Task blocking time Delay */
        vTaskDelay(500);
    }
}

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
    uint8_t testData[] = "Hello UART!\r\n";
    uint16_t dataLength = sizeof(testData) - 1;  // 不包括字符串结束符
    
    while (1)
    {
        // 使用轮询方式检查接收数据
        if (USART_ReadStatusFlag(MINI_COM2, USART_FLAG_RXBNE))
        {
            // 读取接收到的数据
            uint8_t receivedByte = USART_RxData(MINI_COM2);
            
            // 存储到接收缓冲区
            if (uartRxIndex < UART_RX_BUFFER_SIZE - 1)
            {
                uartRxBuffer[uartRxIndex++] = receivedByte;
                
                // 如果接收到换行符，表示一帧数据接收完成
                if (receivedByte == '\n' || receivedByte == '\r')
                {
                    // 数据处理完成标志
                    uartDataReceived = 1;
                    
                    // 可以在这里添加你的数据处理逻辑
                    // 例如：解析命令、存储数据等
                    
                    // 重置接收索引
                    uartRxIndex = 0;
                }
            }
            else
            {
                // 缓冲区满，重置索引
                uartRxIndex = 0;
            }
        }
        
        // 简单的 LED 指示，确认任务在运行
        APM_MINI_LEDToggle(LED3);
        
        /* Task blocking time Delay */
        vTaskDelay(10);  // 缩短延时，提高轮询频率
    }
}



