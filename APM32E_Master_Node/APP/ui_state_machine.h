#ifndef UI_STATE_MACHINE_H
#define UI_STATE_MACHINE_H

#include <stdint.h>
#include <stdbool.h>
#include "charging_station_ui.h"
#include "gui_guider.h"  // 包含lv_ui类型定义

// 页面状态枚举
typedef enum {
    PAGE_MAIN = 0,
    PAGE_DETAIL,
    PAGE_CONTROL,
    PAGE_MENU,
    PAGE_TEXT,
    PAGE_STATIONS,
    PAGE_POWER_MANAGEMENT,
    PAGE_BATTERY_TEST,
    PAGE_BATTERY_MAINTENANCE,
    PAGE_MONITOR,
    PAGE_SETTINGS,
    PAGE_STATION_DETAIL,
    PAGE_COUNT
} page_state_t;

// 按键事件枚举
typedef enum {
    KEY_EVENT_NONE = 0,
    KEY_EVENT_UP,
    KEY_EVENT_DOWN,
    KEY_EVENT_LEFT,
    KEY_EVENT_RIGHT,
    KEY_EVENT_OK,
    KEY_EVENT_RETURN,
    KEY_EVENT_MENU,
    KEY_EVENT_COUNT
} key_event_t;

// 页面上下文结构
typedef struct {
    page_state_t current_page;
    page_state_t previous_page;
    int selected_item;
    int selected_station;
    int selected_mode;
    bool page_initialized;
    uint32_t last_update_time;
} ui_context_t;

// 页面处理器结构
typedef struct {
    void (*init)(ui_context_t* ctx);
    void (*update)(ui_context_t* ctx);
    void (*handle_key)(ui_context_t* ctx, key_event_t event);
    void (*exit)(ui_context_t* ctx);
} page_handler_t;

// 状态转换表结构
typedef struct {
    page_state_t current_state;
    key_event_t event;
    page_state_t next_state;
    void (*action)(ui_context_t* ctx);
} state_transition_t;

// 状态机初始化
void ui_state_machine_init(void);

// 状态机处理函数
void ui_state_machine_process(ui_context_t* ctx, key_event_t event);

// 页面更新函数
void ui_page_update(ui_context_t* ctx);

// 获取当前UI上下文
ui_context_t* ui_get_context(void);

// 按键事件转换函数
key_event_t convert_key_to_event(KeyEnum key);

// 动态数据更新函数
void update_dynamic_data(ui_context_t* ctx);

// 页面切换辅助函数
void switch_to_page(ui_context_t* ctx, page_state_t new_page);

// 状态机主任务函数已移至main.c中的Lvgl_ui_task

// 高亮功能函数（供外部调用）
void update_option_highlight(lv_ui *ui);
void select_option(int new_index);
void select_control_option(int new_index);
void update_control_option_highlight(void);
void update_menu_selection(void);

#endif // UI_STATE_MACHINE_H
