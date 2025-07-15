#ifndef __TEST_CODE_H
#define __TEST_CODE_H
#include "main.h"

void rtp_test(void);						//µç×è´¥ÃşÆÁ²âÊÔº¯Êı
void ctp_test(void);						//µçÈİ´¥ÃşÆÁ²âÊÔº¯Êı
void DrawTestPage(uint8_t *str);			//Ö÷¹Ì¶¨À¸
void Chinese_Font_test(void); 	//ÖĞÎÄ²âÊÔ
void main_test(uint8_t *id);					//²âÊÔÖ÷Ò³
void FillRec_Test(void);				//Í¼ĞÎ²âÊÔ
void English_Font_test(void);		//Ó¢ÎÄ²âÊÔ
void Pic_test(void);						//Í¼Æ¬²âÊÔ
void Rotate_Test(void);					//Ğı×ª²âÊÔ
void Color_Test(void);					//´¿É«²âÊÔ
void Switch_test(void);					//ÏÔÊ¾¿ª¹Ø²âÊÔ
void Read_Test(void);						//¶ÁÑÕÉ«²âÊÔ
void LCD_ShowNumber(uint16_t x, uint16_t y, int32_t val, uint8_t base, uint8_t size);


#endif

