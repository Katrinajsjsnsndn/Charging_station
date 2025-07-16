/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#define BUFFER_SIZE 7
extern uint8_t rx_buffer[BUFFER_SIZE];  
extern uint8_t rx_done;                 
extern uint8_t rx_len;                 
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern DMA_HandleTypeDef hdma_usart2_rx;

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DB13_Pin GPIO_PIN_13
#define DB13_GPIO_Port GPIOC
#define DB14_Pin GPIO_PIN_14
#define DB14_GPIO_Port GPIOC
#define DB15_Pin GPIO_PIN_15
#define DB15_GPIO_Port GPIOC
#define DB0_Pin GPIO_PIN_0
#define DB0_GPIO_Port GPIOC
#define DB1_Pin GPIO_PIN_1
#define DB1_GPIO_Port GPIOC
#define DB2_Pin GPIO_PIN_2
#define DB2_GPIO_Port GPIOC
#define DB3_Pin GPIO_PIN_3
#define DB3_GPIO_Port GPIOC
#define CS_Pin GPIO_PIN_4
#define CS_GPIO_Port GPIOA
#define CLK_Pin GPIO_PIN_5
#define CLK_GPIO_Port GPIOA
#define DB4_Pin GPIO_PIN_4
#define DB4_GPIO_Port GPIOC
#define DB5_Pin GPIO_PIN_5
#define DB5_GPIO_Port GPIOC
#define DB6_Pin GPIO_PIN_6
#define DB6_GPIO_Port GPIOC
#define DB7_Pin GPIO_PIN_7
#define DB7_GPIO_Port GPIOC
#define DB8_Pin GPIO_PIN_8
#define DB8_GPIO_Port GPIOC
#define DB9_Pin GPIO_PIN_9
#define DB9_GPIO_Port GPIOC
#define DC_Pin GPIO_PIN_8
#define DC_GPIO_Port GPIOA
#define RS485_EN_Pin GPIO_PIN_11
#define RS485_EN_GPIO_Port GPIOA
#define DB10_Pin GPIO_PIN_10
#define DB10_GPIO_Port GPIOC
#define DB11_Pin GPIO_PIN_11
#define DB11_GPIO_Port GPIOC
#define DB12_Pin GPIO_PIN_12
#define DB12_GPIO_Port GPIOC
#define RST_Pin GPIO_PIN_3
#define RST_GPIO_Port GPIOB
#define SDO_Pin GPIO_PIN_4
#define SDO_GPIO_Port GPIOB
#define SDI_Pin GPIO_PIN_5
#define SDI_GPIO_Port GPIOB
#define FMARK_Pin GPIO_PIN_6
#define FMARK_GPIO_Port GPIOB
#define BL_Pin GPIO_PIN_7
#define BL_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_8
#define LED_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
