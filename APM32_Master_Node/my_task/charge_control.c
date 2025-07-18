#include "charge_control.h"

/**
 * @brief      充电使能
 *
 * @param[in]  参数1名称   参数1的详细描述。
 * @param[in]  参数2名称   参数2的详细描述。
 * @param[out] 返回值名称  返回值的详细描述。
 *
 * @return     返回值的类型和含义。
 *
 * @note       充电使能（CE）PA12
					
TODO 卡了很久的bug，iic引脚明明配错了为什么检测设备的代码还可以，但是之前就不行。					
					
 * @see        相关函数或类的引用。
 */
uint16_t MCP4725_Read_EEPROM(void)
{
    uint8_t buf[3];
    uint16_t value;

    IIC_Start();
    IIC_Write_Byte(0x61); // 0xC0写，0xC1读（你的模块地址，部分模块用0x61）
    IIC_Wait_Ack();
    buf[0] = IIC_Read_Byte(1); // 状态字节，ACK
    buf[1] = IIC_Read_Byte(1); // 高8位，ACK
    buf[2] = IIC_Read_Byte(0); // 低8位，NACK
    IIC_Stop();

    value = ((buf[1] << 4) | (buf[2] >> 4)) & 0x0FFF; // 12位DAC值
    return value;
}
void MCP4725_WriteData_Voltage(uint16_t Vout)   //电压单位mV
{
	uint8_t temp;
	uint16_t Dn;
	Dn = ( 4096 * Vout) / 5000; //这里的VREF_5V宏定义为5000
	temp = (0x0F00 & Dn) >> 8;  //12位数据。0000XXXX XXXXXXXX 
	IIC_Start();
	IIC_Write_Byte(0XC0);      //器件寻址，器件代吗：1100； 地址位A2，A1，A0为 0 ， 0 ， 1；-> 1100 0010
    //这个地址0XC0或0XC1,根据自己购买的模块决定
    IIC_Wait_Ack();	 
    IIC_Write_Byte(temp); 	  //将高8位送到DAC寄存器
    IIC_Wait_Ack();	 
    IIC_Write_Byte(Dn);        //将低8位送到DAC寄存器
	IIC_Wait_Ack();	
    IIC_Stop();//产生一个停止条件  	
	HAL_Delay(10);	
}


void MCP4725_WriteData_Digital(uint16_t data)   //12位数字量
{
    uint8_t data_H=0,data_L=0;
	data_H = ( 0x0F00 & data) >> 8;
	data_L = 0X00FF & data ;
	IIC_Start();
	IIC_Write_Byte(0XC0);      //器件寻址，器件代吗：1100； 地址位A2，A1，A0为 0 ， 0 ， 0；-> 1100 0010
    IIC_Wait_Ack();	 
    IIC_Write_Byte(data_H); 	
    IIC_Wait_Ack();	 
    IIC_Write_Byte(data_L);
	IIC_Wait_Ack();	
    IIC_Stop();//产生一个停止条件  	
	HAL_Delay(10);	
}
 

 /**
  * @brief  初始化IO
  * @param  无
  * @retval 无
  */
void IIC_GPIO_Config(void)
{		
    GPIO_InitTypeDef  GPIO_InitStruct;
    IIC_GPIO_CLK_ENABLE();													   
    GPIO_InitStruct.Pin = IIC_SCL_PIN | IIC_SDA_PIN;	
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;  
    /*设置引脚为上拉模式*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    /*设置引脚速率为高速 */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStruct);	
		IIC_SDA_SCL(GPIO_PIN_SET);
}

/*
*********************************************************************************************************
*	函 数 名: i2c_Delay
*	功能说明: I2C总线位延迟，最快400KHz
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*　
	 	下面的时间是通过逻辑分析仪测试得到的。
    工作条件：CPU主频72MHz ，MDK编译环境，1级优化
  
		循环次数为10时，SCL频率 = 205KHz 
		循环次数为7时，SCL频率 = 347KHz， SCL高电平时间1.5us，SCL低电平时间2.87us 
	 	循环次数为5时，SCL频率 = 421KHz， SCL高电平时间1.25us，SCL低电平时间2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
 * @brief IIC起始信号
 */
void IIC_Start(void)
{
	IIC_SDA(GPIO_PIN_SET);//先拉高SDA再拉高SCL，防止出现错误信号
	IIC_SCL(GPIO_PIN_SET);
	i2c_Delay();//延时保证SCL高电平时，SDA下降沿
	IIC_SDA(GPIO_PIN_RESET);
	IIC_SCL(GPIO_PIN_RESET);//如果不拉低SCL线，那么数据传输过程中的SDA的电平变换就会被认为是起始和结束信号
}

/*
 * @brief IIC停止信号
 */
void IIC_Stop(void)
{
	IIC_SDA(GPIO_PIN_RESET);//先拉低SDA再拉高SCL，防止出现错误信号
	IIC_SCL(GPIO_PIN_SET);
	i2c_Delay();//延时保证SCL高电平时，SDA上升沿沿
	IIC_SDA(GPIO_PIN_SET);
	/*这里就不用拉低SCL了，因为IIC通讯已经结束*/
}

/*
 * @brief IIC主机等待应答
 */
void IIC_Wait_Ack(void)
{
	//一定要先拉高SDA，再拉高SCL。否者就可能变成起始信号和结束信号了
	IIC_SDA(GPIO_PIN_SET);
	IIC_SCL(GPIO_PIN_SET);
	while(IIC_Read_SDA());
	IIC_SCL(GPIO_PIN_RESET);//IIC通讯没结束，拉低SCL线
}


/*
 * @brief IIC写字节
 * @para  待写入的字节数据
 */
void IIC_Write_Byte(uint8_t IIC_Byte)
{
	uint8_t i,data;
	data = IIC_Byte;
	for(i=0;i<8;i++)		
	{
		IIC_SCL(GPIO_PIN_RESET);
		if(data&0x80)
			IIC_SDA(GPIO_PIN_SET);
		else 
			IIC_SDA(GPIO_PIN_RESET);
		data=data<<1;
		IIC_SCL(GPIO_PIN_SET);
	}
	IIC_SCL(GPIO_PIN_RESET);//通讯未结束，拉低SCL
}
uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i, receive = 0;
    IIC_SDA(GPIO_PIN_SET); // SDA为输入
    for(i=0; i<8; i++)
    {
        IIC_SCL(GPIO_PIN_RESET);
        i2c_Delay();
        IIC_SCL(GPIO_PIN_SET);
        receive <<= 1;
        if(IIC_Read_SDA()) receive++;
        i2c_Delay();
    }
    IIC_SCL(GPIO_PIN_RESET);
    // 发送ACK或NACK
    if(ack)
        IIC_SDA(GPIO_PIN_RESET);
    else
        IIC_SDA(GPIO_PIN_SET);
    IIC_SCL(GPIO_PIN_SET);
    i2c_Delay();
    IIC_SCL(GPIO_PIN_RESET);
    IIC_SDA(GPIO_PIN_SET); // 恢复SDA
    return receive;
}
void check_device(uint8_t addr)
{
	IIC_Start();
	IIC_Write_Byte(addr | write_bit);
	IIC_Wait_Ack();
}