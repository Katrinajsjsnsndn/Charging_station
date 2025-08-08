#include "ui_state_machine.h"
#include "lvgl.h"
#include "gui_guider.h"
#include "events_init.h"
#include "main.h"
#include <stdio.h>

// 全局变量声明
extern lv_ui guider_ui;
extern lv_obj_t* control_labels[3];
static int selected_option = 0;
static int selected_control_option = 0;

// 全局UI上下文
static ui_context_t g_ui_context = {0};

// 前向声明 - 页面处理函数
static void main_page_init(ui_context_t* ctx);
static void main_page_update(ui_context_t* ctx);
static void main_page_handle_key(ui_context_t* ctx, key_event_t event);
static void main_page_exit(ui_context_t* ctx);

static void detail_page_init(ui_context_t* ctx);
static void detail_page_update(ui_context_t* ctx);
static void detail_page_handle_key(ui_context_t* ctx, key_event_t event);
static void detail_page_exit(ui_context_t* ctx);

static void control_page_init(ui_context_t* ctx);
static void control_page_update(ui_context_t* ctx);
static void control_page_handle_key(ui_context_t* ctx, key_event_t event);
static void control_page_exit(ui_context_t* ctx);

static void menu_page_init(ui_context_t* ctx);
static void menu_page_update(ui_context_t* ctx);
static void menu_page_handle_key(ui_context_t* ctx, key_event_t event);
static void menu_page_exit(ui_context_t* ctx);

// 页面处理器数组
static const page_handler_t page_handlers[PAGE_COUNT] = {
    [PAGE_MAIN] = {
        .init = main_page_init,
        .update = main_page_update,
        .handle_key = main_page_handle_key,
        .exit = main_page_exit
    },
    [PAGE_DETAIL] = {
        .init = detail_page_init,
        .update = detail_page_update,
        .handle_key = detail_page_handle_key,
        .exit = detail_page_exit
    },
    [PAGE_CONTROL] = {
        .init = control_page_init,
        .update = control_page_update,
        .handle_key = control_page_handle_key,
        .exit = control_page_exit
    },
    [PAGE_MENU] = {
        .init = menu_page_init,
        .update = menu_page_update,
        .handle_key = menu_page_handle_key,
        .exit = menu_page_exit
    },
    // 其他页面暂时使用默认处理器
    [PAGE_STATIONS] = {0},
    [PAGE_POWER_MANAGEMENT] = {0},
    [PAGE_BATTERY_TEST] = {0},
    [PAGE_BATTERY_MAINTENANCE] = {0},
    [PAGE_MONITOR] = {0},
    [PAGE_SETTINGS] = {0},
    [PAGE_STATION_DETAIL] = {0}
};

// 状态转换表
static const state_transition_t state_transitions[] = {
    // 主页面转换
    {PAGE_MAIN, KEY_EVENT_OK, PAGE_DETAIL, NULL},
    {PAGE_MAIN, KEY_EVENT_MENU, PAGE_MENU, NULL},
    
    // 详情页面转换
    {PAGE_DETAIL, KEY_EVENT_OK, PAGE_CONTROL, NULL},
    {PAGE_DETAIL, KEY_EVENT_RETURN, PAGE_MAIN, NULL},
    
    // 控制页面转换
    {PAGE_CONTROL, KEY_EVENT_RETURN, PAGE_DETAIL, NULL},
    
    // 菜单页面转换
    {PAGE_MENU, KEY_EVENT_RETURN, PAGE_MAIN, NULL},
    
    // 结束标记
    {PAGE_COUNT, KEY_EVENT_NONE, PAGE_COUNT, NULL}
};

// 状态机初始化
void ui_state_machine_init(void)
{
    // 初始化UI上下文
    g_ui_context.current_page = PAGE_MAIN;
    g_ui_context.previous_page = PAGE_MAIN;
    g_ui_context.selected_item = 0;
    g_ui_context.selected_station = 0;
    g_ui_context.selected_mode = 0;
    g_ui_context.page_initialized = false;
    g_ui_context.last_update_time = 0;
    
    // 初始化LVGL UI
    setup_ui(&guider_ui);
    events_init(&guider_ui);
}

