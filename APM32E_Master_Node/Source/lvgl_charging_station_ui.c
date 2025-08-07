#include "lvgl.h"
#include "lvgl_charging_station_ui.h"
// 子站数量
#define STATION_NUM 4

// 子站信息结构体
typedef struct {
    const char *name;
    const char *status;
    const char *detail;
    uint32_t color; // 用0xRRGGBB常量
} station_info_t;

// 示例子站数据
static const station_info_t station_data[STATION_NUM] = {
    {"子站 01", "充电中 2.5A", "36.2V | 90W", 0x27ae60},
    {"子站 02", "空闲", "● 电池已连接", 0xaaaaaa},
    {"子站 03", "充电中 3.0A", "42.1V | 126W", 0x27ae60},
    {"子站 04", "空闲", "○ 请连接电池", 0xe74c3c},
};

// 主界面UI句柄
typedef struct {
    lv_obj_t *title;
    lv_obj_t *main_status;
    lv_obj_t *main_status_icon;
    lv_obj_t *station_grid;
    lv_obj_t *station_card[STATION_NUM];
    lv_obj_t *station_name[STATION_NUM];
    lv_obj_t *station_status[STATION_NUM];
    lv_obj_t *station_detail[STATION_NUM];
    lv_obj_t *power_bar;
    lv_obj_t *power_label;
    lv_obj_t *nav_hint;
} charging_station_ui_t;

static charging_station_ui_t ui;

