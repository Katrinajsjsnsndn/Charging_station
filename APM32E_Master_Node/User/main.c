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

#include "apm32e10x.h"
#include "apm32e10x_adc.h"
#include "apm32e10x_gpio.h"
#include "apm32e10x_rcm.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "list.h"
#include "my_init.h"
#include "lcd.h"
#include "test code.h"
#include "charge_control.h"
#include "rs485.h"
#include "charging_station_ui.h"
#include "lvgl.h" 
#include "lv_port_disp_template.h"
#include "gui_guider.h"           // Gui Guider 生成的界面和控件的声明
#include "events_init.h"          // Gui Guider 生成的初始化事件、回调函数
#include "lvgl_charging_station_ui.h"

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
static TaskHandle_t xHandleLVGL = NULL;

/* User create task */
static void UserTaskCreate(void);

/* Led toggle task */
void charging_station_ui_task(void* pvParameters);
/* Usart test task */
void vTaskUsartTest(void* pvParameters);
void Lvgl_ui_task(void* pvParameters);

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
    USART2_DMA_RX_Init();
    IIC_GPIO_Config();

    LCD_Init();
    LCD_Display_Dir(3);
    LCD_Clear(WHITE);
    lv_init();
    lv_port_disp_init();
    
    // 初始化ADC
    ADC_Init_Once();
    
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
//    xTaskCreate(charging_station_ui_task,
//                "charging_station_ui_task",
//                256,  // 增加栈空间
//                NULL,
//                0,
//                &xHandleTaskLedToggle);

    xTaskCreate(vTaskUsartTest,
                "TaskUsartTest",
                128,  // 增加栈空间
                NULL,
                2,
                &xHandleTaskUsartTest);
	  xTaskCreate(Lvgl_ui_task,
                "Lvgl_ui_task",
                1024,  // 增加栈空间
                NULL,
                2,
                &xHandleLVGL);
}


// FreeRTOSConfig.h 里要有 configUSE_TICK_HOOK 设为 1
void vApplicationTickHook(void)
{
    lv_tick_inc(1); // 1ms，如果你的 FreeRTOS tick 是 1ms
}
/**@} end of group GPIO_Toggle_Functions */
/**@} end of group GPIO_Toggle */
/**@} end of group Examples */
lv_ui  guider_ui;                     // 声明 界面对象
static uint8_t key_val = 0, key_old = 0, key_down = 0;

uint8_t key_scan(void)
{
    uint8_t temp = KEY_NONE;
    if (GPIO_ReadInputBit(GPIOB, GPIO_PIN_1) == RESET)         // KEY1
        temp = KEY_RETURN;
    else if (GPIO_ReadInputBit(GPIOA, GPIO_PIN_7) == RESET)    // KEY2
        temp = KEY_LEFT;
    else if (GPIO_ReadInputBit(GPIOC, GPIO_PIN_5) == RESET)    // KEY3
        temp = KEY_MENU;
    else if (GPIO_ReadInputBit(GPIOC, GPIO_PIN_4) == RESET)    // KEY4
        temp = KEY_OK;
    else if (GPIO_ReadInputBit(GPIOA, GPIO_PIN_6) == RESET)    // KEY5
        temp = KEY_RIGHT;
    else if (GPIO_ReadInputBit(GPIOA, GPIO_PIN_5) == RESET)    // KEY6
        temp = KEY_DOWN;
    else if (GPIO_ReadInputBit(GPIOA, GPIO_PIN_4) == RESET)    // KEY7
        temp = KEY_UP;
    return temp;
}
void Lvgl_ui_task(void* pvParameters)
{
		//charging_station_ui_create(lv_scr_act());
    setup_ui(&guider_ui);           // 初始化 UI
		events_init(&guider_ui);       // 初始化 事件  
	while(1)
	{
				key_val = key_scan();
        key_down = key_val & (key_val ^ key_old);
        key_old = key_val;
				if (key_down)
        {
					 if (key_down == KEY_OK)
					 {
						 ui_load_scr_animation(&guider_ui, &guider_ui.screen_detail, guider_ui.screen_detail_del, &guider_ui.screen_Master_del, setup_scr_screen_detail, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
					 }
					 else if(key_down==KEY_RETURN)
					 {
						 ui_load_scr_animation(&guider_ui, &guider_ui.screen_Master, guider_ui.screen_Master_del, &guider_ui.screen_control_del, setup_scr_screen_Master, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
					 }					
				}
        lv_task_handler();
        vTaskDelay(5);
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
uint16_t adc_pa1, adc_pb0;
extern Station_Info_t stations[STATION_NUM] ;
float out_cal,out_current;


void vTaskUsartTest(void* pvParameters)
{
    
    // 初始化ADC
    ADC_Init_Once();
  
    while (1)
    {
 

//			    // 读取PA1和PB0的ADC值
//        adc_pa1 = ADC_ReadChannel(1);  // PA1对应ADC通道1
//        //adc_pb0 = ADC_ReadChannel(8);  // PB0对应ADC通道8
//        
//    	out_cal=(adc_pb0/4095.0f)*3.3/0.005;
//			out_current=(adc_pa1/4095.0f)*3.3*11.0f;
//			stations[0].discharge_voltage=out_current;
//			stations[0].discharge_current=1.5;
//			stations[0].discharge_power=out_cal*out_current;

//        RS485_Master_Receive_Process();
//        /* Task blocking time Delay */
        vTaskDelay(10);
		
    }
}

// ADC初始化标志
static uint8_t adc_initialized = 0;

/* ADC初始化函数 */
void ADC_Init_Once(void)
{
    if(adc_initialized) return;
    
    GPIO_Config_T gpioConfig;
    
    // 使能ADC1时钟
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);
    
    // 配置GPIO为模拟输入
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB);
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.pin = GPIO_PIN_1;  // 配置PA1
    gpioConfig.mode = GPIO_MODE_ANALOG;
    GPIO_Config(GPIOA, &gpioConfig);
    
    gpioConfig.pin = GPIO_PIN_0;  // 配置PB0
    GPIO_Config(GPIOB, &gpioConfig);
    
    // 配置ADC
    ADC_Config_T adcConfig;
    ADC_ConfigStructInit(&adcConfig);
    // 根据APM32库的特点，使用软件触发模式
    adcConfig.externalTrigConv = ADC_EXT_TRIG_CONV_None;  // 软件触发
    ADC_Config(ADC1, &adcConfig);
    
    // 使能ADC
    ADC_Enable(ADC1);
    
    // 等待ADC稳定
    for(volatile uint32_t i = 0; i < 1000; i++);
    
    adc_initialized = 1;
}

/* ADC读取函数 */
uint16_t ADC_ReadChannel(uint8_t channel)
{
    // 确保ADC已初始化
    ADC_Init_Once();
    
    // 配置ADC通道
    ADC_ConfigRegularChannel(ADC1, channel, 1, ADC_SAMPLETIME_239CYCLES5);
    
    // 开始转换
    ADC_EnableSoftwareStartConv(ADC1);
    
    // 等待转换完成
    uint32_t timeout = 10000;
    while(!ADC_ReadStatusFlag(ADC1, ADC_FLAG_EOC) && timeout--);
    
    // 读取结果
    uint16_t result = ADC_ReadConversionValue(ADC1);
    
    // 如果超时，返回0
    if(timeout == 0)
    {
        return 0;
    }
    
    return result;
}