// 查找状态转换
static const state_transition_t* find_state_transition(page_state_t state, key_event_t event)
{
    for (int i = 0; state_transitions[i].current_state != PAGE_COUNT; i++) {
        if (state_transitions[i].current_state == state && 
            state_transitions[i].event == event) {
            return &state_transitions[i];
        }
    }
    return NULL;
}

// 状态机处理函数
void ui_state_machine_process(ui_context_t* ctx, key_event_t event)
{
    if (ctx == NULL || event == KEY_EVENT_NONE) {
        return;
    }
    
    // 查找状态转换
    const state_transition_t* transition = find_state_transition(ctx->current_page, event);
    
    if (transition != NULL) {
        // 执行转换动作
        if (transition->action != NULL) {
            transition->action(ctx);
        }
        
        // 页面切换
        if (transition->next_state != ctx->current_page) {
            // 退出当前页面
            if (page_handlers[ctx->current_page].exit != NULL) {
                page_handlers[ctx->current_page].exit(ctx);
            }
            
            // 更新状态
            ctx->previous_page = ctx->current_page;
            ctx->current_page = transition->next_state;
            ctx->page_initialized = false;
        }
    }
    
    // 处理页面内部逻辑
    if (page_handlers[ctx->current_page].handle_key != NULL) {
        page_handlers[ctx->current_page].handle_key(ctx, event);
    }
}

// 页面更新函数
void ui_page_update(ui_context_t* ctx)
{
    if (ctx == NULL) {
        return;
    }
    
    // 初始化页面（如果需要）
    if (!ctx->page_initialized) {
        if (page_handlers[ctx->current_page].init != NULL) {
            page_handlers[ctx->current_page].init(ctx);
        }
        ctx->page_initialized = true;
    }
    
    // 更新页面
    if (page_handlers[ctx->current_page].update != NULL) {
        page_handlers[ctx->current_page].update(ctx);
    }
}

// 获取当前UI上下文
ui_context_t* ui_get_context(void)
{
    return &g_ui_context;
}

// 按键事件转换函数
key_event_t convert_key_to_event(KeyEnum key)
{
    switch (key) {
        case KEY_UP: return KEY_EVENT_UP;
        case KEY_DOWN: return KEY_EVENT_DOWN;
        case KEY_LEFT: return KEY_EVENT_LEFT;
        case KEY_RIGHT: return KEY_EVENT_RIGHT;
        case KEY_OK: return KEY_EVENT_OK;
        case KEY_RETURN: return KEY_EVENT_RETURN;
        case KEY_MENU: return KEY_EVENT_MENU;
        default: return KEY_EVENT_NONE;
    }
}

// 动态数据更新函数
void update_dynamic_data(ui_context_t* ctx)
{
    if (ctx == NULL) {
        return;
    }
    
    static uint32_t update_counter = 0;
    update_counter++;
    
    // 每50次循环更新一次数据（约250ms）
    if (update_counter % 50 == 0) {
        // 更新control页面的进度条
        if (ctx->current_page == PAGE_CONTROL && guider_ui.screen_control_bar_1 != NULL) {
            static int control_bar_value = 75;
            control_bar_value = (control_bar_value + 5) % 100;
            lv_bar_set_value(guider_ui.screen_control_bar_1, control_bar_value, LV_ANIM_OFF);
        }
        
        // 更新detail页面的进度条
        if (ctx->current_page == PAGE_DETAIL && guider_ui.screen_detail_bar_1 != NULL) {
            static int detail_bar_value = 50;
            detail_bar_value = (detail_bar_value + 2) % 100;
            lv_bar_set_value(guider_ui.screen_detail_bar_1, detail_bar_value, LV_ANIM_ON);
            
            // 同时更新充电百分比标签
            static char percent_text[20];
            snprintf(percent_text, sizeof(percent_text), "Charged %d%%", detail_bar_value);
            lv_label_set_text(guider_ui.screen_detail_label_7, percent_text);
            
            // 更新电压显示（模拟变化）
            static float voltage = 17.23f;
            voltage = 16.5f + (detail_bar_value / 100.0f) * 1.5f;
            static char voltage_text[20];
            snprintf(voltage_text, sizeof(voltage_text), "%.2fV", voltage);
            lv_label_set_text(guider_ui.screen_detail_label_8, voltage_text);
            
            // 更新功率显示
            static int power = 20;
            power = 15 + (detail_bar_value / 20);
            static char power_text[20];
            snprintf(power_text, sizeof(power_text), "%dW", power);
            lv_label_set_text(guider_ui.screen_detail_label_10, power_text);
            
            // 更新温度显示（模拟变化）
            static int temp_counter = 0;
            temp_counter++;
            int temperature = 30 + (temp_counter % 20);
            static char temp_text[20];
            snprintf(temp_text, sizeof(temp_text), "%d°C", temperature);
            lv_label_set_text(guider_ui.screen_detail_label_11, temp_text);
        }
    }
}

