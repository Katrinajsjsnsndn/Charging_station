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



void setup_scr_screen_menu(lv_ui *ui)
{
    //Write codes screen_menu
    ui->screen_menu = lv_obj_create(NULL);
    lv_obj_set_size(ui->screen_menu, 320, 240);
    lv_obj_set_scrollbar_mode(ui->screen_menu, LV_SCROLLBAR_MODE_OFF);

    //Write style for screen_menu, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_bg_opa(ui->screen_menu, 254, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_menu, lv_color_hex(0x000000), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_grad_dir(ui->screen_menu, LV_GRAD_DIR_NONE, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_label_1
    ui->screen_menu_label_1 = lv_label_create(ui->screen_menu);
    lv_label_set_text(ui->screen_menu_label_1, "主菜单");
    lv_label_set_long_mode(ui->screen_menu_label_1, LV_LABEL_LONG_WRAP);
    lv_obj_set_pos(ui->screen_menu_label_1, 110, 23);
    lv_obj_set_size(ui->screen_menu_label_1, 100, 32);

    //Write style for screen_menu_label_1, Part: LV_PART_MAIN, State: LV_STATE_DEFAULT.
    lv_obj_set_style_border_width(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_radius(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(ui->screen_menu_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_menu_label_1, &lv_font_SourceHanSerifSC_Regular_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui->screen_menu_label_1, 255, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_line_space(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_align(ui->screen_menu_label_1, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_width(ui->screen_menu_label_1, 0, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write codes screen_menu_list_1
    ui->screen_menu_list_1 = lv_list_create(ui->screen_menu);
    ui->screen_menu_list_1_item0 = lv_list_add_btn(ui->screen_menu_list_1, LV_SYMBOL_RIGHT, "子站管理");
    ui->screen_menu_list_1_item1 = lv_list_add_btn(ui->screen_menu_list_1, LV_SYMBOL_RIGHT, "充电设置");
    ui->screen_menu_list_1_item2 = lv_list_add_btn(ui->screen_menu_list_1, LV_SYMBOL_RIGHT, "状态监控");
    ui->screen_menu_list_1_item3 = lv_list_add_btn(ui->screen_menu_list_1, LV_SYMBOL_RIGHT, "系统设置");
    ui->screen_menu_list_1_item4 = lv_list_add_btn(ui->screen_menu_list_1, LV_SYMBOL_RIGHT, "故障记录");
    lv_obj_set_pos(ui->screen_menu_list_1, 17, 51);
    lv_obj_set_size(ui->screen_menu_list_1, 282, 162);
    lv_obj_set_scrollbar_mode(ui->screen_menu_list_1, LV_SCROLLBAR_MODE_OFF);

    //Write style state: LV_STATE_DEFAULT for &style_screen_menu_list_1_main_main_default
    static lv_style_t style_screen_menu_list_1_main_main_default;
    ui_init_style(&style_screen_menu_list_1_main_main_default);

    lv_style_set_pad_top(&style_screen_menu_list_1_main_main_default, 5);
    lv_style_set_pad_left(&style_screen_menu_list_1_main_main_default, 5);
    lv_style_set_pad_right(&style_screen_menu_list_1_main_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_menu_list_1_main_main_default, 5);
    lv_style_set_bg_opa(&style_screen_menu_list_1_main_main_default, 238);
    lv_style_set_bg_color(&style_screen_menu_list_1_main_main_default, lv_color_hex(0x000000));
    lv_style_set_bg_grad_dir(&style_screen_menu_list_1_main_main_default, LV_GRAD_DIR_NONE);
    lv_style_set_border_width(&style_screen_menu_list_1_main_main_default, 1);
    lv_style_set_border_opa(&style_screen_menu_list_1_main_main_default, 255);
    lv_style_set_border_color(&style_screen_menu_list_1_main_main_default, lv_color_hex(0x000000));
    lv_style_set_border_side(&style_screen_menu_list_1_main_main_default, LV_BORDER_SIDE_FULL);
    lv_style_set_radius(&style_screen_menu_list_1_main_main_default, 3);
    lv_style_set_shadow_width(&style_screen_menu_list_1_main_main_default, 0);
    lv_obj_add_style(ui->screen_menu_list_1, &style_screen_menu_list_1_main_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_menu_list_1_main_scrollbar_default
    static lv_style_t style_screen_menu_list_1_main_scrollbar_default;
    ui_init_style(&style_screen_menu_list_1_main_scrollbar_default);

    lv_style_set_radius(&style_screen_menu_list_1_main_scrollbar_default, 2);
    lv_style_set_bg_opa(&style_screen_menu_list_1_main_scrollbar_default, 255);
    lv_style_set_bg_color(&style_screen_menu_list_1_main_scrollbar_default, lv_color_hex(0x202020));
    lv_style_set_bg_grad_dir(&style_screen_menu_list_1_main_scrollbar_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_menu_list_1, &style_screen_menu_list_1_main_scrollbar_default, LV_PART_SCROLLBAR|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_menu_list_1_extra_btns_main_default
    static lv_style_t style_screen_menu_list_1_extra_btns_main_default;
    ui_init_style(&style_screen_menu_list_1_extra_btns_main_default);

    lv_style_set_pad_top(&style_screen_menu_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_left(&style_screen_menu_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_right(&style_screen_menu_list_1_extra_btns_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_menu_list_1_extra_btns_main_default, 5);
    lv_style_set_border_width(&style_screen_menu_list_1_extra_btns_main_default, 0);
    lv_style_set_text_color(&style_screen_menu_list_1_extra_btns_main_default, lv_color_hex(0x949494));
    lv_style_set_text_font(&style_screen_menu_list_1_extra_btns_main_default, &lv_font_SourceHanSerifSC_Regular_13);
    lv_style_set_text_opa(&style_screen_menu_list_1_extra_btns_main_default, 255);
    lv_style_set_radius(&style_screen_menu_list_1_extra_btns_main_default, 3);
    lv_style_set_bg_opa(&style_screen_menu_list_1_extra_btns_main_default, 151);
    lv_style_set_bg_color(&style_screen_menu_list_1_extra_btns_main_default, lv_color_hex(0x000000));
    lv_style_set_bg_grad_dir(&style_screen_menu_list_1_extra_btns_main_default, LV_GRAD_DIR_NONE);
    lv_obj_add_style(ui->screen_menu_list_1_item4, &style_screen_menu_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_menu_list_1_item3, &style_screen_menu_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_menu_list_1_item2, &style_screen_menu_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_menu_list_1_item1, &style_screen_menu_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_add_style(ui->screen_menu_list_1_item0, &style_screen_menu_list_1_extra_btns_main_default, LV_PART_MAIN|LV_STATE_DEFAULT);

    //Write style state: LV_STATE_DEFAULT for &style_screen_menu_list_1_extra_texts_main_default
    static lv_style_t style_screen_menu_list_1_extra_texts_main_default;
    ui_init_style(&style_screen_menu_list_1_extra_texts_main_default);

    lv_style_set_pad_top(&style_screen_menu_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_left(&style_screen_menu_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_right(&style_screen_menu_list_1_extra_texts_main_default, 5);
    lv_style_set_pad_bottom(&style_screen_menu_list_1_extra_texts_main_default, 5);
    lv_style_set_border_width(&style_screen_menu_list_1_extra_texts_main_default, 0);
    lv_style_set_text_color(&style_screen_menu_list_1_extra_texts_main_default, lv_color_hex(0xffffff));
    lv_style_set_text_font(&style_screen_menu_list_1_extra_texts_main_default, &lv_font_SourceHanSerifSC_Regular_12);
    lv_style_set_text_opa(&style_screen_menu_list_1_extra_texts_main_default, 255);
    lv_style_set_radius(&style_screen_menu_list_1_extra_texts_main_default, 3);
    lv_style_set_transform_width(&style_screen_menu_list_1_extra_texts_main_default, 0);
    lv_style_set_bg_opa(&style_screen_menu_list_1_extra_texts_main_default, 255);
    lv_style_set_bg_color(&style_screen_menu_list_1_extra_texts_main_default, lv_color_hex(0x000000));
    lv_style_set_bg_grad_dir(&style_screen_menu_list_1_extra_texts_main_default, LV_GRAD_DIR_NONE);

    //The custom code of screen_menu.


    //Update current screen layout.
    lv_obj_update_layout(ui->screen_menu);

}
