#ifndef LVGL_CHARGING_STATION_UI_H
#define LVGL_CHARGING_STATION_UI_H

#include "lvgl.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建充电站主界面
 * @param parent 父对象（一般为lv_scr_act()）
 */
void charging_station_ui_create(lv_obj_t *parent);

#ifdef __cplusplus
}
#endif

#endif // LVGL_CHARGING_STATION_UI_H



