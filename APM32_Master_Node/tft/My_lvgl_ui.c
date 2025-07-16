#include "my_lvgl_ui.h"
#include "lcd.h"
#include "string.h"
#define COLOR_BG      0x0000   // 黑色
#define COLOR_TITLE   0x2980   // 蓝色
#define COLOR_CARD    0x27E0   // 绿色
#define COLOR_CARD_GRAY 0x4208 // 深灰
#define COLOR_WHITE   0xFFFF
#define COLOR_BLACK   0x0000
#define COLOR_GREEN   0x07E0
#define COLOR_RED     0xF800
// 画一个圆角矩形
void LCD_FillRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint8_t r, uint16_t color)
{
    // 填充中间矩形
    LCD_Fill(x1 + r, y1, x2 - r, y2, color); // 横向中间
    LCD_Fill(x1, y1 + r, x2, y2 - r, color); // 纵向中间

    // 填充四个圆角
    gui_fill_circle(x1 + r, y1 + r, r, color); // 左上
    gui_fill_circle(x2 - r, y1 + r, r, color); // 右上
    gui_fill_circle(x1 + r, y2 - r, r, color); // 左下
    gui_fill_circle(x2 - r, y2 - r, r, color); // 右下
}
void draw_main_ui(void)
{
    // 1. 清屏为黑色
    LCD_Fill(0, 0, 319, 239, COLOR_BG);

    // 2. 标题栏
    LCD_Fill(10, 10, 309, 42, COLOR_TITLE);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_TITLE;
    Show_Str(60, 18, (uint8_t*)"电动工具充电站 v1.0", 24, 1);

    // 3. 主站状态栏
    LCD_Fill(10, 52, 309, 92, COLOR_BLACK);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_BLACK;
    Show_Str(120, 58, (uint8_t*)"主站状态", 24, 1);

    // 绿色圆点和“正常运行”
    POINT_COLOR = COLOR_GREEN;
    Show_Str(80, 82, (uint8_t*)"● 正常运行", 16, 1);

    // 4. 子站卡片参数
    int card_w = 140, card_h = 54, card_x_gap = 16, card_y_gap = 12, r = 10;
    int start_x = 20, start_y = 104;

    // 子站1
    LCD_FillRoundRect(start_x, start_y, start_x+card_w, start_y+card_h, r, COLOR_CARD);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_CARD;
    Show_Str(start_x+10, start_y+6, (uint8_t*)"子站 01", 16, 1);
    Show_Str(start_x+10, start_y+26, (uint8_t*)"充电中 2.5A", 16, 1);
    Show_Str(start_x+10, start_y+42, (uint8_t*)"35.5V | 90W", 16, 1);

    // 子站2
    LCD_FillRoundRect(start_x+card_w+card_x_gap, start_y, start_x+2*card_w+card_x_gap, start_y+card_h, r, COLOR_CARD_GRAY);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_CARD_GRAY;
    Show_Str(start_x+card_w+card_x_gap+10, start_y+6, (uint8_t*)"子站 02", 16, 1);
    Show_Str(start_x+card_w+card_x_gap+10, start_y+26, (uint8_t*)"空闲", 16, 1);
    POINT_COLOR = COLOR_RED;
    Show_Str(start_x+card_w+card_x_gap+10, start_y+42, (uint8_t*)"○ 请连接电池", 16, 1);

    // 子站3
    LCD_FillRoundRect(start_x, start_y+card_h+card_y_gap, start_x+card_w, start_y+2*card_h+card_y_gap, r, COLOR_CARD);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_CARD;
    Show_Str(start_x+10, start_y+card_h+card_y_gap+6, (uint8_t*)"子站 03", 16, 1);
    Show_Str(start_x+10, start_y+card_h+card_y_gap+26, (uint8_t*)"充电中 2A", 16, 1);
    Show_Str(start_x+10, start_y+card_h+card_y_gap+42, (uint8_t*)"43.0V | 25W", 16, 1);

    // 子站4
    LCD_FillRoundRect(start_x+card_w+card_x_gap, start_y+card_h+card_y_gap, start_x+2*card_w+card_x_gap, start_y+2*card_h+card_y_gap, r, COLOR_CARD);
    POINT_COLOR = COLOR_WHITE;
    BACK_COLOR = COLOR_CARD;
    Show_Str(start_x+card_w+card_x_gap+10, start_y+card_h+card_y_gap+6, (uint8_t*)"子站 04", 16, 1);
    Show_Str(start_x+card_w+card_x_gap+10, start_y+card_h+card_y_gap+26, (uint8_t*)"充电中 2A", 16, 1);
    Show_Str(start_x+card_w+card_x_gap+10, start_y+card_h+card_y_gap+42, (uint8_t*)"0.6V | 25W", 16, 1);
}