// 页面切换辅助函数
void switch_to_page(ui_context_t* ctx, page_state_t new_page)
{
    if (ctx == NULL || new_page >= PAGE_COUNT) {
        return;
    }
    
    // 退出当前页面
    if (page_handlers[ctx->current_page].exit != NULL) {
        page_handlers[ctx->current_page].exit(ctx);
    }
    
    // 更新状态
    ctx->previous_page = ctx->current_page;
    ctx->current_page = new_page;
    ctx->page_initialized = false;
}

// 状态机主任务函数已移至main.c中的Lvgl_ui_task

// ==================== 页面处理函数实现 ====================

// 主页面处理函数
static void main_page_init(ui_context_t* ctx)
{
    // 加载主页面UI
    lv_scr_load(guider_ui.screen_Master);
    lv_refr_now(NULL);
    
    // 初始化选择状态
    ctx->selected_station = 0;
    update_option_highlight(&guider_ui);
}

static void main_page_update(ui_context_t* ctx)
{
    // 主页面动态更新逻辑
    // 可以在这里更新功率显示、状态等
}

static void main_page_handle_key(ui_context_t* ctx, key_event_t event)
{
    // 主页面内部按键处理
    switch (event) {
        case KEY_EVENT_UP:
        case KEY_EVENT_DOWN:
        case KEY_EVENT_LEFT:
        case KEY_EVENT_RIGHT:
            // 更新选择状态
            int new_index = ctx->selected_station;
            
            switch (event) {
                case KEY_EVENT_LEFT:
                    new_index = (new_index - 1 + 4) % 4;
                    break;
                case KEY_EVENT_RIGHT:
                    new_index = (new_index + 1) % 4;
                    break;
                case KEY_EVENT_UP:
                    new_index = (new_index - 2 + 4) % 4;
                    break;
                case KEY_EVENT_DOWN:
                    new_index = (new_index + 2) % 4;
                    break;
                default:
                    break;
            }
            
            if (new_index != ctx->selected_station) {
                ctx->selected_station = new_index;
                select_option(new_index);
            }
            break;
    }
}

static void main_page_exit(ui_context_t* ctx)
{
    // 清理主页面资源
    // 保存状态等
}

// 详情页面处理函数
static void detail_page_init(ui_context_t* ctx)
{
    // 加载详情页面UI
    lv_scr_load(guider_ui.screen_detail);
    lv_refr_now(NULL);
}

static void detail_page_update(ui_context_t* ctx)
{
    // 详情页面动态更新逻辑
}

static void detail_page_handle_key(ui_context_t* ctx, key_event_t event)
{
    // 详情页面内部按键处理
}

static void detail_page_exit(ui_context_t* ctx)
{
    // 清理详情页面资源
}

// 控制页面处理函数
static void control_page_init(ui_context_t* ctx)
{
    // 加载控制页面UI
    lv_scr_load(guider_ui.screen_control);
    lv_refr_now(NULL);
    
    // 初始化控制页面选中状态
    ctx->selected_mode = 0;
    selected_control_option = 0;
    guider_ui.screen_control_selected_index = 0;
    update_control_option_highlight();
}

static void control_page_update(ui_context_t* ctx)
{
    // 控制页面动态更新逻辑
}

