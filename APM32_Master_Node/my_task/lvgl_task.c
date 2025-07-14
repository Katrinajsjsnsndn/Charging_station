#include "lvgl_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
//#include "lcd.h"
//#include "test code.h"
//#include "lvgl.h" 
//#include "lv_port_disp_template.h"
#include "semphr.h"
#include "rs485.h"
#include "test code.h"

//#include "adc.h"
//#include "dac.h"


extern float current;
extern uint16_t dac_set;
void Current_Set(void);
uint8_t read_key();

/******************************************************************************************************/
/*FreeRTOS配置*/

/* START_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define START_TASK_PRIO     1           /* 任务优先级 */
#define START_STK_SIZE      128         /* 任务堆栈大小 */
TaskHandle_t StartTask_Handler;         /* 任务句柄 */
void start_task(void *pvParameters);    /* 任务函数 */

/* LV_DEMO_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LV_DEMO_TASK_PRIO   3           /* 任务优先级 */
#define LV_DEMO_STK_SIZE    128        /* 任务堆栈大小 */
TaskHandle_t LV_DEMOTask_Handler;       /* 任务句柄 */
void lv_demo_task(void *pvParameters);  /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO       3           /* 任务优先级 */
#define LED_STK_SIZE        128         /* 任务堆栈大小 */
TaskHandle_t RS485Task_Handler;           /* 任务句柄 */
void rs485_task(void *pvParameters);      /* 任务函数 */


//lv_ui guider_ui;
void lvgl_task()
{
		//Lvgl_init();
	  xTaskCreate((TaskFunction_t )start_task,            /* 任务函数 */
                (const char*    )"start_task",          /* 任务名称 */
                (uint16_t       )START_STK_SIZE,        /* 任务堆栈大小 */
                (void*          )NULL,                  /* 传递给任务函数的参数 */
                (UBaseType_t    )START_TASK_PRIO,       /* 任务优先级 */
                (TaskHandle_t*  )&StartTask_Handler);   /* 任务句柄 */

    vTaskStartScheduler();                              /* 开启任务调度 */
	
}


/**
 * @brief       start_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
BaseType_t xStatus,xStatus_2;

void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    taskENTER_CRITICAL();           /* 进入临界区 */

    /* 创建LVGL任务 */
    xStatus=xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    /* LED测试任务 */
    xStatus_2=xTaskCreate((TaskFunction_t )rs485_task,
                (const char*    )"rs485_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&RS485Task_Handler);

    taskEXIT_CRITICAL();            /* 退出临界区 */
    vTaskDelete(StartTask_Handler); /* 删除开始任务 */
}

/**
 * @brief       LVGL运行例程
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
void lv_demo_task(void *pvParameters)
{
    pvParameters = pvParameters;
//		setup_ui(&guider_ui);
//		events_init(&guider_ui);
    while(1)
    {
        //lv_timer_handler(); /* LVGL计时器 */
        vTaskDelay(5);
    }
}

/**
 * @brief       led_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
uint16_t read_adc;
uint8_t key_val,key_old,key_down,key_up;

void rs485_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    while(1)
    {

				key_val=read_key();
				key_down= key_val&(key_val^key_old);
				key_up=~key_val&(key_val^key_old);
				key_old=key_val;
				if(key_down==KEY_UP)
				{
					RS485_Master_Send_Turn(0x01,1);
				}
				if(key_down==KEY_DOWN)
				{
					RS485_Master_Send_Turn(0x01,0);
				}
				if(key_down==KEY_LEFT)
				{
					RS485_Master_Send_Turn(0x01,3);
				}
        vTaskDelay(10);
				HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET);

    }
}


uint8_t read_key()
{
	
    uint8_t temp = KEY_NONE;

    if (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == 0)
        temp = KEY_RETURN;
    else if (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == 0)
        temp = KEY_MENU;
    else if (HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == 0)
        temp = KEY_OK;
    else if (HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin) == 0)
        temp = KEY_LEFT;
    else if (HAL_GPIO_ReadPin(KEY_5_GPIO_Port, KEY_5_Pin) == 0)
        temp = KEY_RIGHT;
    else if (HAL_GPIO_ReadPin(KEY_6_GPIO_Port, KEY_6_Pin) == 0)
        temp = KEY_DOWN;
    else if (HAL_GPIO_ReadPin(KEY_7_GPIO_Port, KEY_7_Pin) == 0)
        temp = KEY_UP;

    return temp;
}


//uint16_t Get_ADC(void)
//{
//    uint16_t adc_value = 0;

//    // 1. 启动ADC转换
//    HAL_ADC_Start(&hadc1);

//    // 2. 等待转换完成
//    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
//    {
//        // 3. 获取转换结果
//        adc_value = HAL_ADC_GetValue(&hadc1);
//    }

//    // 4. 停止ADC
//    HAL_ADC_Stop(&hadc1);
//		return adc_value;
//}

//void Current_Set()
//{
// 	dac_set=4095*((current/5.0f)/2.1f);
//	  HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, dac_set);//(x/4096)*2.1v
//    HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
//	 


//}
//void Lvgl_init()
//{
// 
//	 	LCD_Init();			   	//
//		LCD_Display_Dir(USE_LCM_DIR);		 		//

//		LCD_Clear(WHITE);		

//		lv_init();
//		lv_port_disp_init();


//}
