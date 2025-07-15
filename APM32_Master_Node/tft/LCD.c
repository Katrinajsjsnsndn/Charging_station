#include "lcd.h"
#include "stdlib.h"
#include "font.h" 
#include "usart.h"
#include "string.h"
#include "gpio.h"

//LCD�Ļ�����ɫ�ͱ���ɫ	   
uint16_t POINT_COLOR=0x0000;	//������ɫ
uint16_t BACK_COLOR=0xFFFF;  //����ɫ 

//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;
	 					    
//д�Ĵ�������
//data:�Ĵ���ֵ
void delay_ms(uint16_t nms)
{	 		  	  
	HAL_Delay(nms);
} 
void LCD_WR_REG(uint16_t data)
{
		uint8_t i;	

		HAL_GPIO_WritePin(SPI_CS_GPIO_PORT,SPI_CS_PIN,0);//SPI_CS = 0;		
		HAL_GPIO_WritePin(SPI_DC_GPIO_PORT,SPI_DC_PIN,0);//SPI_DC = 0;//RS=0 ����
		
		for(i=0; i<8; i++)
		{
			if (data & 0x80)
			 HAL_GPIO_WritePin(SPI_SDI_GPIO_PORT,SPI_SDI_PIN,1);//SPI_SDI = 1;
			else
			 HAL_GPIO_WritePin(SPI_SDI_GPIO_PORT,SPI_SDI_PIN,0);//SPI_SDI = 0;
			
			data <<= 1;		 
			HAL_GPIO_WritePin(SPI_SCK_GPIO_PORT,SPI_SCK_PIN,0);//SPI_SCK = 0;		  
			HAL_GPIO_WritePin(SPI_SCK_GPIO_PORT,SPI_SCK_PIN,1);//SPI_SCK = 1;
		}
		HAL_GPIO_WritePin(SPI_CS_GPIO_PORT,SPI_CS_PIN,1);//SPI_CS=1;				
}
//д���ݺ���
//data:�Ĵ���ֵ
void LCD_WR_DATA(uint16_t data)
{
		uint8_t i;
	
		HAL_GPIO_WritePin(SPI_CS_GPIO_PORT,SPI_CS_PIN,0);//SPI_CS = 0;		
		HAL_GPIO_WritePin(SPI_DC_GPIO_PORT,SPI_DC_PIN,1);//SPI_DC = 1;//RS=1 ����
		
		for(i=0; i<8; i++)
		{
			if (data & 0x80)
			 		HAL_GPIO_WritePin(SPI_SDI_GPIO_PORT,SPI_SDI_PIN,1);//SPI_SDI = 1;
			else
			  HAL_GPIO_WritePin(SPI_SDI_GPIO_PORT,SPI_SDI_PIN,0);//SPI_SDI = 0;
			
			data <<= 1;		 
			HAL_GPIO_WritePin(SPI_SCK_GPIO_PORT,SPI_SCK_PIN,0);//SPI_SCK = 0;		  
			HAL_GPIO_WritePin(SPI_SCK_GPIO_PORT,SPI_SCK_PIN,1);//SPI_SCK = 1;
		}
		HAL_GPIO_WritePin(SPI_CS_GPIO_PORT,SPI_CS_PIN,1);//SPI_CS=1;			
}

//д�Ĵ���
//LCD_Reg:�Ĵ������
//LCD_RegValue:Ҫд���ֵ
void LCD_WriteReg(uint16_t LCD_Reg,uint16_t LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WriteRAM(LCD_RegValue);	    		 
}   

//��ʼдGRAM
void LCD_WriteRAM_Prepare(void)
{
	LCD_WR_REG(lcddev.wramcmd);
} 
//LCDдGRAM
//RGB_Code:��ɫֵ
void LCD_WriteRAM(uint16_t RGB_Code)
{							    
//дʮ��λGRAM
  uint8_t i;
	HAL_GPIO_WritePin(SPI_CS_GPIO_PORT,SPI_CS_PIN,0);//SPI_CS = 0;
	HAL_GPIO_WritePin(SPI_DC_GPIO_PORT,SPI_DC_PIN,1);//SPI_DC = 1;
	
	for(i=0; i<16; i++)
	{
		if (RGB_Code & 0x8000)
		 HAL_GPIO_WritePin(SPI_SDI_GPIO_PORT,SPI_SDI_PIN,1);//SPI_SDI = 1;
		else
		 HAL_GPIO_WritePin(SPI_SDI_GPIO_PORT,SPI_SDI_PIN,0);//SPI_SDI = 0;
		
		RGB_Code <<= 1;		 
			HAL_GPIO_WritePin(SPI_SCK_GPIO_PORT,SPI_SCK_PIN,0);//SPI_SCK = 0;		  
			HAL_GPIO_WritePin(SPI_SCK_GPIO_PORT,SPI_SCK_PIN,1);//SPI_SCK = 1;
	} 
		HAL_GPIO_WritePin(SPI_CS_GPIO_PORT,SPI_CS_PIN,1);//SPI_CS=1;			
}

