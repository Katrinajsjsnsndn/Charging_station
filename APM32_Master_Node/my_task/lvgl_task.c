#include "lvgl_task.h"
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "rs485.h"
#include "lcd.h"
#include "rs485.h"
#include "lvgl.h" 
#include "lv_port_disp_template.h"

#include "charge_control.h"
#include "charging_station_ui.h"


extern uint16_t dac_set;
extern Station_Info_t stations[STATION_NUM];
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
                (uint16_t       )256,
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
    
			MCP4725_WriteData_Digital(1240);

    while(1)
    {
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


				RS485_Master_Receive_Process();
        vTaskDelay(10);
    }
}





 RS485_Frame_t *pHdr;
// 接收处理函数
float receive_current, receive_val;

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

    /* 2.3 校验和检查：校验范围 = 整个帧（除最后一个字节） */
    uint8_t calc_cs = RS485_CalcChecksum(rx_buffer, expect_len - 1);
    if (calc_cs != rx_buffer[expect_len - 1]) goto frame_err;

   /* 5. 命令分发 */
    switch (pHdr->cmd) {
        case CMD_SET_PARAM:
            if (pHdr->len >= 1) {
                uint8_t param = pHdr->data[0]; // 直接通过结构体访问
                switch (param) {
                    case 0: /* 处理 param=0 */ break;
                    case 1: /* 处理 param=1 */ break;
                    case 3: /* 处理 param=3 */ break;
                    default: /* 无效参数 */ break;
                }
            }
            break;

        case CMD_READ_DATA:
            // 示例：回复当前数据（read_current 和 read_val）
							memcpy(&receive_current, &pHdr->data[0], 4); // 解析第1个float（read_current）
							memcpy(&receive_val, &pHdr->data[4], 4);     // 解析第2个float（read_val）

            break;

        default:
            /* 未知命令 */
            break;
    }
		if(pHdr->addr_from==0x01)
		{
				stations[0].battery_connected = 1;
				stations[0].current = receive_current;
				stations[0].voltage = receive_val;
				stations[0].power = receive_current * receive_val;

				// 状态判断
				if(stations[0].battery_connected) 
				{
						if(stations[0].current > 0.1f) {
								stations[0].status = 1; // 充电中
						} else {
								stations[0].status = 0; // 空闲
						}
				} 
				else 
				{
						stations[0].status = 0; // 未连接
				}
		}
frame_err:
    /* 3. 重新启动下一轮 DMA 接收 */
    rx_done = 0;
    rx_len  = 0;
    HAL_UART_Receive_DMA(&huart2, rx_buffer, BUFFER_SIZE);
}
