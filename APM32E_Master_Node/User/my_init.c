#include "my_init.h"

/*!
 * @brief       Delay
 *
 * @param       count:  delay count
 *
 * @retval      None
 */
void Delay(uint32_t count)
{
    volatile uint32_t delay = count;
    while (delay--);
}

void GPIO_INIT()
{
    GPIO_Config_T GPIO_configStruct;
    
    // 使能 GPIOA 时钟
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
    
    // 配置 PA11 为推挽输出
    GPIO_configStruct.pin = GPIO_PIN_11;
    GPIO_configStruct.mode = GPIO_MODE_OUT_PP;
    GPIO_configStruct.speed = GPIO_SPEED_50MHz;
    
    GPIO_Config(GPIOA, &GPIO_configStruct);
    
    // 设置 PA11 为低电平
    GPIO_ResetBit(GPIOA, GPIO_PIN_11);
}

void USART_Init(void)
{
    USART_Config_T usartConfigStruct;

    usartConfigStruct.baudRate = 115200;
    usartConfigStruct.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    usartConfigStruct.mode = USART_MODE_TX_RX;
    usartConfigStruct.parity = USART_PARITY_NONE;
    usartConfigStruct.stopBits = USART_STOP_BIT_1;
    usartConfigStruct.wordLength = USART_WORD_LEN_8B;

    APM_MINI_COMInit(COM1, &usartConfigStruct);
    APM_MINI_COMInit(COM2, &usartConfigStruct);

    Delay(0x7FFF);

    /* 暂时禁用中断，使用轮询方式 */
    // USART_EnableInterrupt(MINI_COM2, USART_INT_RXBNE);
    // NVIC_EnableIRQRequest(USART2_IRQn, 3, 0);
    
    /* 清除接收标志 */
    USART_ClearStatusFlag(MINI_COM2, USART_FLAG_RXBNE);
}

//// 串口发送函数
//void USART_SendData(uint8_t* data, uint16_t length)
//{
//    for (uint16_t i = 0; i < length; i++)
//    {
//        // 等待发送数据寄存器空
//        while (USART_ReadStatusFlag(MINI_COM2, USART_FLAG_TXBE) == RESET);
//        
//        // 发送数据
//        USART_TxData(MINI_COM2, data[i]);
//    }
//    
//    // 等待发送完成
//    while (USART_ReadStatusFlag(MINI_COM2, USART_FLAG_TC) == RESET);
//}
