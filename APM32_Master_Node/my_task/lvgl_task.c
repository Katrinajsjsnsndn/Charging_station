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


extern float current;
extern uint16_t dac_set;
void Current_Set(void);
uint8_t read_key();

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
#define LV_DEMO_STK_SIZE    512        /* �����ջ��С */
TaskHandle_t LV_DEMOTask_Handler;       /* ������ */
void lv_demo_task(void *pvParameters);  /* ������ */

/* LED_TASK ���� ����
 * ����: ������ �������ȼ� ��ջ��С ��������
 */
#define LED_TASK_PRIO       3           /* �������ȼ� */
#define LED_STK_SIZE        0         /* �����ջ��С */
TaskHandle_t RS485Task_Handler;           /* ������ */
void rs485_task(void *pvParameters);      /* ������ */

//TODO rtos��ջ����
//lv_ui guider_ui;

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
    pvParameters = pvParameters;


    while(1)
    {

			lv_timer_handler(); /* LVGL��ʱ�� */
			vTaskDelay(5);   // 
 /* 2.1 ׼��һ���㹻��Ļ�������һ������Լ 40 �ֽڣ� */

    }
}

/**
 * @brief       led_task
 * @param       pvParameters : �������(δ�õ�)
 * @retval      ��
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
				uint8_t on_off = 1;

				if(key_down==KEY_UP)
				{
					on_off=1;
					RS485_Master_Send_Turn(0x01, &on_off, 1);				
				}
				if(key_down==KEY_DOWN)
				{
					on_off=0;
					
					RS485_Master_Send_Turn(0x01, &on_off, 1);	
				}
				if(key_down==KEY_RETURN)
				{
					on_off=3;
					RS485_Master_Send_Turn(0x01, &on_off, 1);	

				}
				RS485_Master_Receive_Process();
        vTaskDelay(10);
				HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET);
    }
}


uint8_t read_key()
{
	
    uint8_t temp = KEY_NONE;

    if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == 0)
        temp = KEY_RETURN;
//    else if (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == 0)
//        temp = KEY_MENU;
//    else if (HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == 0)
//        temp = KEY_OK;
//    else if (HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin) == 0)
//        temp = KEY_LEFT;
//    else if (HAL_GPIO_ReadPin(KEY_5_GPIO_Port, KEY_5_Pin) == 0)
//        temp = KEY_RIGHT;
//    else if (HAL_GPIO_ReadPin(KEY_6_GPIO_Port, KEY_6_Pin) == 0)
//        temp = KEY_DOWN;
//    else if (HAL_GPIO_ReadPin(KEY_7_GPIO_Port, KEY_7_Pin) == 0)
//        temp = KEY_UP;

    return temp;
}




 RS485_Frame_t *pHdr;
// ���մ�����

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

//    /* 2.3 У��ͼ�飺У�鷶Χ = ����֡�������һ���ֽڣ� */
//    uint8_t calc_cs = RS485_CalcChecksum(rx_buffer, expect_len - 1);
//    if (calc_cs != rx_buffer[expect_len - 1]) goto frame_err;

    /* 2.4 ����ַ� */
    switch (pHdr->cmd)
    {
        case CMD_SET_PARAM:
            if (pHdr->len >= 1)          /* ���� 1 �ֽڲ��� */
            {
                uint8_t param = rx_buffer[5];   /* data ��ʼλ�� */
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
            /* TODO����Э��ظ� */
            break;

        default:
            break;
    }

frame_err:
    /* 3. ����������һ�� DMA ���� */
    rx_done = 0;
    rx_len  = 0;
    HAL_UART_Receive_DMA(&huart2, rx_buffer, BUFFER_SIZE);
}