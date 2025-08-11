/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include <stdio.h>
#include "gui_guider.h"
#include "events_init.h"
#include "widgets_init.h"
#include "custom.h"



void setup_scr_screen_control(lv_ui *ui)
{
    //Write codes screen_control
    ui->screen_control = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_control, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_control, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_control, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control, lv_color_hex(0x010000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_1
    ui->screen_control_label_1 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_1, "快速充电");
    lv_label_set_long_mode(ui->screen_control_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_1, 109, 11);
    lv_obj_set_size(ui->screen_control_label_1, 100, 32);

    //Write style for screen_control_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_1, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_bar_1
    ui->screen_control_bar_1 = lv_bar_create(ui->screen_control);
    lv_obj_set_style_anim_time(ui->screen_control_bar_1, 1000, 0);
    lv_bar_set_mode(ui->screen_control_bar_1, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_control_bar_1, 0, 100);
    lv_bar_set_value(ui->screen_control_bar_1, 50, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_control_bar_1, 24, 43);
    lv_obj_set_size(ui->screen_control_bar_1, 279, 12);

    //Write style for screen_control_bar_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control_bar_1, 178, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control_bar_1, lv_color_hex(0x487b4b), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control_bar_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_bar_1, 10, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_bar_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style for screen_control_bar_1, Part: LV_PART_INDICATOR, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control_bar_1, 255, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control_bar_1, lv_color_hex(0x08ff54), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control_bar_1, LV_GRAD_DIR_NONE, LV_PART_INDICATOR|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_bar_1, 10, LV_PART_INDICATOR|LV_STATE_DEFAULT);

    //Write codes screen_control_label_2
    ui->screen_control_label_2 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_2, "已充电 65%");
    lv_label_set_long_mode(ui->screen_control_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_2, 95, 66);
    lv_obj_set_size(ui->screen_control_label_2, 123, 16);

    //Write style for screen_control_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_2, lv_color_hex(0x868686), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_2, &lv_font_SourceHanSerifSC_Regular_15, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_2, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_2, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_3
    ui->screen_control_label_3 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_3, "充电模式");
    lv_label_set_long_mode(ui->screen_control_label_3, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_3, 10, 104);
    lv_obj_set_size(ui->screen_control_label_3, 100, 32);

    //Write style for screen_control_label_3, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_3, lv_color_hex(0x09caed), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_3, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_3, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_3, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_3, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_4
    ui->screen_control_label_4 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_4, "25W\n");
    lv_label_set_long_mode(ui->screen_control_label_4, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_4, 209, 124);
    lv_obj_set_size(ui->screen_control_label_4, 94, 17);

    //Write style for screen_control_label_4, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_4, lv_color_hex(0x0ded09), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_4, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_4, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_4, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_4, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_5
    ui->screen_control_label_5 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_5, "90W");
    lv_label_set_long_mode(ui->screen_control_label_5, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_5, 206, 155);
    lv_obj_set_size(ui->screen_control_label_5, 100, 19);

    //Write style for screen_control_label_5, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_5, lv_color_hex(0xfe8638), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_5, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_5, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_5, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_5, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_6
    ui->screen_control_label_6 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_6, "30W");
    lv_label_set_long_mode(ui->screen_control_label_6, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_6, 206, 186);
    lv_obj_set_size(ui->screen_control_label_6, 100, 32);

    //Write style for screen_control_label_6, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_6, lv_color_hex(0xff0792), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_6, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_6, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_6, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_6, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_msgbox_1
    static const char * screen_control_msgbox_1_btns[] = {""};
    ui->screen_control_msgbox_1 = lv_msgbox_create(ui->screen_control, "启动成功", "标准充电已启动", screen_control_msgbox_1_btns, false);
    lv_obj_set_size(lv_msgbox_get_btns(ui->screen_control_msgbox_1), 0, 30);
    lv_obj_set_pos(ui->screen_control_msgbox_1, 61, 73);
    lv_obj_set_size(ui->screen_control_msgbox_1, 203, 123);
    lv_obj_add_flag(ui->screen_control_msgbox_1, LV_OBJ_FLAG_HIDDEN);

    //Write style for screen_control_msgbox_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_control_msgbox_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control_msgbox_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_control_msgbox_1, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_control_msgbox_1, 2, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui->screen_control_msgbox_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui->screen_control_msgbox_1, lv_color_hex(0x575757), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_side(ui->screen_control_msgbox_1, LV_BORDER_SIDE_FULL, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_msgbox_1, 18, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_msgbox_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_control_msgbox_1_extra_title_main_default
    static lv_style_t style_screen_control_msgbox_1_extra_title_main_default;
    ui_init_style(&style_screen_control_msgbox_1_extra_title_main_default);

    lv_style_set_text_color(&style_screen_control_msgbox_1_extra_title_main_default, lv_color_hex(0x00f93e));
    lv_style_set_text_font(&style_screen_control_msgbox_1_extra_title_main_default, &lv_font_SourceHanSerifSC_Regular_19);
    lv_style_set_text_opa(&style_screen_control_msgbox_1_extra_title_main_default, 255);
    lv_style_set_text_letter_space(&style_screen_control_msgbox_1_extra_title_main_default, 0);
    lv_style_set_text_line_space(&style_screen_control_msgbox_1_extra_title_main_default, 15);
    lv_obj_add_style(lv_msgbox_get_title(ui->screen_control_msgbox_1), &style_screen_control_msgbox_1_extra_title_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_control_msgbox_1_extra_content_main_default
    static lv_style_t style_screen_control_msgbox_1_extra_content_main_default;
    ui_init_style(&style_screen_control_msgbox_1_extra_content_main_default);

    lv_style_set_text_color(&style_screen_control_msgbox_1_extra_content_main_default, lv_color_hex(0x007f01));
    lv_style_set_text_font(&style_screen_control_msgbox_1_extra_content_main_default, &lv_font_SourceHanSerifSC_Regular_14);
    lv_style_set_text_opa(&style_screen_control_msgbox_1_extra_content_main_default, 255);
    lv_style_set_text_letter_space(&style_screen_control_msgbox_1_extra_content_main_default, 0);
    lv_style_set_text_line_space(&style_screen_control_msgbox_1_extra_content_main_default, 10);
    lv_obj_add_style(lv_msgbox_get_text(ui->screen_control_msgbox_1), &style_screen_control_msgbox_1_extra_content_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_control_msgbox_1_extra_btns_items_default
    static lv_style_t style_screen_control_msgbox_1_extra_btns_items_default;
    ui_init_style(&style_screen_control_msgbox_1_extra_btns_items_default);

    lv_style_set_bg_opa(&style_screen_control_msgbox_1_extra_btns_items_default, 0);
    lv_style_set_border_width(&style_screen_control_msgbox_1_extra_btns_items_default, 2);
    lv_style_set_border_opa(&style_screen_control_msgbox_1_extra_btns_items_default, 255);
    lv_style_set_border_color(&style_screen_control_msgbox_1_extra_btns_items_default, lv_color_hex(0x299d9c));
    lv_style_set_border_side(&style_screen_control_msgbox_1_extra_btns_items_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_control_msgbox_1_extra_btns_items_default, 4);
    lv_style_set_text_color(&style_screen_control_msgbox_1_extra_btns_items_default, lv_color_hex(0x01798e));
    lv_style_set_text_font(&style_screen_control_msgbox_1_extra_btns_items_default, &lv_font_SourceHanSerifSC_Regular_13);
    lv_style_set_text_opa(&style_screen_control_msgbox_1_extra_btns_items_default, 255);
    lv_obj_add_style(lv_msgbox_get_btns(ui->screen_control_msgbox_1), &style_screen_control_msgbox_1_extra_btns_items_default, LV_PART_ITEMS|LV_STATE_DEFAULT);

    //Write codes screen_control_label_7
    ui->screen_control_label_7 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_7, "标准模式");
    lv_label_set_long_mode(ui->screen_control_label_7, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_7, 16, 128);
    lv_obj_set_size(ui->screen_control_label_7, 87, 20);

    //Write style for screen_control_label_7, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_7, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_7, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_7, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_7, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_7, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_8
    ui->screen_control_label_8 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_8, "快充模式");
    lv_label_set_long_mode(ui->screen_control_label_8, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_8, 24, 155);
    lv_obj_set_size(ui->screen_control_label_8, 73, 20);

    //Write style for screen_control_label_8, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_8, lv_color_hex(0xfdfdfd), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_8, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_8, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_8, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_8, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_9
    ui->screen_control_label_9 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_9, "放电测试");
    lv_label_set_long_mode(ui->screen_control_label_9, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_9, 10, 186);
    lv_obj_set_size(ui->screen_control_label_9, 100, 32);

    //Write style for screen_control_label_9, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_9, lv_color_hex(0xfafafa), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_9, &lv_font_SourceHanSerifSC_Regular_14, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_9, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_9, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_9, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //The custom code of screen_control.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_control);

    //Init events for screen.
    events_init_screen_control(ui);
}