//��mdk -O1ʱ���Ż�ʱ��Ҫ����
//��ʱi
void opt_delay(uint8_t i)
{
	while(i--);
}

//LCD������ʾ
void LCD_DisplayOn(void)
{					   
	LCD_WR_REG(0X29);	//������ʾ
}	 
//LCD�ر���ʾ
void LCD_DisplayOff(void)
{	   
	LCD_WR_REG(0X28);	//�ر���ʾ
}   
//���ù��λ��
//Xpos:������
//Ypos:������
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{	 	    
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(Xpos>>8);LCD_WR_DATA(Xpos&0XFF); 			 
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(Ypos>>8);LCD_WR_DATA(Ypos&0XFF); 		 
} 		 
   
//����
//x,y:����
//POINT_COLOR:�˵����ɫ
void LCD_DrawPoint(uint16_t x,uint16_t y)
{
	LCD_SetCursor(x,y);		//���ù��λ�� 
	LCD_WriteRAM_Prepare();	//��ʼд��GRAM
	LCD_WriteRAM(POINT_COLOR); 
}	 
//���ٻ���
//x,y:����
//color:��ɫ
void LCD_Fast_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{	   
		//���ù��λ��
		LCD_SetCursor(x,y); 	 
		//д����ɫ
		LCD_WriteReg(lcddev.wramcmd,color);
}


