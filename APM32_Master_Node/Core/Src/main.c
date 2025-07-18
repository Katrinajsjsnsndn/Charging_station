/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#define FRAME_START_BYTE 0xAA
#define MAX_FRAME_LENGTH 32

#include "lvgl_task.h"
#include "lcd.h"
#include "test code.h"
#include "lvgl.h" 
#include "lv_port_disp_template.h"
#include "my_lvgl_ui.h"
#include "charge_control.h"
#include "Lvgl_ui.h"
#include "../generated/gui_guider.h"
#include "../generated/events_init.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// 定义按键枚举
typedef enum {
    KEY_NONE = 0,
    KEY_RETURN = 1,
    KEY_MENU = 2,
    KEY_OK = 3,
    KEY_LEFT = 4,
    KEY_RIGHT = 5,
    KEY_DOWN = 6,
    KEY_UP = 7
} KeyEnum;
// 状态定义（可用于外部调用）
uint8_t rx_buffer[BUFFER_SIZE] = {0};  
uint8_t rx_done = 0;                   
uint8_t rx_len = 0;                 
float current=0.1;
uint16_t dac_set;
lv_ui guider_ui;
uint8_t in_detail = 0; // 新增，主界面/详情页状态

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//lv_ui guider_ui;

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
	HAL_UART_Receive_DMA(&huart2, rx_buffer, BUFFER_SIZE);
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	
	
	IIC_GPIO_Config();

	HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,GPIO_PIN_RESET);
	  LCD_Init();
    LCD_Display_Dir(2);
    LCD_Clear(WHITE);
# ifdef LVGL_UI 

				lv_init();
				lv_port_disp_init();
				lvgl_main_screen_create();
				//setup_ui(&guider_ui);
				//events_init(&guider_ui);
#endif
	lvgl_task();

  /* USER CODE END 2 */

  /* Call init function for freertos objects (in cmsis_os2.c) */
//  MX_FREERTOS_Init();

//  /* Start scheduler */
//  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
#ifdef LVGL_UI		
		uint8_t key = read_key();
        if (key != KEY_NONE) {
            if (!in_detail) {
                // 主界面按键处理
                switch (key) {
                    case KEY_LEFT:
                        if (selected_card % 2 == 1) selected_card--;
                        break;
                    case KEY_RIGHT:
                        if (selected_card % 2 == 0) selected_card++;
                        break;
                    case KEY_UP:
                        if (selected_card >= 2) selected_card -= 2;
                        break;
                    case KEY_DOWN:
                        if (selected_card < 2) selected_card += 2;
                        break;
                    case KEY_OK:
                        // 进入详情页
                        if (selected_card == 0)
                            lvgl_detail_screen_create("Station 01", "Charging", 36.2, 2.5, 90);
                        else if (selected_card == 1)
                            lvgl_detail_screen_create("Station 02", "Idle", 0, 0, 0);
                        else if (selected_card == 2)
                            lvgl_detail_screen_create("Station 03", "Charging", 42.1, 3.0, 126);
                        else if (selected_card == 3)
                            lvgl_detail_screen_create("Station 04", "Idle", 0, 0, 0);
                        in_detail = 1;
                        break;
                    case KEY_MENU:
                        // 可扩展菜单页
                        break;
                }
                lvgl_update_card_highlight(selected_card);
            } else {
                // 详情页按键处理
                if (key == KEY_RETURN) {
                    lvgl_main_screen_create();
                    in_detail = 0;
                }
            }
        }
		 lv_tick_inc(1);
    lv_task_handler();
    HAL_Delay(5);
#endif		
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
#ifdef LVGL_UI
uint8_t read_key(void)
{
	
    uint8_t temp = KEY_NONE;

    if (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == 0)
        temp = KEY_UP;
    else if (HAL_GPIO_ReadPin(KEY_6_GPIO_Port, KEY_6_Pin) == 0)
        temp = KEY_MENU;
    else if (HAL_GPIO_ReadPin(KEY_5_GPIO_Port, KEY_5_Pin) == 0)
        temp = KEY_OK;
    else if (HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == 0)
        temp = KEY_RIGHT;
    else if (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == 0)
        temp = KEY_DOWN;
    else if (HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin) == 0)
        temp = KEY_LEFT;
    else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == 0)
        temp = KEY_RETURN;

    return temp;
}
#endif
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
  /* USER CODE END USART2_IRQn 0 */
  /* USER CODE BEGIN USART2_IRQn 1 */
	/* 1. 先判断 IDLE */
    if (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_IDLE) != RESET)
    {
        __HAL_UART_CLEAR_IDLEFLAG(&huart2);   // 读 SR + 读 DR

        /* 2. 停 DMA，计算长度 */
        HAL_UART_DMAStop(&huart2);
        rx_len = BUFFER_SIZE - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);
        rx_done = 1;

        /* 3. 重新启动 DMA 接收 */
        HAL_UART_Receive_DMA(&huart2, rx_buffer, BUFFER_SIZE);
    }

  /* USER CODE END USART2_IRQn 1 */
}
/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
