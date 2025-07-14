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
//		setup_ui(&guider_ui);
//		events_init(&guider_ui);
    while(1)
    {
        //lv_timer_handler(); /* LVGL��ʱ�� */
        vTaskDelay(5);
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

//    // 1. ����ADCת��
//    HAL_ADC_Start(&hadc1);

//    // 2. �ȴ�ת�����
//    if (HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY) == HAL_OK)
//    {
//        // 3. ��ȡת�����
//        adc_value = HAL_ADC_GetValue(&hadc1);
//    }

//    // 4. ֹͣADC
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