void charging_station_ui_create(lv_obj_t *parent)
{
    // 背景色
    lv_obj_set_style_bg_color(parent, lv_color_hex(0x1a1a1a), 0);
    lv_obj_set_style_bg_opa(parent, LV_OPA_COVER, 0);

    // 顶部标题栏
    ui.title = lv_label_create(parent);
    lv_label_set_text(ui.title, "电动工具充电站 v1.0");
    lv_obj_set_style_text_color(ui.title, lv_color_hex(0xFFFFFF), 0);
    lv_obj_set_style_bg_color(ui.title, lv_color_hex(0x2980b9), 0);
    lv_obj_set_style_bg_opa(ui.title, LV_OPA_COVER, 0);
    lv_obj_set_width(ui.title, 320);
    lv_obj_set_height(ui.title, 28);
    lv_obj_align(ui.title, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_text_align(ui.title, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_all(ui.title, 4, 0);

    // 主站状态
    ui.main_status_icon = lv_label_create(parent);
    lv_label_set_text(ui.main_status_icon, "●");
    lv_obj_set_style_text_color(ui.main_status_icon, lv_color_hex(0x27ae60), 0);
    lv_obj_set_style_text_font(ui.main_status_icon, LV_FONT_DEFAULT, 0);
    lv_obj_align(ui.main_status_icon, LV_ALIGN_TOP_MID, -40, 34);

    ui.main_status = lv_label_create(parent);
    lv_label_set_text(ui.main_status, "主站状态: 正常运行");
    lv_obj_set_style_text_color(ui.main_status, lv_color_hex(0xFFFFFF), 0);
    lv_obj_align_to(ui.main_status, ui.main_status_icon, LV_ALIGN_OUT_RIGHT_MID, 8, 0);

    // 子站网格容器（无布局，仅用于分组）
    ui.station_grid = lv_obj_create(parent);
    lv_obj_set_size(ui.station_grid, 280, 100);
    lv_obj_set_style_bg_opa(ui.station_grid, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(ui.station_grid, LV_OPA_0, 0);
    lv_obj_align(ui.station_grid, LV_ALIGN_TOP_MID, 0, 60);

    // 手动排布4个子站卡片（2x2）
    for(int i=0; i<STATION_NUM; ++i) {
        int col = i % 2;
        int row = i / 2;
        int card_w = 130, card_h = 44;
        int gap_x = 20, gap_y = 12;
        int x = col * (card_w + gap_x);
        int y = row * (card_h + gap_y);

        ui.station_card[i] = lv_obj_create(ui.station_grid);
        lv_obj_set_size(ui.station_card[i], card_w, card_h);
        lv_obj_set_style_radius(ui.station_card[i], 8, 0);
        lv_obj_set_style_bg_color(ui.station_card[i], lv_color_hex(0x2c3e50), 0);
        lv_obj_set_style_border_color(ui.station_card[i], lv_color_hex(0x3498db), 0);
        lv_obj_set_style_border_width(ui.station_card[i], 2, 0);
        lv_obj_set_style_pad_all(ui.station_card[i], 2, 0);
        lv_obj_set_pos(ui.station_card[i], x, y);

        // 子站名称
        ui.station_name[i] = lv_label_create(ui.station_card[i]);
        lv_label_set_text(ui.station_name[i], station_data[i].name);
        lv_obj_set_style_text_color(ui.station_name[i], lv_color_hex(0xFFFFFF), 0);
        lv_obj_align(ui.station_name[i], LV_ALIGN_TOP_LEFT, 6, 2);

        // 子站状态
        ui.station_status[i] = lv_label_create(ui.station_card[i]);
        lv_label_set_text(ui.station_status[i], station_data[i].status);
        lv_obj_set_style_text_color(ui.station_status[i], lv_color_hex(station_data[i].color), 0);
        lv_obj_align(ui.station_status[i], LV_ALIGN_TOP_LEFT, 6, 20);

        // 子站详情
        ui.station_detail[i] = lv_label_create(ui.station_card[i]);
        lv_label_set_text(ui.station_detail[i], station_data[i].detail);
        lv_obj_set_style_text_color(ui.station_detail[i], lv_color_hex(0xCCCCCC), 0);
        lv_obj_align(ui.station_detail[i], LV_ALIGN_TOP_RIGHT, -6, 20);
    }

    // 功率分配条
    ui.power_bar = lv_bar_create(parent);
    lv_obj_set_size(ui.power_bar, 220, 14);
    lv_obj_align(ui.power_bar, LV_ALIGN_TOP_MID, 0, 170);
    lv_bar_set_range(ui.power_bar, 0, 200);
    lv_bar_set_value(ui.power_bar, 126, LV_ANIM_OFF);
    lv_obj_set_style_bg_color(ui.power_bar, lv_color_hex(0x333333), 0);
    lv_obj_set_style_bg_opa(ui.power_bar, LV_OPA_COVER, 0);
    lv_obj_set_style_radius(ui.power_bar, 7, 0);
    lv_obj_set_style_outline_opa(ui.power_bar, LV_OPA_0, 0);
    lv_obj_set_style_border_opa(ui.power_bar, LV_OPA_0, 0);
    lv_obj_set_style_pad_all(ui.power_bar, 0, 0);
    lv_obj_set_style_anim_time(ui.power_bar, 0, 0);
    lv_obj_set_style_bg_grad_dir(ui.power_bar, LV_GRAD_DIR_NONE, 0);
    // 指示器部分样式
    lv_obj_set_style_bg_color(ui.power_bar, lv_color_hex(0x27ae60), LV_PART_INDICATOR);
    lv_obj_set_style_bg_opa(ui.power_bar, LV_OPA_COVER, LV_PART_INDICATOR);
    lv_obj_set_style_radius(ui.power_bar, 7, LV_PART_INDICATOR);

    // 功率分配数值
    ui.power_label = lv_label_create(parent);
    lv_label_set_text(ui.power_label, "126W / 200W");
    lv_obj_set_style_text_color(ui.power_label, lv_color_hex(0x3498db), 0);
    lv_obj_align_to(ui.power_label, ui.power_bar, LV_ALIGN_OUT_RIGHT_MID, 10, 0);

    // 底部导航提示
    ui.nav_hint = lv_label_create(parent);
    lv_label_set_text(ui.nav_hint, "←→↑↓选择子站 OK查看详情 MENU菜单");
    lv_obj_set_style_text_color(ui.nav_hint, lv_color_hex(0xAAAAAA), 0);
    lv_obj_set_width(ui.nav_hint, 320);
    lv_obj_set_style_text_align(ui.nav_hint, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_align(ui.nav_hint, LV_ALIGN_BOTTOM_MID, 0, -4);
}
