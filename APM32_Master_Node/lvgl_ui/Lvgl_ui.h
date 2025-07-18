#ifndef __LVGL_UI_H
#define __LVGL_UI_H

void lvgl_main_screen_create(void);
void lvgl_update_card_highlight(uint8_t selected);
extern uint8_t selected_card;
void lvgl_detail_screen_create(const char *station_name, const char *status, float voltage, float current, int power);

#endif

