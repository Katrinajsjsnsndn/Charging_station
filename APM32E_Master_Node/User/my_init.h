#ifndef __MY_INIT_H
#define __MY_INIT_H

#include "apm32e10x_gpio.h"
#include "apm32e10x_usart.h"
#include "apm32e10x_rcm.h"
#include "board.h"

// 延时函数
void Delay(uint32_t count);

// GPIO初始化函数
void GPIO_INIT(void);

// 串口初始化函数
void USART_Init(void);

// 串口发送函数
void USART_SendData(uint8_t* data, uint16_t length);

#endif