//dir:����ѡ�� 	0-0����ת��1-180����ת��2-270����ת��3-90����ת
void LCD_Display_Dir(uint8_t dir)
{
	if(dir==0||dir==1)			//����
	{
			lcddev.dir=0;	//����
			lcddev.width=240;
			lcddev.height=320;

			lcddev.wramcmd=0X2C;
			lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;
		
		if(dir==0)        //0-0����ת
		{
			LCD_WR_REG(0x36); 
			LCD_WR_DATA((0<<3)|(0<<7)|(0<<6)|(0<<5));
		}else							//1-180����ת
		{
			LCD_WR_REG(0x36); 
			LCD_WR_DATA((0<<3)|(1<<7)|(1<<6)|(0<<5));		
		}
		
	}else if(dir==2||dir==3)
	{
		
			lcddev.dir=1;	//����
			lcddev.width=320;
			lcddev.height=240;

			lcddev.wramcmd=0X2C;
			lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B; 

				if(dir==2)				//2-270����ת
				{
					LCD_WR_REG(0x36); 
					LCD_WR_DATA((0<<3)|(1<<7)|(0<<6)|(1<<5));

				}else							//3-90����ת
				{
					LCD_WR_REG(0x36); 
					LCD_WR_DATA((0<<3)|(0<<7)|(1<<6)|(1<<5));
				}		
	}	


		//������ʾ����	
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);LCD_WR_DATA((lcddev.width-1)&0XFF);
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
	
	
}	 
//���ô���,���Զ����û������굽�������Ͻ�(sx,sy).
//sx,sy:������ʼ����(���Ͻ�)
//width,height:���ڿ�Ⱥ͸߶�,�������0!!
//�����С:width*height. 
void LCD_Set_Window(uint16_t sx,uint16_t sy,uint16_t width,uint16_t height)
{    
	uint16_t twidth,theight;
	twidth=sx+width-1;
	theight=sy+height-1;

		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(sx>>8); 
		LCD_WR_DATA(sx&0XFF);	 
		LCD_WR_DATA(twidth>>8); 
		LCD_WR_DATA(twidth&0XFF);  
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(sy>>8); 
		LCD_WR_DATA(sy&0XFF); 
		LCD_WR_DATA(theight>>8); 
		LCD_WR_DATA(theight&0XFF); 

}
//��ʼ��lcd
void LCD_Init(void)
{

//	RCC->APB2ENR|=1<<2;//��ʹ������PORTAʱ��
// 	RCC->APB2ENR|=1<<3;//��ʹ������PORTBʱ��
// 	RCC->APB2ENR|=1<<4;//��ʹ������PORTCʱ��

//	RCC->APB2ENR|=1<<0;    //��������ʱ��
//	JTAG_Set(SWD_ENABLE);  //����SWD
//	
//	GPIOA->CRL&=0XFFFFF0FF;
//	GPIOA->CRL|=0X00000300;//LED(���⿪��) = PA2
//	GPIOA->CRH&=0X0FF00FFF;
//	GPIOA->CRH|=0X30038000;//SPI_SDO = PA11  CS = PA12   DC = PA15
//	GPIOA->ODR|=0X9804;    //IO����
//	
//	GPIOC->CRH&=0X000FFFFF;
//	GPIOC->CRH|=0X33300000;//SDI = PC13  SCK = PC14  RST = PC15 	
//	GPIOC->ODR|=0XE000;    //IO����
//	

	
	HAL_GPIO_WritePin(SPI_RST_GPIO_PORT,SPI_RST_PIN,1);//SPI_RST=1;
	delay_ms(1);
	HAL_GPIO_WritePin(SPI_RST_GPIO_PORT,SPI_RST_PIN,0);
	delay_ms(10);
	HAL_GPIO_WritePin(SPI_RST_GPIO_PORT,SPI_RST_PIN,1);//SPI_RST=1;
	delay_ms(120);  


//************* Start Initial Sequence **********//
LCD_WR_REG(0x11);
delay_ms(120); //Delay 120ms
//--------display and color format setting-----------//
LCD_WR_REG(0x36);
LCD_WR_DATA(0x00);
LCD_WR_REG(0x3a);
LCD_WR_DATA(0x05);
//--------ST7789V Frame rate setting------------//
LCD_WR_REG(0xb2);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x0c);
LCD_WR_DATA(0x00);
LCD_WR_DATA(0x33);
LCD_WR_DATA(0x33);
LCD_WR_REG(0xb7);
LCD_WR_DATA(0x35);
//-----------ST7789V Power setting---------------//
LCD_WR_REG(0xbb);
LCD_WR_DATA(0x28);
LCD_WR_REG(0xc0);
LCD_WR_DATA(0x2c);
LCD_WR_REG(0xc2);
LCD_WR_DATA(0x01);
LCD_WR_REG(0xc3);
LCD_WR_DATA(0x0b);
LCD_WR_REG(0xc4);
LCD_WR_DATA(0x20);
LCD_WR_REG(0xc6);
LCD_WR_DATA(0x0f);
LCD_WR_REG(0xd0);
LCD_WR_DATA(0xa4);
LCD_WR_DATA(0xa1);
//------------ST7789V gamma setting-------------//
LCD_WR_REG(0xe0);
LCD_WR_DATA(0xd0);
LCD_WR_DATA(0x01);
LCD_WR_DATA(0x08);
LCD_WR_DATA(0x0f);
LCD_WR_DATA(0x11);
LCD_WR_DATA(0x2a);
LCD_WR_DATA(0x36);
LCD_WR_DATA(0x55);
LCD_WR_DATA(0x44);
LCD_WR_DATA(0x3a);
LCD_WR_DATA(0x0b);
LCD_WR_DATA(0x06);
LCD_WR_DATA(0x11);
LCD_WR_DATA(0x20);
LCD_WR_REG(0xe1);
LCD_WR_DATA(0xd0);
LCD_WR_DATA(0x02);
LCD_WR_DATA(0x07);
LCD_WR_DATA(0x0a);
LCD_WR_DATA(0x0b);
LCD_WR_DATA(0x18);
LCD_WR_DATA(0x34);
LCD_WR_DATA(0x43);
LCD_WR_DATA(0x4a);
LCD_WR_DATA(0x2b);
LCD_WR_DATA(0x1b);
LCD_WR_DATA(0x1c);
LCD_WR_DATA(0x22);
LCD_WR_DATA(0x1f);
LCD_WR_REG(0x29);

} 



