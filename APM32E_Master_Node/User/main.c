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
                4096,  // 增加栈大小到4KB
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
static uint8_t key_val = 0, key_old = 0, key_down = 0;

// 选项框高亮相关变量
static int selected_option = 0;  // 当前选中的选项索引 (0-3)
lv_obj_t* control_labels[3] = {NULL, NULL, NULL}; // Control页面标签指针
static int selected_control_option = 0; // Control页面当前选中的选项索引 (0-2)



// 选项框高亮功能函数
void update_option_highlight(lv_ui *ui)
{
    // 获取所有选项框
    lv_obj_t* option_boxes[4] = {
        ui->screen_Master_cont_1,
        ui->screen_Master_cont_2,
        ui->screen_Master_cont_3,
        ui->screen_Master_cont_4
    };
    
    // 更新所有选项框的样式
    for(int i = 0; i < 4; i++) {
        if(i == selected_option) {
            // 高亮选中的选项：绿色边框，高亮背景，添加阴影
            lv_obj_set_style_border_width(option_boxes[i], 3, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(option_boxes[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(option_boxes[i], 8, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(option_boxes[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
        } else {
            // 普通样式：恢复原始颜色
            lv_obj_set_style_border_width(option_boxes[i], 2, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(option_boxes[i], lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(option_boxes[i], 0, LV_PART_MAIN|LV_STATE_DEFAULT);
            // 恢复原始背景色
            if(i == 0) lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0x5a9838), LV_PART_MAIN|LV_STATE_DEFAULT);
            else if(i == 1) lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
            else if(i == 2) lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0xff9800), LV_PART_MAIN|LV_STATE_DEFAULT);
            else if(i == 3) lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0xf44336), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
    }
}

void select_option(int new_index)
{
    if(new_index < 0 || new_index >= 4) return;
    
    selected_option = new_index;
    update_option_highlight(&guider_ui);
}

void select_control_option(int new_index)
{
    if(new_index < 0 || new_index >= 3) return;
    
    selected_control_option = new_index;
    guider_ui.screen_control_selected_index = new_index;
    update_control_option_highlight();
}

void update_control_option_highlight(void)
{
    for(int i = 0; i < 3; i++) {
        if(control_labels[i] == NULL || !lv_obj_is_valid(control_labels[i])) continue;
        
        if(i == selected_control_option) {
            lv_obj_set_style_bg_color(control_labels[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(control_labels[i], 255, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(control_labels[i], 2, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(control_labels[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
        } else {
            lv_obj_set_style_bg_opa(control_labels[i], 0, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(control_labels[i], 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        }
    }
}




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
    // 添加调试指示
    APM_MINI_LEDToggle(LED2);  // 指示任务启动
    
    // 分步初始化，避免内存问题
    setup_ui(&guider_ui);           // 初始化 UI
    vTaskDelay(10);  // 给系统一些时间处理
    
    events_init(&guider_ui);       // 初始化 事件  
    vTaskDelay(10);  // 给系统一些时间处理
    
    // 设置初始高亮（第一个选项）
    update_option_highlight(&guider_ui);
    

    
    while(1)
    {
        key_val = key_scan();
        key_down = key_val & (key_val ^ key_old);
        key_old = key_val;
        
        if (key_down)
        {
            if (key_down == KEY_OK)
            {
                // 检查当前界面，实现正确的切换逻辑
                if (lv_scr_act() == guider_ui.screen_Master)
                {
                    // 从master界面进入detail界面
                    ui_load_scr_animation(&guider_ui, &guider_ui.screen_detail, guider_ui.screen_detail_del, &guider_ui.screen_Master_del, setup_scr_screen_detail, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
                }
                else if (lv_scr_act() == guider_ui.screen_detail)
                {
                    // 从detail界面进入control界面 - 直接加载测试
                    if (guider_ui.screen_control == NULL) {
                        setup_scr_screen_control(&guider_ui);
                    }
                    lv_scr_load(guider_ui.screen_control);
                    // 初始化control页面选中状态
                    selected_control_option = 0;
                    guider_ui.screen_control_selected_index = 0;
                    // 恢复高亮功能
                    update_control_option_highlight();
                }
            }
            else if(key_down == KEY_RETURN)
            {
                // 检查当前界面，实现正确的返回逻辑
                if (lv_scr_act() == guider_ui.screen_detail)
                {
                    // 从detail界面返回master界面
                    ui_load_scr_animation(&guider_ui, &guider_ui.screen_Master, guider_ui.screen_Master_del, &guider_ui.screen_detail_del, setup_scr_screen_Master, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
                    // 重新应用高亮
                    update_option_highlight(&guider_ui);
                }
                else if (lv_scr_act() == guider_ui.screen_control)
                {
                    // 从control界面返回detail界面
                    ui_load_scr_animation(&guider_ui, &guider_ui.screen_detail, guider_ui.screen_detail_del, &guider_ui.screen_control_del, setup_scr_screen_detail, LV_SCR_LOAD_ANIM_NONE, 200, 200, false, true);
                }
            }
            // 方向键处理（仅在master界面有效）
            else if(key_down == KEY_LEFT && lv_scr_act() == guider_ui.screen_Master)
            {
                // 向左选择（上一个选项）
                int new_index = selected_option - 1;
                if(new_index < 0) new_index = 3;  // 循环到最后一个
                select_option(new_index);
            }
            else if(key_down == KEY_RIGHT && lv_scr_act() == guider_ui.screen_Master)
            {
                // 向右选择（下一个选项）
                int new_index = selected_option + 1;
                if(new_index >= 4) new_index = 0;  // 循环到第一个
                select_option(new_index);
            }
            else if(key_down == KEY_UP && lv_scr_act() == guider_ui.screen_Master)
            {
                // 向上选择（上一行）
                int new_index = selected_option - 2;
                if(new_index < 0) new_index += 4;  // 循环
                select_option(new_index);
            }
            else if(key_down == KEY_DOWN && lv_scr_act() == guider_ui.screen_Master)
            {
                // 向下选择（下一行）
                int new_index = selected_option + 2;
                if(new_index >= 4) new_index -= 4;  // 循环
                select_option(new_index);
            }
            // Control页面上下选择处理
            else if(key_down == KEY_UP && lv_scr_act() == guider_ui.screen_control)
            {
                int new_index = selected_control_option - 1;
                if(new_index < 0) new_index = 2;
                select_control_option(new_index);
            }
            else if(key_down == KEY_DOWN && lv_scr_act() == guider_ui.screen_control)
            {
                int new_index = selected_control_option + 1;
                if(new_index > 2) new_index = 0;
                select_control_option(new_index);
            }                   
        }
        
        lv_task_handler();
        
        // 动态更新一些显示数据
        static uint32_t update_counter = 0;
        update_counter++;
        
        // 每100次循环更新一次数据（约500ms）
        if(update_counter % 100 == 0) {
            // 更新control页面的进度条
            if(lv_scr_act() == guider_ui.screen_control && guider_ui.screen_control_bar_1 != NULL) {
                static int bar_value = 75;
                bar_value = (bar_value + 5) % 100;
                lv_bar_set_value(guider_ui.screen_control_bar_1, bar_value, LV_ANIM_ON);
            }
            
            // 更新detail页面的数据（如果有的话）
            if(lv_scr_act() == guider_ui.screen_detail) {
                // 这里可以添加detail页面的数据更新
            }
        }
        
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
