#include "my_lvgl_ui.h"
#include "lcd.h"
#include "string.h"
#define COLOR_BG      0x0000   // ��ɫ
#define COLOR_TITLE   0x2980   // ��ɫ
#define COLOR_CARD    0x27E0   // ��ɫ
#define COLOR_CARD_GRAY 0x4208 // ���
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000
#define COLOR_GREEN   0x07E0
#define COLOR_RED     0xF800
// ��һ��Բ�Ǿ���
void LCD_FillRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t r, uint16_t color)
{
    // ����м����
    LCD_Fill(x1 + r, y1, x2 - r, y2, color); // �����м�
    LCD_Fill(x1, y1 + r, x2, y2 - r, color); // �����м�

    // ����ĸ�Բ��
    gui_fill_circle(x1 + r, y1 + r, r, color); // ����
    gui_fill_circle(x2 - r, y1 + r, r, color); // ����
    gui_fill_circle(x1 + r, y2 - r, r, color); // ����
    gui_fill_circle(x2 - r, y2 - r, r, color); // ����
}
void draw_main_ui(void)
{
    // 1. ����Ϊ��ɫ
    LCD_Fill(0, 0, 319, 239, COLOR_BG);

    // 2. ������
    LCD_Fill(10, 10, 309, 42, COLOR_TITLE);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_TITLE;
    Show_Str(60, 18, (uint8_t*)"�綯���߳��վ v1.0", 24, 1);

    // 3. ��վ״̬��
    LCD_Fill(10, 52, 309, 92, COLOR_BLACK);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_BLACK;
    Show_Str(120, 58, (uint8_t*)"��վ״̬", 24, 1);

    // ��ɫԲ��͡��������С�
    POINT_COLOR = COLOR_GREEN;
    Show_Str(80, 82, (uint8_t*)"�� ��������", 16, 1);

    // 4. ��վ��Ƭ����
    int card_w = 140, card_h = 54, card_x_gap = 16, card_y_gap = 12, r = 10;
    int start_x = 20, start_y = 104;

    // ��վ1
    LCD_FillRoundRect(start_x, start_y, start_x+card_w, start_y+card_h, r, COLOR_CARD);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_CARD;
    Show_Str(start_x+10, start_y+6, (uint8_t*)"��վ 01", 16, 1);
    Show_Str(start_x+10, start_y+26, (uint8_t*)"����� 2.5A", 16, 1);
    Show_Str(start_x+10, start_y+42, (uint8_t*)"35.5V | 90W", 16, 1);

    // ��վ2
    LCD_FillRoundRect(start_x+card_w+card_x_gap, start_y, start_x+2*card_w+card_x_gap, start_y+card_h, r, COLOR_CARD_GRAY);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_CARD_GRAY;
    Show_Str(start_x+card_w+card_x_gap+10, start_y+6, (uint8_t*)"��վ 02", 16, 1);
    Show_Str(start_x+card_w+card_x_gap+10, start_y+26, (uint8_t*)"����", 16, 1);
    POINT_COLOR = COLOR_RED;
    Show_Str(start_x+card_w+card_x_gap+10, start_y+42, (uint8_t*)"�� �����ӵ��", 16, 1);

    // ��վ3
    LCD_FillRoundRect(start_x, start_y+card_h+card_y_gap, start_x+card_w, start_y+2*card_h+card_y_gap, r, COLOR_CARD);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_CARD;
    Show_Str(start_x+10, start_y+card_h+card_y_gap+6, (uint8_t*)"��վ 03", 16, 1);
    Show_Str(start_x+10, start_y+card_h+card_y_gap+26, (uint8_t*)"����� 2A", 16, 1);
    Show_Str(start_x+10, start_y+card_h+card_y_gap+42, (uint8_t*)"43.0V | 25W", 16, 1);

    // ��վ4
    LCD_FillRoundRect(start_x+card_w+card_x_gap, start_y+card_h+card_y_gap, start_x+2*card_w+card_x_gap, start_y+2*card_h+card_y_gap, r, COLOR_CARD);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_CARD;
    Show_Str(start_x+card_w+card_x_gap+10, start_y+card_h+card_y_gap+6, (uint8_t*)"��վ 04", 16, 1);
    Show_Str(start_x+card_w+card_x_gap+10, start_y+card_h+card_y_gap+26, (uint8_t*)"����� 2A", 16, 1);
    Show_Str(start_x+card_w+card_x_gap+10, start_y+card_h+card_y_gap+42, (uint8_t*)"0.6V | 25W", 16, 1);
}