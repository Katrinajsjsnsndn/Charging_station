
#include "usart.h"		
#include "lcd.h"   
#include "pic.h"
#include "test code.h"

uint16_t ColorTab[5]={RED,GREEN,BLUE,YELLOW,BRED};//定义颜色数组
uint16_t ColornTab[8]={RED,MAGENTA,GREEN,DARKBLUE,BLUE,BLACK,LIGHTGREEN};


//主固定栏
void DrawTestPage(uint8_t *str)
{
	//绘制固定栏up
	LCD_Clear(WHITE);
	LCD_Fill(0,0,lcddev.width,20,BLUE);
	POINT_COLOR=GREEN;
	LCD_DrawLine(0,20,lcddev.width,20);
	POINT_COLOR=BLUE;
	LCD_DrawLine(0,21,lcddev.width,21);
	POINT_COLOR=BRRED;
	LCD_DrawLine(0,22,lcddev.width,22);
	//绘制固定栏down
	LCD_Fill(0,lcddev.height-20,lcddev.width,lcddev.height,BLUE);
	POINT_COLOR=GREEN;
	LCD_DrawLine(0,lcddev.height-20,lcddev.width,lcddev.height-20);
	POINT_COLOR=BLUE;
	LCD_DrawLine(0,lcddev.height-21,lcddev.width,lcddev.height-21);
	POINT_COLOR=BRRED;            
	LCD_DrawLine(0,lcddev.height-22,lcddev.width,lcddev.height-22);	
	POINT_COLOR=WHITE;
	Gui_StrCenter(0,2,str,16,1);//居中显示
	Gui_StrCenter(0,lcddev.height-18,"深圳市鸿讯电子有限公司",16,1);//居中显示
}

//中文测试
void Chinese_Font_test(void)
{	
	DrawTestPage("中文显示测试");
	
		POINT_COLOR=RED;	//画笔颜色
		BACK_COLOR=BLACK;  //背景色 
	
		Show_Str(10,30,"16X16:鸿讯电子",16,1);
		Show_Str(10,50,"16X16:TFT液晶显示屏",16,1);
		Show_Str(10,70,"24X24:中文显示",24,1);
		Show_Str(10,100,"32X32:测试例程",32,1);
		delay_ms(1500);
}

//测试主页
void main_test(uint8_t *id)
{
	DrawTestPage("显示屏测试程序");
	POINT_COLOR=RED;
	Gui_StrCenter(0,100,"显示屏",32,1);
	POINT_COLOR=GREEN;
	Gui_StrCenter(0,135,"测试程序",32,1);
	POINT_COLOR=BLUE;
	Gui_StrCenter(0,170,id,24,1);
	POINT_COLOR=BLACK;
	Gui_StrCenter(0,196,"240RGB*320",24,1);
	POINT_COLOR=BLACK;
	Gui_StrCenter(0,235,"TEL:13528896127",24,1);
	delay_ms(1800);			
}

//图形测试
void FillRec_Test(void)
{
	uint8_t i=0;
	DrawTestPage("图形测试");
	LCD_Fill(0,24,lcddev.width,lcddev.height-24,WHITE);
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_DrawRectangle(lcddev.width/2-25+(i*15),lcddev.height/2-25+(i*15),lcddev.width/2-25+(i*15)+60,lcddev.height/2-25+(i*15)+60); 
		delay_ms(100);
	}
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_Draw_Circle(50+(i*35),60,30);
		delay_ms(100);
	}
	delay_ms(1500);
	
	
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		LCD_Fill(lcddev.width/2-25+(i*15),lcddev.height/2-25+(i*15),lcddev.width/2-25+(i*15)+60,lcddev.height/2-25+(i*15)+60,POINT_COLOR); 
		delay_ms(100);
	}
	for (i=0; i<5; i++) 
	{
		POINT_COLOR=ColorTab[i];
		gui_fill_circle(50+(i*35),60,30,POINT_COLOR);
		delay_ms(100);
	}
	delay_ms(1500);
}

//英文测试
void English_Font_test(void)
{
	DrawTestPage("英文显示测试");
	POINT_COLOR=RED;
	BACK_COLOR=WHITE;
	LCD_ShowString(10,30,200,12,12,"6X12:abcdefghijklmnopqrstuvwxyz");
	LCD_ShowString(10,45,200,12,12,"6X12:ABCDEFGHIJKLMNOP0123456789");
	POINT_COLOR=GREEN;
	LCD_ShowString(10,60,200,16,16,"8X16:abcdefghijklmnopqrstuvwxyz");
	LCD_ShowString(10,80,200,16,16,"8X16:ABCDEFGHIJKLMNOP0123456789");
	POINT_COLOR=BLUE;
	LCD_ShowString(10,100,200,24,24,"12X24:abcdefghijklmnopqrstuvwxyz");
	LCD_ShowString(10,128,200,24,24,"12X24:ABCDEFGHIJKLMNOP0123456789");
	delay_ms(1500);
}

