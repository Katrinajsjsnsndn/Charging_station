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
#include "lcd.h"
#include "rs485.h"
#include "lvgl.h" 
#include "lv_port_disp_template.h"
//#include "adc.h"
//#include "dac.h"
#include "charge_control.h"
#include "charging_station_ui.h"


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
#define LV_DEMO_STK_SIZE    256        /* 任务堆栈大小 */
TaskHandle_t LV_DEMOTask_Handler;       /* 任务句柄 */
void lv_demo_task(void *pvParameters);  /* 任务函数 */

/* LED_TASK 任务 配置
 * 包括: 任务句柄 任务优先级 堆栈大小 创建任务
 */
#define LED_TASK_PRIO       3           /* 任务优先级 */
#define LED_STK_SIZE        128         /* 任务堆栈大小 */
TaskHandle_t RS485Task_Handler;           /* 任务句柄 */
void rs485_task(void *pvParameters);      /* 任务函数 */

//TODO rtos堆栈分配

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

    /* 充电站UI任务 */
    xTaskCreate((TaskFunction_t )charging_station_ui_task,
                (const char*    )"charging_ui_task",
                (uint16_t       )128,
                (void*          )NULL,
                (UBaseType_t    )2,
                (TaskHandle_t*  )NULL);

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
    

    while(1)
    {

		check_device(0XC0);
			vTaskDelay(5);   // 

    }
}

/**
 * @brief       led_task
 * @param       pvParameters : 传入参数(未用到)
 * @retval      无
 */
uint16_t read_adc;

void rs485_task(void *pvParameters)
{
    pvParameters = pvParameters;

    while(1)
    {

//				key_val=read_key();
//				key_down= key_val&(key_val^key_old);
//				key_up=~key_val&(key_val^key_old);
//				key_old=key_val;
//				uint8_t on_off = 1;

//				if(key_down==KEY_UP)
//				{
//					on_off=1;
//					RS485_Master_Send_Turn(0x01, &on_off, 1);				
//				}
//				if(key_down==KEY_DOWN)
//				{
//					on_off=0;
//					
//					RS485_Master_Send_Turn(0x01, &on_off, 1);	
//				}
//				if(key_down==KEY_RETURN)
//				{
//					on_off=3;
//					RS485_Master_Send_Turn(0x01, &on_off, 1);	

//				}
//				RS485_Master_Receive_Process();
        vTaskDelay(10);
				HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET);
    }
}





 RS485_Frame_t *pHdr;
// 接收处理函数

/* ----------------------------------------------------------
 * 2. 接收处理：按“头-len-data-checksum”方式解析
 * ---------------------------------------------------------- */
void RS485_Master_Receive_Process(void)
{
    if (!rx_done) return;

    /* 最小长度：固定头 5 字节 + 至少 1 字节 data + 1 字节 checksum */
    if (rx_len < 7) goto frame_err;

     pHdr = (RS485_Frame_t *)rx_buffer;

    /* 2.1 帧头检查 */
    if (pHdr->head != RS485_FRAME_HEAD) goto frame_err;

    /* 2.2 长度检查 */
    uint16_t expect_len = 5 + pHdr->len + 1;   /* 5固定 + data[len] + checksum */
    if (rx_len != expect_len) goto frame_err;

//    /* 2.3 校验和检查：校验范围 = 整个帧（除最后一个字节） */
//    uint8_t calc_cs = RS485_CalcChecksum(rx_buffer, expect_len - 1);
//    if (calc_cs != rx_buffer[expect_len - 1]) goto frame_err;

    /* 2.4 命令分发 */
    switch (pHdr->cmd)
    {
        case CMD_SET_PARAM:
            if (pHdr->len >= 1)          /* 至少 1 字节参数 */
            {
                uint8_t param = rx_buffer[5];   /* data 起始位置 */
                if (param == 1)
                {
                    
                }
                else if (param == 0)
                {
                }
                else if (param == 3)
                {
                }
            }
            break;

        case CMD_READ_DATA:
            /* TODO：按协议回复 */
            break;

        default:
            break;
    }

frame_err:
    /* 3. 重新启动下一轮 DMA 接收 */
    rx_done = 0;
    rx_len  = 0;
    HAL_UART_Receive_DMA(&huart2, rx_buffer, BUFFER_SIZE);
}