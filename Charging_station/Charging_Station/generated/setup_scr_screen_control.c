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
    lv_obj_set_style_bg_opa(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_1, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_control_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_control_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_control_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_bar_1
    ui->screen_control_bar_1 = lv_bar_create(ui->screen_control);
    lv_obj_set_style_anim_time(ui->screen_control_bar_1, 1000, 0);
    lv_bar_set_mode(ui->screen_control_bar_1, LV_BAR_MODE_NORMAL);
    lv_bar_set_range(ui->screen_control_bar_1, 0, 100);
    lv_bar_set_value(ui->screen_control_bar_1, 50, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_control_bar_1, 24, 43);
    lv_obj_set_size(ui->screen_control_bar_1, 274, 15);

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

    //Write codes screen_control_list_1
    ui->screen_control_list_1 = lv_list_create(ui->screen_control);
    ui->screen_control_list_1_item0 =lv_list_add_text(ui->screen_control_list_1, "标准充电             ");
    ui->screen_control_list_1_item1 =lv_list_add_text(ui->screen_control_list_1, "快速充电");
    ui->screen_control_list_1_item2 =lv_list_add_text(ui->screen_control_list_1, "放电测试");
    lv_obj_set_pos(ui->screen_control_list_1, 24, 100);
    lv_obj_set_size(ui->screen_control_list_1, 271, 122);
    lv_obj_set_scrollbar_mode(ui->screen_control_list_1, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_screen_control_list_1_main_main_default
    static lv_style_t style_screen_control_list_1_main_main_default;
    ui_init_style(&style_screen_control_list_1_main_main_default);

    lv_style_set_pad_top(&style_screen_control_list_1_main_main_default, 5);
    lv_style_set_pad_left(&style_screen_control_list_1_main_main_default, 5);
    lv_style_set_pad_right(&style_screen_control_list_1_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_control_list_1_main_main_default, 5);
    lv_style_set_bg_opa(&style_screen_control_list_1_main_main_default, 255);
    lv_style_set_bg_color(&style_screen_control_list_1_main_main_default, lv_color_hex(0x626262));
    lv_style_set_bg_grad_dir(&style_screen_control_list_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_control_list_1_main_main_default, 1);
    lv_style_set_border_opa(&style_screen_control_list_1_main_main_default, 255);
    lv_style_set_border_color(&style_screen_control_list_1_main_main_default, lv_color_hex(0x494949));
    lv_style_set_border_side(&style_screen_control_list_1_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_control_list_1_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_control_list_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_control_list_1, &style_screen_control_list_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_control_list_1_main_scrollbar_default
    static lv_style_t style_screen_control_list_1_main_scrollbar_default;
    ui_init_style(&style_screen_control_list_1_main_scrollbar_default);

    lv_style_set_radius(&style_screen_control_list_1_main_scrollbar_default, 3);
    lv_style_set_bg_opa(&style_screen_control_list_1_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_control_list_1_main_scrollbar_default, lv_color_hex(0xaa9c9c));
    lv_style_set_bg_grad_dir(&style_screen_control_list_1_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_control_list_1, &style_screen_control_list_1_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_control_list_1_extra_btns_main_default
    static lv_style_t style_screen_control_list_1_extra_btns_main_default;
    ui_init_style(&style_screen_control_list_1_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_control_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_control_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_control_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_control_list_1_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_control_list_1_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_control_list_1_extra_btns_main_default, lv_color_hex(0x0D3055));
    lv_style_set_text_font(&style_screen_control_list_1_extra_btns_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_screen_control_list_1_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_control_list_1_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_control_list_1_extra_btns_main_default, 255);
    lv_style_set_bg_color(&style_screen_control_list_1_extra_btns_main_default, lv_color_hex(0xffffff));
    lv_style_set_bg_grad_dir(&style_screen_control_list_1_extra_btns_main_default, LV_GRAD_DIR_NONE);

    //Write style state: LV_STATE_DEFAULT for &style_screen_control_list_1_extra_texts_main_default
    static lv_style_t style_screen_control_list_1_extra_texts_main_default;
    ui_init_style(&style_screen_control_list_1_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_control_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_control_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_control_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_control_list_1_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_control_list_1_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_control_list_1_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_screen_control_list_1_extra_texts_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_screen_control_list_1_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_control_list_1_extra_texts_main_default, 1);
    lv_style_set_transform_width(&style_screen_control_list_1_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_control_list_1_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_control_list_1_extra_texts_main_default, lv_color_hex(0x1ea8fa));
    lv_style_set_bg_grad_dir(&style_screen_control_list_1_extra_texts_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_control_list_1_item2, &style_screen_control_list_1_extra_texts_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_control_list_1_item1, &style_screen_control_list_1_extra_texts_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_control_list_1_item0, &style_screen_control_list_1_extra_texts_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_control_label_2
    ui->screen_control_label_2 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_2, "已充电");
    lv_label_set_long_mode(ui->screen_control_label_2, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_control_label_2, 123, 72);
    lv_obj_set_size(ui->screen_control_label_2, 64, 16);

    //Write style for screen_control_label_2, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_control_label_2, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_control_label_2, lv_color_hex(0x868686), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_2, &lv_font_SourceHanSerifSC_Regular_12, LV_PART_MAIN|LV_STATE_DEFAULT);
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

    //The custom code of screen_control.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_control);

}