static void control_page_handle_key(ui_context_t* ctx, key_event_t event)
{
    // 控制页面内部按键处理
    switch (event) {
        case KEY_EVENT_UP:
        case KEY_EVENT_DOWN:
            int new_index = ctx->selected_mode;
            
            if (event == KEY_EVENT_UP) {
                new_index = (new_index - 1 + 3) % 3;
            } else {
                new_index = (new_index + 1) % 3;
            }
            
            if (new_index != ctx->selected_mode) {
                ctx->selected_mode = new_index;
                select_control_option(new_index);
            }
            break;
    }
}

static void control_page_exit(ui_context_t* ctx)
{
    // 清理控制页面资源
}

// 菜单页面处理函数
static void menu_page_init(ui_context_t* ctx)
{
    // 加载菜单页面UI
    // 这里需要创建菜单页面，暂时使用详情页面代替
    lv_scr_load(guider_ui.screen_detail);
    lv_refr_now(NULL);
    
    ctx->selected_item = 0;
}

static void menu_page_update(ui_context_t* ctx)
{
    // 菜单页面动态更新逻辑
}

static void menu_page_handle_key(ui_context_t* ctx, key_event_t event)
{
    // 菜单页面内部按键处理
    switch (event) {
        case KEY_EVENT_UP:
        case KEY_EVENT_DOWN:
            int new_index = ctx->selected_item;
            
            if (event == KEY_EVENT_UP && new_index > 0) {
                new_index--;
            } else if (event == KEY_EVENT_DOWN && new_index < 5) { // 假设有6个菜单项
                new_index++;
            }
            
            ctx->selected_item = new_index;
            break;
    }
}

static void menu_page_exit(ui_context_t* ctx)
{
    // 清理菜单页面资源
}

// ==================== 高亮功能函数实现 ====================

// 选项框高亮功能函数
void update_option_highlight(lv_ui *ui)
{
    // 获取所有选项框
    lv_obj_t* option_boxes[4] = {
        ui->screen_Master_cont_1,
        ui->screen_Master_cont_2,
        ui->screen_Master_cont_3,
        ui->screen_Master_cont_4
    };
    
    // 更新所有选项框的样式
    for(int i = 0; i < 4; i++) {
        if(i == selected_option) {
            // 高亮选中的选项：绿色边框，高亮背景，添加阴影
            lv_obj_set_style_border_width(option_boxes[i], 3, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(option_boxes[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(option_boxes[i], 8, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_color(option_boxes[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
        } else {
            // 普通样式：恢复原始颜色
            lv_obj_set_style_border_width(option_boxes[i], 2, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(option_boxes[i], lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_shadow_width(option_boxes[i], 0, LV_PART_MAIN|LV_STATE_DEFAULT);
            // 恢复原始背景色
            if(i == 0) lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0x5a9838), LV_PART_MAIN|LV_STATE_DEFAULT);
            else if(i == 1) lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0x2195f6), LV_PART_MAIN|LV_STATE_DEFAULT);
            else if(i == 2) lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0xff9800), LV_PART_MAIN|LV_STATE_DEFAULT);
            else if(i == 3) lv_obj_set_style_bg_color(option_boxes[i], lv_color_hex(0xf44336), LV_PART_MAIN|LV_STATE_DEFAULT);
        }
    }
}

void select_option(int new_index)
{
    if(new_index < 0 || new_index >= 4) return;
    
    selected_option = new_index;
    update_option_highlight(&guider_ui);
}

void select_control_option(int new_index)
{
    if(new_index < 0 || new_index >= 3) return;
    
    selected_control_option = new_index;
    guider_ui.screen_control_selected_index = new_index;
    update_control_option_highlight();
}

void update_control_option_highlight(void)
{
    for(int i = 0; i < 3; i++) {
        if(control_labels[i] == NULL || !lv_obj_is_valid(control_labels[i])) continue;
        
        if(i == selected_control_option) {
            lv_obj_set_style_bg_color(control_labels[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(control_labels[i], 255, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(control_labels[i], 2, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_color(control_labels[i], lv_color_hex(0x00ff00), LV_PART_MAIN|LV_STATE_DEFAULT);
        } else {
            lv_obj_set_style_bg_opa(control_labels[i], 0, LV_PART_MAIN|LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(control_labels[i], 0, LV_PART_MAIN|LV_STATE_DEFAULT);
        }
    }
}
