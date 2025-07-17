#include "charge_control.h"

/**
 * @brief      ���ʹ��
 *
 * @param[in]  ����1����   ����1����ϸ������
 * @param[in]  ����2����   ����2����ϸ������
 * @param[out] ����ֵ����  ����ֵ����ϸ������
 *
 * @return     ����ֵ�����ͺͺ��塣
 *
 * @note       ���ʹ�ܣ�CE��PA12
					
TODO ���˺ܾõ�bug��iic�������������Ϊʲô����豸�Ĵ��뻹���ԣ�����֮ǰ�Ͳ��С�					
					
 * @see        ��غ�����������á�
 */
uint16_t MCP4725_Read_EEPROM(void)
{
    uint8_t buf[3];
    uint16_t value;

    IIC_Start();
    IIC_Write_Byte(0x61); // 0xC0д��0xC1�������ģ���ַ������ģ����0x61��
    IIC_Wait_Ack();
    buf[0] = IIC_Read_Byte(1); // ״̬�ֽڣ�ACK
    buf[1] = IIC_Read_Byte(1); // ��8λ��ACK
    buf[2] = IIC_Read_Byte(0); // ��8λ��NACK
    IIC_Stop();

    value = ((buf[1] << 4) | (buf[2] >> 4)) & 0x0FFF; // 12λDACֵ
    return value;
}
void MCP4725_WriteData_Voltage(uint16_t Vout)   //��ѹ��λmV
{
	uint8_t temp;
	uint16_t Dn;
	Dn = ( 4096 * Vout) / 5000; //�����VREF_5V�궨��Ϊ5000
	temp = (0x0F00 & Dn) >> 8;  //12λ���ݡ�0000XXXX XXXXXXXX 
	IIC_Start();
	IIC_Write_Byte(0XC0);      //����Ѱַ����������1100�� ��ַλA2��A1��A0Ϊ 0 �� 0 �� 1��-> 1100 0010
    //�����ַ0XC0��0XC1,�����Լ������ģ�����
    IIC_Wait_Ack();	 
    IIC_Write_Byte(temp); 	  //����8λ�͵�DAC�Ĵ���
    IIC_Wait_Ack();	 
    IIC_Write_Byte(Dn);        //����8λ�͵�DAC�Ĵ���
	IIC_Wait_Ack();	
    IIC_Stop();//����һ��ֹͣ����  	
	HAL_Delay(10);	
}


void MCP4725_WriteData_Digital(uint16_t data)   //12λ������
{
    uint8_t data_H=0,data_L=0;
	data_H = ( 0x0F00 & data) >> 8;
	data_L = 0X00FF & data ;
	IIC_Start();
	IIC_Write_Byte(0XC0);      //����Ѱַ����������1100�� ��ַλA2��A1��A0Ϊ 0 �� 0 �� 0��-> 1100 0010
    IIC_Wait_Ack();	 
    IIC_Write_Byte(data_H); 	
    IIC_Wait_Ack();	 
    IIC_Write_Byte(data_L);
	IIC_Wait_Ack();	
    IIC_Stop();//����һ��ֹͣ����  	
	HAL_Delay(10);	
}
 

 /**
  * @brief  ��ʼ��IO
  * @param  ��
  * @retval ��
  */
void IIC_GPIO_Config(void)
{		
    GPIO_InitTypeDef  GPIO_InitStruct;
    IIC_GPIO_CLK_ENABLE();													   
    GPIO_InitStruct.Pin = IIC_SCL_PIN | IIC_SDA_PIN;	
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_OD;  
    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    /*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(IIC_GPIO_PORT, &GPIO_InitStruct);	
		IIC_SDA_SCL(GPIO_PIN_SET);
}

/*
*********************************************************************************************************
*	�� �� ��: i2c_Delay
*	����˵��: I2C����λ�ӳ٣����400KHz
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void i2c_Delay(void)
{
	uint8_t i;

	/*��
	 	�����ʱ����ͨ���߼������ǲ��Եõ��ġ�
    ����������CPU��Ƶ72MHz ��MDK���뻷����1���Ż�
  
		ѭ������Ϊ10ʱ��SCLƵ�� = 205KHz 
		ѭ������Ϊ7ʱ��SCLƵ�� = 347KHz�� SCL�ߵ�ƽʱ��1.5us��SCL�͵�ƽʱ��2.87us 
	 	ѭ������Ϊ5ʱ��SCLƵ�� = 421KHz�� SCL�ߵ�ƽʱ��1.25us��SCL�͵�ƽʱ��2.375us 
	*/
	for (i = 0; i < 10; i++);
}

/*
 * @brief IIC��ʼ�ź�
 */
void IIC_Start(void)
{
	IIC_SDA(GPIO_PIN_SET);//������SDA������SCL����ֹ���ִ����ź�
	IIC_SCL(GPIO_PIN_SET);
	i2c_Delay();//��ʱ��֤SCL�ߵ�ƽʱ��SDA�½���
	IIC_SDA(GPIO_PIN_RESET);
	IIC_SCL(GPIO_PIN_RESET);//���������SCL�ߣ���ô���ݴ�������е�SDA�ĵ�ƽ�任�ͻᱻ��Ϊ����ʼ�ͽ����ź�
}

/*
 * @brief IICֹͣ�ź�
 */
void IIC_Stop(void)
{
	IIC_SDA(GPIO_PIN_RESET);//������SDA������SCL����ֹ���ִ����ź�
	IIC_SCL(GPIO_PIN_SET);
	i2c_Delay();//��ʱ��֤SCL�ߵ�ƽʱ��SDA��������
	IIC_SDA(GPIO_PIN_SET);
	/*����Ͳ�������SCL�ˣ���ΪIICͨѶ�Ѿ�����*/
}

/*
 * @brief IIC�����ȴ�Ӧ��
 */
void IIC_Wait_Ack(void)
{
	//һ��Ҫ������SDA��������SCL�����߾Ϳ��ܱ����ʼ�źźͽ����ź���
	IIC_SDA(GPIO_PIN_SET);
	IIC_SCL(GPIO_PIN_SET);
	while(IIC_Read_SDA());
	IIC_SCL(GPIO_PIN_RESET);//IICͨѶû����������SCL��
}


/*
 * @brief IICд�ֽ�
 * @para  ��д����ֽ�����
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
	IIC_SCL(GPIO_PIN_RESET);//ͨѶδ����������SCL
}
uint8_t IIC_Read_Byte(uint8_t ack)
{
    uint8_t i, receive = 0;
    IIC_SDA(GPIO_PIN_SET); // SDAΪ����
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
    // ����ACK��NACK
    if(ack)
        IIC_SDA(GPIO_PIN_RESET);
    else
        IIC_SDA(GPIO_PIN_SET);
    IIC_SCL(GPIO_PIN_SET);
    i2c_Delay();
    IIC_SCL(GPIO_PIN_RESET);
    IIC_SDA(GPIO_PIN_SET); // �ָ�SDA
    return receive;
}
void check_device(uint8_t addr)
{
	IIC_Start();
	IIC_Write_Byte(addr | write_bit);
	IIC_Wait_Ack();
}