//��ȡ��ĳ�����ɫֵ	 
//x,y:����
////����ֵ:�˵����ɫ
//uint16_t LCD_ReadPoint(uint16_t x,uint16_t y)
//{
//	uint8_t i,r,g,b,reg=0x2e;
// 	uint16_t color;
//	if(x>=lcddev.width||y>=lcddev.height)return 0;	//�����˷�Χ,ֱ�ӷ���		   
//	LCD_SetCursor(x,y);
//	SPI_CS = 0;		
//	SPI_DC = 0;
//		
//		for(i=0; i<8; i++)
//		{
//			if (reg & 0x80)
//			 SPI_SDI = 1;
//			else
//			 SPI_SDI = 0;
//			
//			reg <<= 1;		 
//			SPI_SCK = 0;  
//			SPI_SCK = 1;	
//		}
//		
//		


//		for(i=0; i<8; i++)							//��һ�οն� �����ηֱ�ΪR G B
//		{
//			SPI_SCK = 0;		 	
//			SPI_SCK = 1;			
//		}


//		for(i=0; i<8; i++)
//		{
//			SPI_SCK = 0;		r=r << 1 | SPI_SDO; 	
//			SPI_SCK = 1;			
//		}



//		for(i=0; i<8; i++)
//		{
//			SPI_SCK = 0;		g=g << 1 | SPI_SDO; 	
//			SPI_SCK = 1;			
//		}


//		for(i=0; i<8; i++)
//		{
//			SPI_SCK = 0;		b=b << 1 | SPI_SDO; 	
//			SPI_SCK = 1;			
//		}		
//		
//		color = ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
//		
//		
//		SPI_CS=1;	

//		
//return color;
//}		
  
//��������
//color:Ҫ���������ɫ
void LCD_Clear(uint16_t color)
{
	uint32_t index=0;      
	uint32_t totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//�õ��ܵ���
	LCD_SetCursor(0x00,0x0000);	//���ù��λ�� 
	LCD_WriteRAM_Prepare();     		//��ʼд��GRAM	  	  
	for(index=0;index<totalpoint;index++)LCD_WriteRAM(color);
}  
//��ָ�����������ָ����ɫ
//�����С:(xend-xsta+1)*(yend-ysta+1)
//xsta
//color:Ҫ������ɫ
void LCD_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t color)
{          
	uint16_t i,j;
	uint16_t xlen=0;
	uint16_t temp;
	if((lcddev.id==0X6804)&&(lcddev.dir==1))	//6804������ʱ�����⴦��  
	{
		temp=sx;
		sx=sy;
		sy=lcddev.width-ex-1;	  
		ex=ey;
		ey=lcddev.width-temp-1;
 		lcddev.dir=0;	 
 		lcddev.setxcmd=0X2A;
		lcddev.setycmd=0X2B;  	 			
		LCD_Fill(sx,sy,ex,ey,color);  
 		lcddev.dir=1;	 
  		lcddev.setxcmd=0X2B;
		lcddev.setycmd=0X2A;  	 
 	}else
	{
		xlen=ex-sx+1;	 
		for(i=sy;i<=ey;i++)
		{
		 	LCD_SetCursor(sx,i);      				//���ù��λ�� 
			LCD_WriteRAM_Prepare();     			//��ʼд��GRAM	  
			for(j=0;j<xlen;j++)LCD_WriteRAM(color);	//���ù��λ�� 	    
		}
	}
}  
//��ָ�����������ָ����ɫ��			 
//(sx,sy),(ex,ey):�����ζԽ�����,�����СΪ:(ex-sx+1)*(ey-sy+1)   
//color:Ҫ������ɫ
void LCD_Color_Fill(uint16_t sx,uint16_t sy,uint16_t ex,uint16_t ey,uint16_t *color)
{  
	uint16_t height,width;
	uint16_t i,j;
	width=ex-sx+1; 			//�õ����Ŀ��
	height=ey-sy+1;			//�߶�
 	for(i=0;i<height;i++)
	{
 		LCD_SetCursor(sx,sy+i);   	//���ù��λ�� 
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM
		for(j=0;j<width;j++)LCD_WriteRAM(color[i*width+j]);//д������ 
	}  
}
//����
//x1,y1:�������
//x2,y2:�յ�����  
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    
//������	  
//(x1,y1),(x2,y2):���εĶԽ�����
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	LCD_DrawLine(x1,y1,x2,y1);
	LCD_DrawLine(x1,y1,x1,y2);
	LCD_DrawLine(x1,y2,x2,y2);
	LCD_DrawLine(x2,y1,x2,y2);
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		LCD_DrawPoint(x0+a,y0-b);             //5
 		LCD_DrawPoint(x0+b,y0-a);             //0           
		LCD_DrawPoint(x0+b,y0+a);             //4               
		LCD_DrawPoint(x0+a,y0+b);             //6 
		LCD_DrawPoint(x0-a,y0+b);             //1       
 		LCD_DrawPoint(x0-b,y0+a);             
		LCD_DrawPoint(x0-a,y0-b);             //2             
  	LCD_DrawPoint(x0-b,y0-a);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 									  
