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
/*FreeRTOS����*/

/* START_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define START_TASK_PRIO     1           /* �������ȼ� */
#define START_STK_SIZE      128         /* �����ջ��С */
TaskHandle_t StartTask_Handler;         /* ������ */
void start_task(void *pvParameters);    /* ������ */

/* LV_DEMO_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LV_DEMO_TASK_PRIO   3           /* �������ȼ� */
#define LV_DEMO_STK_SIZE    128        /* �����ջ��С */
TaskHandle_t LV_DEMOTask_Handler;       /* ������ */
void lv_demo_task(void *pvParameters);  /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO       3           /* �������ȼ� */
#define LED_STK_SIZE        128         /* �����ջ��С */
TaskHandle_t RS485Task_Handler;           /* ������ */
void rs485_task(void *pvParameters);      /* ������ */

//TODO rtos��ջ����

void lvgl_task()
{
		//Lvgl_init();
	  xTaskCreate((TaskFunction_t )start_task,            /* ������ */
                (const char*    )"start_task",          /* �������� */
                (uint16_t       )START_STK_SIZE,        /* �����ջ��С */
                (void*          )NULL,                  /* ���ݸ��������Ĳ��� */
                (UBaseType_t    )START_TASK_PRIO,       /* �������ȼ� */
                (TaskHandle_t*  )&StartTask_Handler);   /* ������ */

    vTaskStartScheduler();                              /* ����������� */
	
}


/**
 * @brief       start_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
 */
BaseType_t xStatus,xStatus_2;

void start_task(void *pvParameters)
{
    pvParameters = pvParameters;
    
    taskENTER_CRITICAL();           /* �����ٽ��� */

    /* ����LVGL���� */
    xStatus=xTaskCreate((TaskFunction_t )lv_demo_task,
                (const char*    )"lv_demo_task",
                (uint16_t       )LV_DEMO_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LV_DEMO_TASK_PRIO,
                (TaskHandle_t*  )&LV_DEMOTask_Handler);

    /* LED�������� */
    xStatus_2=xTaskCreate((TaskFunction_t )rs485_task,
                (const char*    )"rs485_task",
                (uint16_t       )LED_STK_SIZE,
                (void*          )NULL,
                (UBaseType_t    )LED_TASK_PRIO,
                (TaskHandle_t*  )&RS485Task_Handler);

    /* ���վUI���� */
    xTaskCreate((TaskFunction_t )charging_station_ui_task,
                (const char*    )"charging_ui_task",
                (uint16_t       )256,
                (void*          )NULL,
                (UBaseType_t    )2,
                (TaskHandle_t*  )NULL);

    taskEXIT_CRITICAL();            /* �˳��ٽ��� */
    vTaskDelete(StartTask_Handler); /* ɾ����ʼ���� */
}

/**
 * @brief       LVGL��������
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
// ���մ�����
float receive_current, receive_val;

/* ----------------------------------------------------------
 * 2. ���մ�������ͷ-len-data-checksum����ʽ����
 * ---------------------------------------------------------- */
void RS485_Master_Receive_Process(void)
{
    if (!rx_done) return;

    /* ��С���ȣ��̶�ͷ 5 �ֽ� + ���� 1 �ֽ� data + 1 �ֽ� checksum */
    if (rx_len < 7) goto frame_err;

     pHdr = (RS485_Frame_t *)rx_buffer;

    /* 2.1 ֡ͷ��� */
    if (pHdr->head != RS485_FRAME_HEAD) goto frame_err;

    /* 2.2 ���ȼ�� */
    uint16_t expect_len = 5 + pHdr->len + 1;   /* 5�̶� + data[len] + checksum */
    if (rx_len != expect_len) goto frame_err;

    /* 2.3 У��ͼ�飺У�鷶Χ = ����֡�������һ���ֽڣ� */
    uint8_t calc_cs = RS485_CalcChecksum(rx_buffer, expect_len - 1);
    if (calc_cs != rx_buffer[expect_len - 1]) goto frame_err;

   /* 5. ����ַ� */
    switch (pHdr->cmd) {
        case CMD_SET_PARAM:
            if (pHdr->len >= 1) {
                uint8_t param = pHdr->data[0]; // ֱ��ͨ���ṹ�����
                switch (param) {
                    case 0: /* ���� param=0 */ break;
                    case 1: /* ���� param=1 */ break;
                    case 3: /* ���� param=3 */ break;
                    default: /* ��Ч���� */ break;
                }
            }
            break;

        case CMD_READ_DATA:
            // ʾ�����ظ���ǰ���ݣ�read_current �� read_val��
							memcpy(&receive_current, &pHdr->data[0], 4); // ������1��float��read_current��
							memcpy(&receive_val, &pHdr->data[4], 4);     // ������2��float��read_val��

            break;

        default:
            /* δ֪���� */
            break;
    }
		if(pHdr->addr_from==0x01)
		{
				stations[0].battery_connected = 1;
				stations[0].current = receive_current;
				stations[0].voltage = receive_val;
				stations[0].power = receive_current * receive_val;

				// ״̬�ж�
				if(stations[0].battery_connected) 
				{
						if(stations[0].current > 0.1f) {
								stations[0].status = 1; // �����
						} else {
								stations[0].status = 0; // ����
						}
				} 
				else 
				{
						stations[0].status = 0; // δ����
				}
		}
frame_err:
    /* 3. ����������һ�� DMA ���� */
    rx_done = 0;
    rx_len  = 0;
    HAL_UART_Receive_DMA(&huart2, rx_buffer, BUFFER_SIZE);
}
