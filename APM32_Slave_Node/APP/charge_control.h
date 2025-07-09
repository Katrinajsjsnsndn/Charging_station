#ifndef __CHARGE_CONTRROL_H
#define __CHARGE_CONTRROL_H


#include "gpio.h"


#define device_address  0xA0//7λ��ַ
#define write_bit       0
#define read_bit        1

//���Ŷ���
/*******************************************************/
#define IIC_SCL_PIN             SCL_Pin
#define IIC_SDA_PIN           	SDA_Pin
#define IIC_GPIO_PORT           GPIOB                     
#define IIC_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOB_CLK_ENABLE()


/* ���κ꣬��������������һ��ʹ�� */
#define IIC_SCL(a)			HAL_GPIO_WritePin(IIC_GPIO_PORT,IIC_SCL_PIN,a)
#define IIC_SDA(a)			HAL_GPIO_WritePin(IIC_GPIO_PORT,IIC_SDA_PIN,a)
#define IIC_SDA_SCL(a)	HAL_GPIO_WritePin(IIC_GPIO_PORT,IIC_SCL_PIN|IIC_SDA_PIN,a)
#define IIC_Read_SDA() 	HAL_GPIO_ReadPin(IIC_GPIO_PORT,IIC_SDA_PIN)

void IIC_Start(void);
void IIC_Stop(void);
void IIC_Write_Byte(uint8_t IIC_Byte);

void IIC_Wait_Ack(void);
void Enable_Charging(void);




				




void IIC_GPIO_Config(void);
void check_device(uint8_t addr);
void MCP4725_WriteData_Digital(uint16_t data);   //12λ������


#endif