/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#include "lvgl.h"
#include "gui_guider.h"
#include "widgets_init.h"

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
    
    // 标题
    ui->screen_control_label_1 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_1, "Charging Control");
    lv_obj_set_pos(ui->screen_control_label_1, 100, 20);
    lv_obj_set_style_text_color(ui->screen_control_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_1, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    
    // 状态显示
    ui->screen_control_label_2 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_2, "Status: Ready");
    lv_obj_set_pos(ui->screen_control_label_2, 20, 60);
    lv_obj_set_style_text_color(ui->screen_control_label_2, lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
    
    // 功率显示标签
    ui->screen_control_label_3 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_3, "Power: 25W");
    lv_obj_set_pos(ui->screen_control_label_3, 20, 90);
    lv_obj_set_style_text_color(ui->screen_control_label_3, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    
    ui->screen_control_label_4 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_4, "Power: 90W");
    lv_obj_set_pos(ui->screen_control_label_4, 20, 120);
    lv_obj_set_style_text_color(ui->screen_control_label_4, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    
    ui->screen_control_label_5 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_5, "Power: 30W");
    lv_obj_set_pos(ui->screen_control_label_5, 20, 150);
    lv_obj_set_style_text_color(ui->screen_control_label_5, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    
    // 进度条
    ui->screen_control_bar_1 = lv_bar_create(ui->screen_control);
    lv_bar_set_range(ui->screen_control_bar_1, 0, 100);
    lv_bar_set_value(ui->screen_control_bar_1, 75, LV_ANIM_OFF);
    lv_obj_set_pos(ui->screen_control_bar_1, 20, 180);
    lv_obj_set_size(ui->screen_control_bar_1, 280, 20);
    lv_obj_set_style_bg_color(ui->screen_control_bar_1, lv_color_hex(0x333333), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui->screen_control_bar_1, lv_color_hex(0x00ff00), LV_PART_INDICATOR|LV_STATE_DEFAULT);
    
    // 添加三个可选择的充电模式标签
    ui->screen_control_mode_label_0 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_mode_label_0, "Standard Charging");
    lv_obj_set_pos(ui->screen_control_mode_label_0, 120, 90);
    lv_obj_set_style_text_color(ui->screen_control_mode_label_0, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    
    ui->screen_control_mode_label_1 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_mode_label_1, "Fast Charging");
    lv_obj_set_pos(ui->screen_control_mode_label_1, 120, 120);
    lv_obj_set_style_text_color(ui->screen_control_mode_label_1, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    
    ui->screen_control_mode_label_2 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_mode_label_2, "Discharge Test");
    lv_obj_set_pos(ui->screen_control_mode_label_2, 120, 150);
    lv_obj_set_style_text_color(ui->screen_control_mode_label_2, lv_color_hex(0xffffff), LV_PART_MAIN|LV_STATE_DEFAULT);
    
    // 添加操作提示
    ui->screen_control_label_6 = lv_label_create(ui->screen_control);
    lv_label_set_text(ui->screen_control_label_6, "Use UP/DOWN to select, OK to confirm");
    lv_obj_set_pos(ui->screen_control_label_6, 20, 210);
    lv_obj_set_style_text_color(ui->screen_control_label_6, lv_color_hex(0x888888), LV_PART_MAIN|LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui->screen_control_label_6, &lv_font_montserratMedium_16, LV_PART_MAIN|LV_STATE_DEFAULT);
    
    // 将标签指针赋值给 control_labels 数组
    extern lv_obj_t* control_labels[3];
    control_labels[0] = ui->screen_control_mode_label_0;
    control_labels[1] = ui->screen_control_mode_label_1;
    control_labels[2] = ui->screen_control_mode_label_2;
    
    // 初始化其他必要的指针为 NULL（只设置未使用的指针）
    ui->screen_control_radiobtn_1 = NULL;
    ui->screen_control_label_3 = NULL;
    ui->screen_control_label_4 = NULL;
    ui->screen_control_label_5 = NULL;
    ui->screen_control_label_6 = NULL;
    ui->screen_control_msgbox_1 = NULL;
    ui->screen_control_selected_index = 0;
    
    //Update current screen layout.
    lv_obj_update_layout(ui->screen_control);

    //Init events for screen.
    events_init_screen_control(ui);
}
