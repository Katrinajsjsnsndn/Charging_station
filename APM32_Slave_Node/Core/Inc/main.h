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
extern uint8_t rx_buffer[BUFFER_SIZE];  // ���ջ�����
extern uint8_t rx_done;                 // ������ɱ�־
extern uint8_t rx_len;                  // ���յ������ݳ���
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_1_Pin GPIO_PIN_13
#define LED_1_GPIO_Port GPIOC
#define LED_2_Pin GPIO_PIN_14
#define LED_2_GPIO_Port GPIOC
#define LED_3_Pin GPIO_PIN_15
#define LED_3_GPIO_Port GPIOC
#define CD4051_A0_Pin GPIO_PIN_13
#define CD4051_A0_GPIO_Port GPIOB
#define CD4051_A1_Pin GPIO_PIN_14
#define CD4051_A1_GPIO_Port GPIOB
#define CD4051_A2_Pin GPIO_PIN_15
#define CD4051_A2_GPIO_Port GPIOB
#define BATOTSW_Pin GPIO_PIN_8
#define BATOTSW_GPIO_Port GPIOA
#define CE_Pin GPIO_PIN_12
#define CE_GPIO_Port GPIOA
#define BATA0_Pin GPIO_PIN_3
#define BATA0_GPIO_Port GPIOB
#define BATA1_Pin GPIO_PIN_4
#define BATA1_GPIO_Port GPIOB
#define BATA2_Pin GPIO_PIN_5
#define BATA2_GPIO_Port GPIOB
#define SCL_Pin GPIO_PIN_6
#define SCL_GPIO_Port GPIOB
#define SDA_Pin GPIO_PIN_7
#define SDA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