//��ָ��λ����ʾһ���ַ�
//x,y:��ʼ����
//num:Ҫ��ʾ���ַ�:" "--->"~"
//size:�����С 12/16/24
//mode:���ӷ�ʽ(1)���Ƿǵ��ӷ�ʽ(0)
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t size,uint8_t mode)
{  							  
    uint8_t temp,t1,t;
	uint16_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(t=0;t<csize;t++)
	{   
		if(size==12)temp=asc2_1206[num][t]; 	 	//����1206����
		else if(size==16)temp=asc2_1608[num][t];	//����1608����
		else if(size==24)temp=asc2_2412[num][t];	//����2412����
		else return;								//û�е��ֿ�
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)LCD_Fast_DrawPoint(x,y,POINT_COLOR);
			else if(mode==0)LCD_Fast_DrawPoint(x,y,BACK_COLOR);
			temp<<=1;
			y++;
			if(y>=lcddev.height)return;		//��������
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=lcddev.width)return;	//��������
				break;
			}
		}  	 
	}  	    	   	 	  
}   
//m^n����
//����ֵ:m^n�η�.
uint32_t LCD_Pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}			 
//��ʾ����,��λΪ0,����ʾ
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//color:��ɫ 
//num:��ֵ(0~4294967295);	 
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,' ',size,0);
				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,0); 
	}
} 
//��ʾ����,��λΪ0,������ʾ
//x,y:�������
//num:��ֵ(0~999999999);	 
//len:����(��Ҫ��ʾ��λ��)
//size:�����С
//mode:
//[7]:0,�����;1,���0.
//[6:1]:����
//[0]:0,�ǵ�����ʾ;1,������ʾ.
void LCD_ShowxNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len,uint8_t size,uint8_t mode)
{  
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				if(mode&0X80)LCD_ShowChar(x+(size/2)*t,y,'0',size,mode&0X01);  
				else LCD_ShowChar(x+(size/2)*t,y,' ',size,mode&0X01);  
 				continue;
			}else enshow=1; 
		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode&0X01); 
	}
} 
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(uint16_t x,uint16_t y,uint16_t width,uint16_t height,uint8_t size,uint8_t *p)
{
	uint8_t x0=x;
	width+=x;
	height+=y;
    while((*p<='~')&&(*p>=' '))//�ж��ǲ��ǷǷ��ַ�!
    {       
        if(x>=width){x=x0;y+=size;}
        if(y>=height)break;//�˳�
        LCD_ShowChar(x,y,*p,size,0);
        x+=size/2;
        p++;
    }
}


