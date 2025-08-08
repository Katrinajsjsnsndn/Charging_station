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
#include "widgets_init.h"

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
#include "gg_external_data.h"
#endif

void ui_init_style(lv_style_t * style)
{
    if (style->prop_cnt > 1)
        lv_style_reset(style);
    else
        lv_style_init(style);
}

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del)
{
    lv_obj_t * act_scr = lv_scr_act();

#if LV_USE_GUIDER_SIMULATOR && LV_USE_FREEMASTER
    if(auto_del) {
        gg_edata_task_clear(act_scr);
    }
#endif
    // 先创建新屏幕
    if (new_scr_del && *new_scr == NULL) {
        setup_scr(ui);
    }
    
    // 确保新屏幕已创建
    if (*new_scr == NULL) {
        return;
    }
    
    // 加载新屏幕 - 使用简单的加载方式，不使用动画
    lv_scr_load(*new_scr);
    
    // 不在这里清理旧屏幕，让调用者处理
    // 这样可以避免在同一个函数调用中创建和删除对象
    
    *old_scr_del = auto_del;
}

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                  uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                  lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb)
{
    lv_anim_t anim;
    lv_anim_init(&anim);
    lv_anim_set_var(&anim, var);
    lv_anim_set_exec_cb(&anim, exec_cb);
    lv_anim_set_values(&anim, start_value, end_value);
    lv_anim_set_time(&anim, duration);
    lv_anim_set_delay(&anim, delay);
    lv_anim_set_path_cb(&anim, path_cb);
    lv_anim_set_repeat_count(&anim, repeat_cnt);
    lv_anim_set_repeat_delay(&anim, repeat_delay);
    lv_anim_set_playback_time(&anim, playback_time);
    lv_anim_set_playback_delay(&anim, playback_delay);
    if (start_cb) {
        lv_anim_set_start_cb(&anim, start_cb);
    }
    if (ready_cb) {
        lv_anim_set_ready_cb(&anim, ready_cb);
    }
    if (deleted_cb) {
        lv_anim_set_deleted_cb(&anim, deleted_cb);
    }
    lv_anim_start(&anim);
}

void init_scr_del_flag(lv_ui *ui)
{

    ui->screen_Master_del = true;
    ui->screen_detail_del = true;
    ui->screen_control_del = true;
}

void setup_ui(lv_ui *ui)
{
    init_scr_del_flag(ui);
    init_keyboard(ui);
    setup_scr_screen_Master(ui);
    lv_scr_load(ui->screen_Master);
}

void control_update_highlight(lv_ui *ui)
{
    /* 高亮选中的control标签，未选中保持透明背景 */
    lv_obj_t* labels[3] = {
        ui->screen_control_mode_label_0,
        ui->screen_control_mode_label_1,
        ui->screen_control_mode_label_2
    };
    
    // 边界检查，确保索引在有效范围内
    if(ui->screen_control_selected_index < 0 || ui->screen_control_selected_index >= 3) {
        ui->screen_control_selected_index = 0; // 重置为默认值
    }
    
    // 检查所有标签是否都已创建
    for(int i = 0; i < 3; i++) {
        if(labels[i] == NULL) {
            return; // 如果有任何标签未创建，直接返回
        }
    }
    
    for(int i = 0; i < 3; i++) {
        // 检查对象是否仍然有效
        if(!lv_obj_is_valid(labels[i])) continue;
        
        if(i == ui->screen_control_selected_index) {
            lv_obj_set_style_bg_color(labels[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(labels[i], 255, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(labels[i], 2, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(labels[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
        } else {
            lv_obj_set_style_bg_opa(labels[i], 0, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(labels[i], 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        }
    }
}

void init_keyboard(lv_ui *ui)
{

}
