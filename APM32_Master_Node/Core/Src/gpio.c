/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
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
#include "gpio.h"

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, DB13_Pin|DB14_Pin|DB15_Pin|DB0_Pin
                          |DB5_Pin|DB8_Pin|DB9_Pin|DB10_Pin
                          |DB11_Pin|DB12_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, CS_Pin|CLK_Pin|GPIO_PIN_7|DC_Pin
                          |RS485_EN_Pin|GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |SDI_Pin|BL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, I2C_SCL_Pin|I2C_SDA_Pin|FMARK_Pin|LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : DB13_Pin DB14_Pin DB15_Pin DB0_Pin
                           DB5_Pin DB8_Pin DB9_Pin DB10_Pin
                           DB11_Pin DB12_Pin */
  GPIO_InitStruct.Pin = DB13_Pin|DB14_Pin|DB15_Pin|DB0_Pin
                          |DB5_Pin|DB8_Pin|DB9_Pin|DB10_Pin
                          |DB11_Pin|DB12_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : KEY_7_Pin KEY_6_Pin KEY_5_Pin KEY_4_Pin
                           KEY_3_Pin KEY_2_Pin */
  GPIO_InitStruct.Pin = KEY_7_Pin|KEY_6_Pin|KEY_5_Pin|KEY_4_Pin
                          |KEY_3_Pin|KEY_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_Pin CLK_Pin PA6 PA7
                           DC_Pin PA15 */
  GPIO_InitStruct.Pin = CS_Pin|CLK_Pin|GPIO_PIN_6|GPIO_PIN_7
                          |DC_Pin|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB1 PB12 PB13 PB14
                           SDI_Pin BL_Pin LED_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14
                          |SDI_Pin|BL_Pin|LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : I2C_SCL_Pin I2C_SDA_Pin */
  GPIO_InitStruct.Pin = I2C_SCL_Pin|I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PB15 KEY_1_Pin PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|KEY_1_Pin|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : RS485_EN_Pin */
  GPIO_InitStruct.Pin = RS485_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS485_EN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : SDO_Pin */
  GPIO_InitStruct.Pin = SDO_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(SDO_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FMARK_Pin */
  GPIO_InitStruct.Pin = FMARK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(FMARK_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 2 */

/* USER CODE END 2 */
