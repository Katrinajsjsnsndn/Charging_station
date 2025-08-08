/*
* Copyright 2025 NXP
* NXP Proprietary. This software is owned or controlled by NXP and may only be used strictly in
* accordance with the applicable license terms. By expressly accepting such terms or by downloading, installing,
* activating and/or otherwise using the software, you are agreeing that you have read, and that you agree to
* comply with and are bound by, such license terms.  If you do not agree to be bound by the applicable license
* terms, then you may not retain, install, activate or otherwise use the software.
*/

#ifndef GUI_GUIDER_H
#define GUI_GUIDER_H
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl.h"

typedef struct
{
  
	lv_obj_t *screen_Master;
	bool screen_Master_del;
	lv_obj_t *screen_Master_Master_screen;
	lv_obj_t *screen_Master_cont_1;
	lv_obj_t *screen_Master_label_1;
	lv_obj_t *screen_Master_label_7;
	lv_obj_t *screen_Master_cont_2;
	lv_obj_t *screen_Master_label_8;
	lv_obj_t *screen_Master_label_2;
	lv_obj_t *screen_Master_cont_3;
	lv_obj_t *screen_Master_label_3;
	lv_obj_t *screen_Master_label_9;
	lv_obj_t *screen_Master_cont_4;
	lv_obj_t *screen_Master_label_4;
	lv_obj_t *screen_Master_label_10;
	lv_obj_t *screen_Master_label_5;
	lv_obj_t *screen_Master_label_6;
	lv_obj_t *screen_detail;
	bool screen_detail_del;
	lv_obj_t *screen_detail_cont_1;
	lv_obj_t *screen_detail_label_1;
	lv_obj_t *screen_detail_label_2;
	lv_obj_t *screen_detail_label_3;
	lv_obj_t *screen_detail_bar_1;
	lv_obj_t *screen_detail_label_4;
	lv_obj_t *screen_detail_label_5;
	lv_obj_t *screen_detail_label_6;
	lv_obj_t *screen_detail_label_7;
	lv_obj_t *screen_detail_label_8;
	lv_obj_t *screen_detail_label_9;
	lv_obj_t *screen_detail_label_10;
	lv_obj_t *screen_detail_label_11;
	lv_obj_t *screen_control;
	bool screen_control_del;
	lv_obj_t *screen_control_label_1;
	lv_obj_t *screen_control_bar_1;
	lv_obj_t *screen_control_label_2;
	lv_obj_t *screen_control_radiobtn_1;
	lv_obj_t *screen_control_radiobtn_1_item0;
	lv_obj_t *screen_control_radiobtn_1_item1;
	lv_obj_t *screen_control_radiobtn_1_item2;
	lv_obj_t *screen_control_label_3;
	lv_obj_t *screen_control_label_4;
	lv_obj_t *screen_control_label_5;
	lv_obj_t *screen_control_label_6;
	lv_obj_t *screen_control_msgbox_1;
}lv_ui;

typedef void (*ui_setup_scr_t)(lv_ui * ui);

void ui_init_style(lv_style_t * style);

void ui_load_scr_animation(lv_ui *ui, lv_obj_t ** new_scr, bool new_scr_del, bool * old_scr_del, ui_setup_scr_t setup_scr,
                           lv_scr_load_anim_t anim_type, uint32_t time, uint32_t delay, bool is_clean, bool auto_del);

void ui_animation(void * var, int32_t duration, int32_t delay, int32_t start_value, int32_t end_value, lv_anim_path_cb_t path_cb,
                       uint16_t repeat_cnt, uint32_t repeat_delay, uint32_t playback_time, uint32_t playback_delay,
                       lv_anim_exec_xcb_t exec_cb, lv_anim_start_cb_t start_cb, lv_anim_ready_cb_t ready_cb, lv_anim_deleted_cb_t deleted_cb);


void init_scr_del_flag(lv_ui *ui);

void setup_ui(lv_ui *ui);

void init_keyboard(lv_ui *ui);

extern lv_ui guider_ui;


void setup_scr_screen_Master(lv_ui *ui);
void setup_scr_screen_detail(lv_ui *ui);
void setup_scr_screen_control(lv_ui *ui);

LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_11)
LV_FONT_DECLARE(lv_font_montserratMedium_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_9)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_16)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_12)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_14)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_15)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_13)
LV_FONT_DECLARE(lv_font_SourceHanSerifSC_Regular_19)


#ifdef __cplusplus
}
#endif
#endif