//图片测试
void Pic_test(void)
{
	DrawTestPage("图片显示测试");
	POINT_COLOR=BLUE;
	//LCD_Fill(0,20,lcddev.width,lcddev.height-20,WHITE);
	Gui_Drawbmp16(60,100,gImage_1);
	Gui_Drawbmp16(130,100,gImage_1);
	delay_ms(1500);
}

//旋转测试
void Rotate_Test(void)
{
	uint8_t num=0;
	uint8_t *Direction[4]={"Rotation:0","Rotation:180","Rotation:270","Rotation:90"};
	
		for(num=0;num<4;num++)
		{		
			LCD_Display_Dir(num);
			DrawTestPage("旋转测试");
			POINT_COLOR=BLUE;
			Show_Str(20,30,Direction[num],16,1);
			Gui_Drawbmp16(30,50,gImage_1);
			delay_ms(1000);delay_ms(1000);
		}

	LCD_Display_Dir(USE_LCM_DIR);	

}



//彩条显色
void DispBand(void)	 
{
	unsigned int i,j,k;
	//unsigned int color[8]={RED,RED,GREEN,GREEN,BLUE,BLUE,WHITE,WHITE};
	unsigned int color[8]={BLUE,GREEN,RED,GBLUE,BRED,YELLOW,BLACK,WHITE};
	LCD_Set_Window(0,0,lcddev.width,lcddev.height);
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	
    	
																								  
	for(i=0;i<8;i++)
	{
		for(j=0;j<lcddev.height/8;j++)
		{
	        for(k=0;k<lcddev.width;k++)
			{
				
				LCD_WriteRAM(color[i]);				
				
			} 
		}
	}
	for(j=0;j<lcddev.height%8;j++)
	{
        for(k=0;k<lcddev.width;k++)
		{
			
				LCD_WriteRAM(color[7]);			
			
		} 
	}
	

	delay_ms(1000);
}

//纯色测试
void Color_Test(void)
{
	DispBand();               
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str(20,30,"WHITE",16,1);delay_ms(1000);
	POINT_COLOR=WHITE;
	LCD_Clear(RED);
	Show_Str(20,30,"RED ",16,1);delay_ms(1000);
	LCD_Clear(GREEN);
	Show_Str(20,30,"GREEN ",16,1);delay_ms(1000);
	LCD_Clear(BLUE);
	Show_Str(20,30,"BLUE ",16,1);delay_ms(1500);
	
}

//显示开关测试
void Switch_test(void)
{
	uint8_t num=3;
	while(num--)
	{
	LCD_DisplayOff();
	delay_ms(400);
	LCD_DisplayOn();
	delay_ms(400);
	}
	
}

////读颜色 测试
//void Read_Test(void)
//{
//	uint16_t color;
//	uint8_t cbuf[20] = {0},i;

//	DrawTestPage("读颜色值测试");
//	LCD_Fill(0,23,lcddev.width,lcddev.height-23,WHITE);	
//	for (i=0; i<7; i++) 
//	{
//		POINT_COLOR=ColornTab[i];
//		LCD_Fill(40-10,55+i*25-10,40+10,55+i*25+10,POINT_COLOR);
//		color = LCD_ReadPoint(40,55+i*25);
//		sprintf((char *)cbuf,"read color:0x%04x",color);
//		Show_Str(40+20,55+i*25-8,cbuf,16,1);
//	}
//	delay_ms(1500);
//}

/* 把整数转成字符串，返回指针指向静态缓冲区 */
char* NumToStr(int32_t val, uint8_t base)
{
    static char buf[35];        // 32 位二进制最长 32+0b+'\0'=35
    char *p = buf + sizeof(buf) - 1;
    uint8_t neg = 0;

    *p = '\0';

    /* 十进制负号处理 */
    if (base == 10 && val < 0)
    {
        neg = 1;
        val = -val;
    }

    /* 进制转换 */
    switch (base)
    {
    case 10:                               /* 十进制 */
        do { *--p = val % 10 + '0'; } while (val /= 10);
        if (neg) *--p = '-';
        break;

    case 16:                               /* 十六进制，大写 */
        *--p = 'x';
        *--p = '0';
        do {
            uint8_t n = val & 0x0F;
            *--p = (n > 9) ? (n - 10 + 'A') : (n + '0');
        } while (val >>= 4);
        break;

    case 2:                                /* 二进制 */
        *--p = 'b';
        *--p = '0';
        do { *--p = (val & 1) + '0'; } while (val >>= 1);
        break;

    default:
        return (char*)"ERR";
    }
    return p;
}
/* 在 (x,y) 处显示整数，base=10/16/2 */
void LCD_ShowNumber(uint16_t x, uint16_t y, int32_t val, uint8_t base, uint8_t size)
{
    LCD_ShowString(x, y, 320, 240, size, (uint8_t*)NumToStr(val, base));
}