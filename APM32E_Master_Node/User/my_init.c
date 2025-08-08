#include "my_init.h"
#include "apm32e10x_dma.h"

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

    //APM_MINI_COMInit(COM1, &usartConfigStruct);
    APM_MINI_COMInit(COM2, &usartConfigStruct);

    Delay(0x7FFF);

		NVIC_SetPriority(USART2_IRQn, 6); // 6 > 5，优先级数值大，实际优先级低
		NVIC_EnableIRQ(USART2_IRQn);
		USART_EnableInterrupt(USART2, USART_INT_RXBNE);		
    /* 清除接收标志 */
    USART_ClearStatusFlag(MINI_COM2, USART_FLAG_RXBNE);
}


// ====== USART2 DMA接收全局变量 ======
#define USART2_RX_BUFFER_SIZE 128
uint8_t usart2_rx_buffer[USART2_RX_BUFFER_SIZE];
volatile uint16_t usart2_rx_len = 0;
volatile uint8_t usart2_rx_done = 0;

// ====== USART2 DMA接收初始化 ======
void USART2_DMA_RX_Init(void)
{
    // 1. 使能时钟
    RCM_EnableAPB1PeriphClock(RCM_APB1_PERIPH_USART2);
    RCM_EnableAHBPeriphClock(RCM_AHB_PERIPH_DMA1);

    // 2. 配置USART2参数（如已在别处配置，可省略）
    // USART_Config_T usartConfig;
    // USART_ConfigStructInit(&usartConfig);
    // usartConfig.baudRate = 115200;
    // usartConfig.mode = USART_MODE_TX_RX;
    // usartConfig.wordLength = USART_WORD_LEN_8B;
    // usartConfig.stopBits = USART_STOP_BIT_1;
    // usartConfig.parity = USART_PARITY_NONE;
    // usartConfig.hardwareFlow = USART_HARDWARE_FLOW_NONE;
    // APM_MINI_COMInit(COM2, &usartConfig);

    // 3. 配置DMA1_Channel6
    DMA_Config_T dmaConfig;
    DMA_ConfigStructInit(&dmaConfig);
    dmaConfig.peripheralBaseAddr = (uint32_t)&USART2->DATA;
    dmaConfig.memoryBaseAddr = (uint32_t)usart2_rx_buffer;
    dmaConfig.dir = DMA_DIR_PERIPHERAL_SRC; // 外设到内存
    dmaConfig.bufferSize = USART2_RX_BUFFER_SIZE;
    dmaConfig.peripheralInc = DMA_PERIPHERAL_INC_DISABLE;
    dmaConfig.memoryInc = DMA_MEMORY_INC_ENABLE;
    dmaConfig.peripheralDataSize = DMA_PERIPHERAL_DATA_SIZE_BYTE;
    dmaConfig.memoryDataSize = DMA_MEMORY_DATA_SIZE_BYTE;
    dmaConfig.loopMode = DMA_MODE_NORMAL;
    dmaConfig.priority = DMA_PRIORITY_HIGH;
    dmaConfig.M2M = DMA_M2MEN_DISABLE;
    DMA_Config(DMA1_Channel6, &dmaConfig);

    // 4. 使能DMA
    DMA_Enable(DMA1_Channel6);

    // 5. 使能USART2的DMA接收请求
    USART_EnableDMA(USART2, USART_DMA_RX);

    // 6. 使能USART2空闲中断
    USART_EnableInterrupt(USART2, USART_INT_IDLE);

    // 7. 配置NVIC
    NVIC_SetPriority(USART2_IRQn, 6);
    NVIC_EnableIRQ(USART2_IRQn);
		USART_EnableInterrupt(USART2, USART_INT_IDLE);
}


#define USART2_RX_BUFFER_SIZE 128
void USART2_IRQHandler(void)
{
    // 直接用寄存器位判断
    if (USART2->STS & USART_FLAG_IDLE)
    {
        volatile uint32_t tmp;
        tmp = USART2->STS;
        tmp = USART2->DATA;

        DMA_Disable(DMA1_Channel6);
        usart2_rx_len = USART2_RX_BUFFER_SIZE - DMA1_Channel6->CHNDATA;
        usart2_rx_done = 1;
        DMA1_Channel6->CHMADDR = (uint32_t)usart2_rx_buffer;
        DMA1_Channel6->CHNDATA = USART2_RX_BUFFER_SIZE;
        DMA_Enable(DMA1_Channel6);
    }
}


// ADC初始化标志
static uint8_t adc_initialized = 0;

/* ADC初始化函数 */
void ADC_Init_Once(void)
{
    if(adc_initialized) return;
    
    GPIO_Config_T gpioConfig;
    
    // 使能ADC1时钟
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_ADC1);
    
    // 配置GPIO为模拟输入
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOA);
    RCM_EnableAPB2PeriphClock(RCM_APB2_PERIPH_GPIOB);
    GPIO_ConfigStructInit(&gpioConfig);
    gpioConfig.pin = GPIO_PIN_1;  // 配置PA1
    gpioConfig.mode = GPIO_MODE_ANALOG;
    GPIO_Config(GPIOA, &gpioConfig);
    
    gpioConfig.pin = GPIO_PIN_0;  // 配置PB0
    GPIO_Config(GPIOB, &gpioConfig);
    
    // 配置ADC
    ADC_Config_T adcConfig;
    ADC_ConfigStructInit(&adcConfig);
    // 根据APM32库的特点，使用软件触发模式
    adcConfig.externalTrigConv = ADC_EXT_TRIG_CONV_None;  // 软件触发
    ADC_Config(ADC1, &adcConfig);
    
    // 使能ADC
    ADC_Enable(ADC1);
    
    // 等待ADC稳定
    for(volatile uint32_t i = 0; i < 1000; i++);
    
    adc_initialized = 1;
}

/* ADC读取函数 */
uint16_t ADC_ReadChannel(uint8_t channel)
{
    // 确保ADC已初始化
    ADC_Init_Once();
    
    // 配置ADC通道
    ADC_ConfigRegularChannel(ADC1, channel, 1, ADC_SAMPLETIME_239CYCLES5);
    
    // 开始转换
    ADC_EnableSoftwareStartConv(ADC1);
    
    // 等待转换完成
    uint32_t timeout = 10000;
    while(!ADC_ReadStatusFlag(ADC1, ADC_FLAG_EOC) && timeout--);
    
    // 读取结果
    uint16_t result = ADC_ReadConversionValue(ADC1);
    
    // 如果超时，返回0
    if(timeout == 0)
    {
        return 0;
    }
    
    return result;
}