//���� 16*16
void GUI_DrawFont16(uint16_t x, uint16_t y,uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//�Զ�ͳ�ƺ�����Ŀ
	
			
	for (k=0;k<HZnum;k++) 
	{
	  if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
	  { 	LCD_Set_Window(x,y,16,16);
				LCD_WriteRAM_Prepare();
		    for(i=0;i<16*2;i++)
		    {
				for(j=0;j<8;j++)
		    	{	
					if(!mode) //�ǵ��ӷ�ʽ
					{
						if(tfont16[k].Msk[i]&(0x80>>j))	LCD_WriteRAM(POINT_COLOR);
						else LCD_WriteRAM(BACK_COLOR);
					}
					else
					{
						//POINT_COLOR=fc;
						if(tfont16[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//��һ����
						x++;
						if((x-x0)==16)
						{
							x=x0;
							y++;
							break;
						}
					}

				}
				
			}
			
			
		}				  	
		continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}

	LCD_Set_Window(0,0,lcddev.width,lcddev.height);//�ָ�����Ϊȫ��  
} 

//���� 24*24
void GUI_DrawFont24(uint16_t x, uint16_t y, uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont24)/sizeof(typFNT_GB24);	//�Զ�ͳ�ƺ�����Ŀ
		
			for (k=0;k<HZnum;k++) 
			{
			  if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
			  { 	LCD_Set_Window(x,y,24,24);
						LCD_WriteRAM_Prepare();
				    for(i=0;i<24*3;i++)
				    {
							for(j=0;j<8;j++)
							{
								if(!mode) //�ǵ��ӷ�ʽ
								{
									if(tfont24[k].Msk[i]&(0x80>>j))	LCD_WriteRAM(POINT_COLOR);
									else LCD_WriteRAM(BACK_COLOR);
								}
							else
							{
								//POINT_COLOR=fc;
								if(tfont24[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//��һ����
								x++;
								if((x-x0)==24)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
			}

	LCD_Set_Window(0,0,lcddev.width,lcddev.height);//�ָ�����Ϊȫ��  
}

//���� 32*32
void GUI_DrawFont32(uint16_t x, uint16_t y, uint8_t *s,uint8_t mode)
{
	uint8_t i,j;
	uint16_t k;
	uint16_t HZnum;
	uint16_t x0=x;
	HZnum=sizeof(tfont32)/sizeof(typFNT_GB32);	//�Զ�ͳ�ƺ�����Ŀ
	for (k=0;k<HZnum;k++) 
			{
			  if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
			  { 	LCD_Set_Window(x,y,32,32);
						LCD_WriteRAM_Prepare();
				    for(i=0;i<32*4;i++)
				    {
						for(j=0;j<8;j++)
				    	{
							if(!mode) //�ǵ��ӷ�ʽ
							{
								if(tfont32[k].Msk[i]&(0x80>>j))	LCD_WriteRAM(POINT_COLOR);
								else LCD_WriteRAM(BACK_COLOR);
							}
							else
							{
								//POINT_COLOR=fc;
								if(tfont32[k].Msk[i]&(0x80>>j))	LCD_DrawPoint(x,y);//��һ����
								x++;
								if((x-x0)==32)
								{
									x=x0;
									y++;
									break;
								}
							}
						}
					}
					
					
				}				  	
				continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
			}
	
	LCD_Set_Window(0,0,lcddev.width,lcddev.height);//�ָ�����Ϊȫ��  
} 



//��ʾ���ֻ����ַ���
void Show_Str(uint16_t x, uint16_t y,uint8_t *str,uint8_t size,uint8_t mode)
{			
	uint16_t x0=x;							  	  
  	uint8_t bHz=0;     //�ַ��������� 
    while(*str!=0)//����δ����
    { 
        if(!bHz)
        {
			if(x>(lcddev.width-size/2)||y>(lcddev.height-size)) 
			return; 
	        if(*str>0x80)bHz=1;//���� 
	        else              //�ַ�
	        {          
		        if(*str==0x0D)//���з���
		        {         
		            y+=size;
		            x=x0;
		            str++; 
		        }  
		        else
				{
					if(size>=24)//�ֿ���û�м���12X24 16X32��Ӣ������,��8X16����
					{ 						
					LCD_ShowChar(x,y,*str,24,mode);
					x+=12; //�ַ�,Ϊȫ�ֵ�һ�� 
					}
					else
					{
					LCD_ShowChar(x,y,*str,size,mode);
					x+=size/2; //�ַ�,Ϊȫ�ֵ�һ�� 
					}
				} 
				str++; 
		        
	        }
        }else//���� 
        {   
				if(x>(lcddev.width-size)||y>(lcddev.height-size)) 
				return;  
							bHz=0;//�к��ֿ�    
				if(size==32)
				GUI_DrawFont32(x,y,str,mode);	 	
				else if(size==24)
				GUI_DrawFont24(x,y,str,mode);	
				else
				GUI_DrawFont16(x,y,str,mode);
					
						str+=2; 
						x+=size;//��һ������ƫ��	    
        }						 
    }   
}


//��ʾ40*40ͼƬ
void Gui_Drawbmp16(uint16_t x,uint16_t y,const unsigned char *p) //��ʾ40*40ͼƬ
{
  	int i; 
	unsigned char picH,picL; 
	LCD_Set_Window(x,y,40,40);
	LCD_WriteRAM_Prepare();	
	
    for(i=0;i<40*40;i++)
	{	
	 	picL=*(p+i*2);	//���ݵ�λ��ǰ
		picH=*(p+i*2+1);				
		LCD_WriteRAM(picH<<8|picL);  						
	}	
	LCD_Set_Window(0,0,lcddev.width,lcddev.height);//�ָ���ʾ����Ϊȫ��	

}

//������ʾ
void Gui_StrCenter(uint16_t x, uint16_t y, uint8_t *str,uint8_t size,uint8_t mode)
{
	uint16_t x1;
	uint16_t len=strlen((const char *)str);
	if(size>16)
	{
		x1=(lcddev.width-len*(size/2))/2;
	}else
	{
		x1=(lcddev.width-len*8)/2;
	}
	
	Show_Str(x+x1,y,str,size,mode);
} 


void Load_Drow_Dialog(void)
{
	LCD_Clear(WHITE);//����   
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ
	BACK_COLOR=WHITE;
	LCD_ShowString(lcddev.width-24,0,200,16,16,"RST");//��ʾ��������
  POINT_COLOR=RED;//���û�����ɫ 
}
////////////////////////////////////////////////////////////////////////////////
//���ݴ�����ר�в���
//��ˮƽ��
//x0,y0:����
//len:�߳���
//color:��ɫ
void gui_draw_hline(uint16_t x0,uint16_t y0,uint16_t len,uint16_t color)
{
	if(len==0)return;
	LCD_Fill(x0,y0,x0+len-1,y0,color);	
}
//��ʵ��Բ
//x0,y0:����
//r:�뾶
//color:��ɫ
void gui_fill_circle(uint16_t x0,uint16_t y0,uint16_t r,uint16_t color)
{											  
	uint32_t i;
	uint32_t imax = ((uint32_t)r*707)/1000+1;
	uint32_t sqmax = (uint32_t)r*(uint32_t)r+(uint32_t)r/2;
	uint32_t x=r;
	gui_draw_hline(x0-r,y0,2*r,color);
	for (i=1;i<=imax;i++) 
	{
		if ((i*i+x*x)>sqmax)// draw lines from outside  
		{
 			if (x>imax) 
			{
				gui_draw_hline (x0-i+1,y0+x,2*(i-1),color);
				gui_draw_hline (x0-i+1,y0-x,2*(i-1),color);
			}
			x--;
		}
		// draw lines from inside (center)  
		gui_draw_hline(x0-x,y0+i,2*x,color);
		gui_draw_hline(x0-x,y0-i,2*x,color);
	}
}  

//��һ������
//(x1,y1),(x2,y2):��������ʼ����
//size�������Ĵ�ϸ�̶�
//color����������ɫ
void lcd_draw_bline(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2,uint8_t size,uint16_t color)
{
	uint16_t t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	if(x1<size|| x2<size||y1<size|| y2<size)return; 
	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		gui_fill_circle(uRow,uCol,size,color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
static uint8_t spi_read_byte(void)
{
    uint8_t i, data = 0;

    for (i = 0; i < 8; i++)
    {
        HAL_GPIO_WritePin(SPI_SCK_GPIO_PORT, SPI_SCK_PIN, GPIO_PIN_RESET);  // ��
        data <<= 1;
        if (HAL_GPIO_ReadPin(SPI_SDO_GPIO_PORT, SPI_SDO_PIN) == GPIO_PIN_SET)
            data |= 0x01;
        HAL_GPIO_WritePin(SPI_SCK_GPIO_PORT, SPI_SCK_PIN, GPIO_PIN_SET);    // �ߣ�����
    }
    return data;
}
uint16_t id_order=0x04;
uint32_t LCD_Read_ID(void)
{
    uint8_t id[3];
    uint32_t id32;

    /* ��ʼ�� SDO Ϊ���� */



 LCD_WR_REG(0x04);
		HAL_GPIO_WritePin(SPI_CS_GPIO_PORT,SPI_CS_PIN,0);//SPI_CS=1;				

    /* 2. �������ݽ׶Σ�DC = 1���������� 3 �ֽ� */
    HAL_GPIO_WritePin(SPI_DC_GPIO_PORT, SPI_DC_PIN, GPIO_PIN_SET);     // DC=1 ����

    id[0] = spi_read_byte();   /* dummy 0x00 */
    id[1] = spi_read_byte();   /* manufacturer ID��ͨ���� 0x85 */
    id[2] = spi_read_byte();   /* version/driver ID */

    HAL_GPIO_WritePin(SPI_CS_GPIO_PORT, SPI_CS_PIN, GPIO_PIN_SET);     // CS = 1

    id32 = ((uint32_t)id[0] << 16) | ((uint32_t)id[1] << 8) | id[2];
    return id32;
}