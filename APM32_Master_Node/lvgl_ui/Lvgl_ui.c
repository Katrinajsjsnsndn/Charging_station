#include "lvgl.h"
#include "Lvgl_ui.h"

#ifdef LVGL_UI
static lv_obj_t *cards[4];
uint8_t selected_card = 0;

void lvgl_update_card_highlight(uint8_t selected)
{
    for (int i = 0; i < 4; i++) {
        if (i == selected) {
            lv_obj_set_style_border_color(cards[i], lv_color_hex(0xff0000), 0); // 红色高亮
            lv_obj_set_style_border_width(cards[i], 4, 0);
        } else {
            lv_obj_set_style_border_color(cards[i], lv_color_hex(0x2195f6), 0); // 默认蓝色
            lv_obj_set_style_border_width(cards[i], 2, 0);
        }
    }
}

void lvgl_main_screen_create(void)
{
    lv_obj_t *scr = lv_scr_act();

    // Station card 1
    cards[0] = lv_obj_create(scr);
    lv_obj_set_pos(cards[0], 7, 66);
    lv_obj_set_size(cards[0], 125, 75);
    lv_obj_set_style_border_color(cards[0], lv_color_hex(0x2195f6), 0);
    lv_obj_set_style_bg_color(cards[0], lv_color_hex(0x62e8b3), 0);
    lv_obj_set_style_bg_opa(cards[0], LV_OPA_COVER, 0);
    lv_obj_set_style_radius(cards[0], 0, 0);

    lv_obj_t *label1 = lv_label_create(cards[0]);
    lv_label_set_text(label1, "Station 01\nCharging 2.5A\n36.2V | 90W");
    lv_obj_set_style_text_color(label1, lv_color_black(), 0);
    lv_obj_align(label1, LV_ALIGN_CENTER, 0, 0);

    // Station card 2
    cards[1] = lv_obj_create(scr);
    lv_obj_set_pos(cards[1], 183, 66);
    lv_obj_set_size(cards[1], 125, 75);
    lv_obj_set_style_border_color(cards[1], lv_color_hex(0x2195f6), 0);
    lv_obj_set_style_bg_color(cards[1], lv_color_hex(0x6fe862), 0);
    lv_obj_set_style_bg_opa(cards[1], LV_OPA_COVER, 0);
    lv_obj_set_style_radius(cards[1], 0, 0);

    lv_obj_t *label2 = lv_label_create(cards[1]);
    lv_label_set_text(label2, "Station 02\nIdle\n○ Please connect battery");
    lv_obj_set_style_text_color(label2, lv_color_black(), 0);
    lv_obj_align(label2, LV_ALIGN_CENTER, 0, 0);

    // Station card 3
    cards[2] = lv_obj_create(scr);
    lv_obj_set_pos(cards[2], 7, 156);
    lv_obj_set_size(cards[2], 125, 75);
    lv_obj_set_style_border_color(cards[2], lv_color_hex(0x58595b), 0);
    lv_obj_set_style_bg_color(cards[2], lv_color_hex(0x545050), 0);
    lv_obj_set_style_bg_opa(cards[2], LV_OPA_COVER, 0);
    lv_obj_set_style_radius(cards[2], 0, 0);

    lv_obj_t *label3 = lv_label_create(cards[2]);
    lv_label_set_text(label3, "Station 03\nCharging 3A\n42.1V | 126W");
    lv_obj_set_style_text_color(label3, lv_color_white(), 0);
    lv_obj_align(label3, LV_ALIGN_CENTER, 0, 0);

    // Station card 4
    cards[3] = lv_obj_create(scr);
    lv_obj_set_pos(cards[3], 183, 156);
    lv_obj_set_size(cards[3], 125, 75);
    lv_obj_set_style_border_color(cards[3], lv_color_hex(0x2195f6), 0);
    lv_obj_set_style_bg_color(cards[3], lv_color_hex(0x645f5f), 0);
    lv_obj_set_style_bg_opa(cards[3], LV_OPA_COVER, 0);
    lv_obj_set_style_radius(cards[3], 0, 0);

    lv_obj_t *label4 = lv_label_create(cards[3]);
    lv_label_set_text(label4, "Station 04\nIdle\n● Battery Connected");
    lv_obj_set_style_text_color(label4, lv_color_white(), 0);
    lv_obj_align(label4, LV_ALIGN_CENTER, 0, 0);

    // Top status
    lv_obj_t *label_status = lv_label_create(scr);
    lv_label_set_text(label_status, "Main Status\nOK Running");
    lv_obj_set_style_text_color(label_status, lv_color_black(), 0);
    lv_obj_set_pos(label_status, 101, 10);
    lv_obj_set_size(label_status, 100, 32);

    // 初始高亮
    lvgl_update_card_highlight(selected_card);
}

// 详情页UI
static lv_obj_t *detail_screen = NULL;
void lvgl_detail_screen_create(const char *station_name, const char *status, float voltage, float current, int power)
{
    detail_screen = lv_obj_create(NULL);
    lv_obj_set_size(detail_screen, 320, 240);
    lv_obj_set_style_bg_color(detail_screen, lv_color_hex(0x222222), 0);

    // 顶部标题
    lv_obj_t *label_title = lv_label_create(detail_screen);
    lv_label_set_text_fmt(label_title, "%s Detail", station_name);
    lv_obj_set_style_text_color(label_title, lv_color_white(), 0);
    lv_obj_align(label_title, LV_ALIGN_TOP_MID, 0, 10);

    // 状态
    lv_obj_t *label_status = lv_label_create(detail_screen);
    lv_label_set_text_fmt(label_status, "Status: %s", status);
    lv_obj_set_style_text_color(label_status, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_align(label_status, LV_ALIGN_TOP_LEFT, 20, 50);

    // 电压
    lv_obj_t *label_voltage = lv_label_create(detail_screen);
    lv_label_set_text_fmt(label_voltage, "Voltage: %.2f V", voltage);
    lv_obj_set_style_text_color(label_voltage, lv_color_white(), 0);
    lv_obj_align(label_voltage, LV_ALIGN_TOP_LEFT, 20, 80);

    // 电流
    lv_obj_t *label_current = lv_label_create(detail_screen);
    lv_label_set_text_fmt(label_current, "Current: %.2f A", current);
    lv_obj_set_style_text_color(label_current, lv_color_white(), 0);
    lv_obj_align(label_current, LV_ALIGN_TOP_LEFT, 20, 110);

    // 功率
    lv_obj_t *label_power = lv_label_create(detail_screen);
    lv_label_set_text_fmt(label_power, "Power: %d W", power);
    lv_obj_set_style_text_color(label_power, lv_color_white(), 0);
    lv_obj_align(label_power, LV_ALIGN_TOP_LEFT, 20, 140);

    // 返回提示
    lv_obj_t *label_back = lv_label_create(detail_screen);
    lv_label_set_text(label_back, "Press RETURN to go back");
    lv_obj_set_style_text_color(label_back, lv_palette_main(LV_PALETTE_GREY), 0);
    lv_obj_align(label_back, LV_ALIGN_BOTTOM_MID, 0, -10);

    lv_scr_load(detail_screen);
}
#endif