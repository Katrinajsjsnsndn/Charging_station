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
#include "ui_state_machine.h"

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
//                256,  
//                NULL,
//                0,
//                &xHandleTaskLedToggle);

    xTaskCreate(vTaskUsartTest,
                "TaskUsartTest",
                128,
                NULL,
                5,
                &xHandleTaskUsartTest);
	  xTaskCreate(Lvgl_ui_task,
                "Lvgl_ui_task",
                6*1024,  // 增加栈大小到4KB
                NULL,
                2,
                &xHandleLVGL);
}


void vApplicationTickHook(void)
{
    lv_tick_inc(1); 
}
/**@} end of group GPIO_Toggle_Functions */
/**@} end of group GPIO_Toggle */
/**@} end of group Examples */
lv_ui  guider_ui;                     // 声明 界面对象
lv_obj_t* control_labels[3] = {NULL, NULL, NULL}; // Control页面标签指针
// 按键扫描变量已在状态机模块中管理
// 选项框高亮相关变量已在状态机模块中管理



// 这些函数已经在状态机模块中实现，这里保留声明供外部调用
void update_option_highlight(lv_ui *ui);
void select_option(int new_index);
void select_control_option(int new_index);
void update_control_option_highlight(void);




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
// 使用状态机模块的UI任务
// 状态机模块提供了更好的代码组织和扩展性
// 核心功能：
// 1. 状态机模块自动处理所有UI页面切换和按键响应
// 2. 要添加新页面，只需在ui_state_machine.h中定义新的页面状态
// 3. 在ui_state_machine.c中实现对应的页面处理函数
// 4. 在状态转换表中添加页面间的转换规则
// 5. 所有动态数据更新都在update_dynamic_data函数中统一管理
void Lvgl_ui_task(void* pvParameters)
{
    // 初始化状态机
    ui_state_machine_init();
    
    // 获取UI上下文
    ui_context_t* ctx = ui_get_context();
    
    // 按键扫描变量
    static uint8_t key_val = 0, key_old = 0, key_down = 0;
    
    while (1) {
        // 按键扫描
        key_val = key_scan();
        key_down = key_val & (key_val ^ key_old);
        key_old = key_val;
        
        // 处理按键事件
        if (key_down) {
            key_event_t event = convert_key_to_event(key_down);
            if (event != KEY_EVENT_NONE) {
                ui_state_machine_process(ctx, event);
            }
        }
        
        // 页面更新
        ui_page_update(ctx);
        
        // LVGL任务处理
        lv_task_handler();
        
        // 动态数据更新
        update_dynamic_data(ctx);
        
        vTaskDelay(2);
    }
}

// 示例：如何在其他任务中调用状态机模块的函数
// 例如，在串口任务中更新UI数据
void update_ui_from_other_task(void)
{
    // 获取UI上下文
    ui_context_t* ctx = ui_get_context();
    
    // 检查当前页面
    if (ctx->current_page == PAGE_MAIN) {
        // 在主页面时更新某些数据
        // 例如更新功率显示等
    }
    
    // 或者直接切换到某个页面
    // switch_to_page(ctx, PAGE_DETAIL);
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


			    // 读取PA1和PB0的ADC值
        adc_pa1 = ADC_ReadChannel(1);  // PA1对应ADC通道1
        //adc_pb0 = ADC_ReadChannel(8);  // PB0对应ADC通道8
        
    	out_cal=(adc_pb0/4095.0f)*3.3/0.005;
			out_current=(adc_pa1/4095.0f)*3.3*11.0f;
			stations[0].discharge_voltage=out_current;
			stations[0].discharge_current=1.5;
			stations[0].discharge_power=out_cal*out_current;

        RS485_Master_Receive_Process();
        /* Task blocking time Delay */
        vTaskDelay(10);
		
    }
}
