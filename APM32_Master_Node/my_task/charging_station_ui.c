#include "charging_station_ui.h"
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "rs485.h"



#define STATION_NUM 4

Station_Info_t stations[STATION_NUM] = {0};
static uint16_t system_total_power = 200;
static uint16_t system_used_power = 0;

// UI状态
typedef enum {
    UI_MAIN = 0,
    UI_MENU,
    UI_DETAIL,
    UI_SETTINGS,
    UI_POWER,
    UI_MONITOR,
    UI_CHARGING
} UIState;
static UIState ui_state = UI_MAIN;
static uint8_t selected_station = 0;
static uint8_t need_redraw = 1;

// 记录上一次选中的卡片索引
static uint8_t last_selected_station = 0;

// 菜单相关
static uint8_t menu_selected = 0;
static const char* menu_items[] = {"子站详情", "功率管理", "系统设置", "监控", "充电控制", "返回"};
static uint8_t menu_items_count = 6;

// 各页面绘制函数声明
static void draw_menu_screen(void);
static void draw_detail_screen(void);
static void draw_settings_screen(void);
static void draw_power_screen(void);
static void draw_monitor_screen(void);
static void draw_charging_screen(void);

// 颜色
#define COLOR_BLACK     0x0000
#define COLOR_WHITE     0xFFFF
#define COLOR_RED       0xF800
#define COLOR_GREEN     0x07E0
#define COLOR_BLUE      0x001F
#define COLOR_DARK_GRAY 0x4208
#define COLOR_ORANGE 0xFD20

typedef struct {
    const char *name;
    uint16_t power;
    uint16_t color;
} ChargeMode_t;

static const ChargeMode_t charge_modes[] = {
    {"标准充电", 25, COLOR_GREEN},
    {"快速充电", 90, COLOR_RED},
    {"放电测试", 30, COLOR_ORANGE}
};
#define CHARGE_MODE_NUM (sizeof(charge_modes)/sizeof(charge_modes[0]))
static uint8_t charge_mode_selected = 0;
static uint8_t charge_progress = 65;

// 记录上一次选中的充电模式索引
static uint8_t last_charge_mode_selected = 0;

// 尺寸
#define SCREEN_WIDTH        320
#define SCREEN_HEIGHT       240
#define HEADER_HEIGHT       60
#define STATION_GRID_X      10
#define STATION_GRID_Y      70
#define STATION_WIDTH       145
#define STATION_HEIGHT      70
#define STATION_MARGIN      10

// 内部函数声明
static void draw_main_screen(void);
static void draw_station_card(uint16_t x, uint16_t y, uint8_t idx, uint8_t selected);
static void draw_progress_bar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t progress, uint16_t color);
static void update_power(void);
static void update_station_selection(uint8_t new_selected);
static void update_charge_mode_selection(uint8_t old_sel, uint8_t new_sel);

// 初始化
void charging_station_ui_init(void)
{
    LCD_Init();
    LCD_Display_Dir(2);
    LCD_Clear(COLOR_BLACK);
    need_redraw = 1;
    update_power();
}

uint8_t read_key()
{
	
    uint8_t temp = KEY_NONE;

    if (HAL_GPIO_ReadPin(KEY_1_GPIO_Port, KEY_1_Pin) == 0)
        temp = KEY_UP;
    else if (HAL_GPIO_ReadPin(KEY_6_GPIO_Port, KEY_6_Pin) == 0)
        temp = KEY_MENU;
    else if (HAL_GPIO_ReadPin(KEY_5_GPIO_Port, KEY_5_Pin) == 0)
        temp = KEY_OK;
    else if (HAL_GPIO_ReadPin(KEY_3_GPIO_Port, KEY_3_Pin) == 0)
        temp = KEY_RIGHT;
    else if (HAL_GPIO_ReadPin(KEY_2_GPIO_Port, KEY_2_Pin) == 0)
        temp = KEY_DOWN;
    else if (HAL_GPIO_ReadPin(KEY_4_GPIO_Port, KEY_4_Pin) == 0)
        temp = KEY_LEFT;
    else if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9) == 0)
        temp = KEY_RETURN;

    return temp;
}
uint8_t key_val = 0, key_old = 0, key_down = 0;
// 主任务优化：导航时只重绘变化卡片，页面切换时才整体刷新
void charging_station_ui_task(void)
{
    charging_station_ui_init();
    while(1)
    {
        key_val = read_key();
        key_down = key_val & (key_val ^ key_old);
        key_old = key_val;

        // 按键导航和页面切换
        if(key_down) {
            switch(ui_state) {
                case UI_MAIN:
                {
                    uint8_t prev_selected = selected_station;
                    if(key_down == KEY_LEFT && selected_station % 2 == 1) selected_station--;
                    if(key_down == KEY_RIGHT && selected_station % 2 == 0) selected_station++;
                    if(key_down == KEY_UP && selected_station >= 2) selected_station -= 2;
                    if(key_down == KEY_DOWN && selected_station < 2) selected_station += 2;
                    if(prev_selected != selected_station) {
                        update_station_selection(selected_station);
                    }
                    if(key_down == KEY_MENU) {
                        ui_state = UI_MENU;
                        need_redraw = 1;
                    } else if(key_down == KEY_OK) {
                        ui_state = UI_DETAIL;
                        need_redraw = 1;
                    }
                    break;
                }
                case UI_MENU:
                {
                    if(key_down == KEY_UP && menu_selected > 0) menu_selected--;
                    if(key_down == KEY_DOWN && menu_selected < menu_items_count-1) menu_selected++;
                    if(key_down == KEY_OK) {
                        if(menu_selected == 0) ui_state = UI_DETAIL;
                        else if(menu_selected == 1) ui_state = UI_POWER;
                        else if(menu_selected == 2) ui_state = UI_SETTINGS;
                        else if(menu_selected == 3) ui_state = UI_MONITOR;
                        else if(menu_selected == 4) ui_state = UI_CHARGING;
                        else if(menu_selected == 5) ui_state = UI_MAIN;
                        need_redraw = 1;
                    }
                    if(key_down == KEY_RETURN) {
                        ui_state = UI_MAIN;
                        need_redraw = 1;
                    }
                    need_redraw = 1;
                    break;
                }
                case UI_DETAIL:
                    if(key_down == KEY_OK) {
                        ui_state = UI_CHARGING;
                        need_redraw = 1;
                    } else if(key_down == KEY_RETURN) {
                        ui_state = UI_MAIN;
                        need_redraw = 1;
                    }
                    break;
                case UI_SETTINGS:
                    if(key_down == KEY_RETURN) {
                        ui_state = UI_MENU;
                        need_redraw = 1;
                    }
                    break;
                case UI_POWER:
                    if(key_down == KEY_RETURN) {
                        ui_state = UI_MENU;
                        need_redraw = 1;
                    }
                    break;
                case UI_MONITOR:
                    if(key_down == KEY_RETURN) {
                        ui_state = UI_MENU;
                        need_redraw = 1;
                    }
                    break;
                case UI_CHARGING:
                    if(key_down == KEY_UP && charge_mode_selected > 0) {
                        uint8_t old_sel = charge_mode_selected;
                        charge_mode_selected--;
                        update_charge_mode_selection(old_sel, charge_mode_selected);
                        last_charge_mode_selected = charge_mode_selected;
                    }
                    if(key_down == KEY_DOWN && charge_mode_selected < CHARGE_MODE_NUM-1) {
                        uint8_t old_sel = charge_mode_selected;
                        charge_mode_selected++;
                        update_charge_mode_selection(old_sel, charge_mode_selected);
                        last_charge_mode_selected = charge_mode_selected;
                    }
                    if(key_down == KEY_RETURN) {
                        ui_state = UI_DETAIL;
                        need_redraw = 1;
                    }
										uint8_t send_order;
										if(key_down == KEY_OK && charge_mode_selected==0 )
										{
											send_order=1;
											RS485_Master_Send_Turn(0x01,&send_order,1);
											//标准充电
										}
										if(key_down == KEY_OK && charge_mode_selected==1 )
										{
											send_order=2;
											RS485_Master_Send_Turn(0x01,&send_order,1);

											//快速充电
										}
										if(key_down == KEY_OK && charge_mode_selected==2 )
										{
											//放电测试
										}
                    // KEY_OK 可扩展为启动充电
                    break;
                default:
                    break;
            }
        }

        if(need_redraw) {
            switch(ui_state) {
                case UI_MAIN: draw_main_screen(); break;
                case UI_MENU: draw_menu_screen(); break;
                case UI_DETAIL: draw_detail_screen(); break;
                case UI_SETTINGS: draw_settings_screen(); break;
                case UI_POWER: draw_power_screen(); break;
                case UI_MONITOR: draw_monitor_screen(); break;
                case UI_CHARGING: draw_charging_screen(); break;
                default: break;
            }
            need_redraw = 0;
        }
        vTaskDelay(100);
    }
}

// 优化主界面绘制：只刷新头部、卡片区和底部提示，不全屏清空
static void draw_main_screen(void)
{
    // 先清理全屏，防止残影
    LCD_Fill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, COLOR_BLACK);

    // 顶部黑色背景
    LCD_Fill(0, 0, SCREEN_WIDTH-1, HEADER_HEIGHT-1, COLOR_BLACK);
    // 居中显示“主站状态”
    POINT_COLOR = COLOR_GREEN;
    Show_Str(128, 8, (uint8_t*)"主站状态", 16, 0);
    // 居中显示绿色“● 正常运行”
    POINT_COLOR = COLOR_GREEN;
    Show_Str(120, 32, (uint8_t*)"● 正常运行", 12, 0);

    // 只刷新卡片区
    for(uint8_t i = 0; i < STATION_NUM; i++) {
        uint16_t x = STATION_GRID_X + (i % 2) * (STATION_WIDTH + STATION_MARGIN);
        uint16_t y = STATION_GRID_Y + (i / 2) * (STATION_HEIGHT + STATION_MARGIN);
        draw_station_card(x, y, i, (i == selected_station));
    }
}

// 优化导航：只重绘前后两个卡片
static void update_station_selection(uint8_t new_selected)
{
    uint16_t old_x = STATION_GRID_X + (last_selected_station % 2) * (STATION_WIDTH + STATION_MARGIN);
    uint16_t old_y = STATION_GRID_Y + (last_selected_station / 2) * (STATION_HEIGHT + STATION_MARGIN);
    draw_station_card(old_x, old_y, last_selected_station, 0);
    uint16_t new_x = STATION_GRID_X + (new_selected % 2) * (STATION_WIDTH + STATION_MARGIN);
    uint16_t new_y = STATION_GRID_Y + (new_selected / 2) * (STATION_HEIGHT + STATION_MARGIN);
    draw_station_card(new_x, new_y, new_selected, 1);
    last_selected_station = new_selected;
}

// 子站卡片
static void draw_station_card(uint16_t x, uint16_t y, uint8_t idx, uint8_t selected)
{
    Station_Info_t *s = &stations[idx];
    uint16_t card_color = selected ? COLOR_BLUE : COLOR_DARK_GRAY;
    LCD_Fill(x, y, x + STATION_WIDTH - 1, y + STATION_HEIGHT - 1, card_color);
    LCD_DrawRectangle(x, y, x + STATION_WIDTH - 1, y + STATION_HEIGHT - 1);

    char title[16];
    sprintf(title, "子站 %02d", idx+1);
    Show_Str(x + 5, y + 5, (uint8_t*)title, 12, 0);

    char status_str[32];
    if(s->status == 1) sprintf(status_str, "充电中 %.1fA", s->current);
    else if(s->status == 0) sprintf(status_str, s->battery_connected ? "空闲" : "未连接");
    else if(s->status == 2) sprintf(status_str, "故障");
    else sprintf(status_str, "完成");
    Show_Str(x + 5, y + 22, (uint8_t*)status_str, 10, 0);

    char detail_str[32];
    if(s->status == 1)
        sprintf(detail_str, "%.1fV | %dW", s->voltage, (int)s->power);
    else if(s->battery_connected)
        sprintf(detail_str, "●电池已连接");
    else
        sprintf(detail_str, "○未连接");
    Show_Str(x + 5, y + 38, (uint8_t*)detail_str, 10, 0);

    if(s->status == 1)
        draw_progress_bar(x + 10, y + STATION_HEIGHT - 14, STATION_WIDTH - 20, 6, s->progress, COLOR_GREEN);
}

// 进度条
static void draw_progress_bar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t progress, uint16_t color)
{
    LCD_Fill(x, y, x + w - 1, y + h - 1, COLOR_DARK_GRAY);
    uint16_t pw = (progress * w) / 100;
    if(pw > 0)
        LCD_Fill(x, y, x + pw - 1, y + h - 1, color);
}

// 功率统计
static void update_power(void)
{
    system_used_power = 0;
    for(uint8_t i = 0; i < STATION_NUM; i++)
        if(stations[i].status == 1)
            system_used_power += (uint16_t)stations[i].power;
}

// 外部接口实现
void set_station_status(uint8_t station_id, uint8_t status)
{
    if(station_id < STATION_NUM) {
        stations[station_id].status = status;
        update_power();
        need_redraw = 1;
    }
}
void set_station_voltage(uint8_t station_id, float voltage)
{
    if(station_id < STATION_NUM) {
        stations[station_id].voltage = voltage;
        need_redraw = 1;
    }
}
void set_station_current(uint8_t station_id, float current)
{
    if(station_id < STATION_NUM) {
        stations[station_id].current = current;
        need_redraw = 1;
    }
}
void set_station_power(uint8_t station_id, float power)
{
    if(station_id < STATION_NUM) {
        stations[station_id].power = power;
        update_power();
        need_redraw = 1;
    }
}
void set_station_progress(uint8_t station_id, uint8_t progress)
{
    if(station_id < STATION_NUM) {
        stations[station_id].progress = progress;
        need_redraw = 1;
    }
}
void set_battery_connected(uint8_t station_id, uint8_t connected)
{
    if(station_id < STATION_NUM) {
        stations[station_id].battery_connected = connected;
        need_redraw = 1;
    }
}
void set_system_power(uint16_t total_power, uint16_t used_power)
{
    system_total_power = total_power;
    system_used_power = used_power;
    need_redraw = 1;
}

// 菜单页面
static void draw_menu_screen(void)
{
    LCD_Fill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, COLOR_BLACK);
    Show_Str(10, 8, (uint8_t*)"主菜单", 16, 0);
    for(uint8_t i=0; i<menu_items_count; i++) {
        uint16_t y = 40 + i*30;
        uint16_t color = (i == menu_selected) ? COLOR_BLUE : COLOR_DARK_GRAY;
        LCD_Fill(20, y, SCREEN_WIDTH-20, y+24, color);
        Show_Str(30, y+6, (uint8_t*)menu_items[i], 12, 0);
    }
    Show_Str(10, 220, (uint8_t*)"↑↓选择 OK进入 RETURN返回", 10, 0);
}

// 详情页面
static void draw_detail_screen(void)
{
    LCD_Fill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, COLOR_BLACK);
    Show_Str(10, 8, (uint8_t*)"子站详情", 16, 0);

    // 获取当前选中子站信息
    Station_Info_t *s = &stations[selected_station];

    char info_str[32];
    sprintf(info_str, "输出电压: %.2f V", s->voltage);
    Show_Str(10, 40, (uint8_t*)info_str, 12, 0);

    sprintf(info_str, "输出电流: %.2f A", s->current);
    Show_Str(10, 65, (uint8_t*)info_str, 12, 0);

    sprintf(info_str, "输出功率: %d W", (int)(s->power));
    Show_Str(10, 90, (uint8_t*)info_str, 12, 0);

    Show_Str(10, 130, (uint8_t*)"返回: RETURN", 12, 0);
}

// 设置页面
static void draw_settings_screen(void)
{
    LCD_Fill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, COLOR_BLACK);
    Show_Str(10, 8, (uint8_t*)"系统设置(示例)", 16, 0);
    Show_Str(10, 40, (uint8_t*)"返回: RETURN", 12, 0);
}

// 功率管理页面
static void draw_power_screen(void)
{
    LCD_Fill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, COLOR_BLACK);
    Show_Str(10, 8, (uint8_t*)"功率管理(示例)", 16, 0);
    Show_Str(10, 40, (uint8_t*)"返回: RETURN", 12, 0);
}

// 监控页面
static void draw_monitor_screen(void)
{
    LCD_Fill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, COLOR_BLACK);
    Show_Str(10, 8, (uint8_t*)"系统监控(示例)", 16, 0);
    Show_Str(10, 40, (uint8_t*)"返回: RETURN", 12, 0);
}

// 充电控制页面
static void draw_charging_screen(void)
{
    LCD_Fill(0, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-1, COLOR_BLACK);

    // 顶部标题
    Show_Str(100, 8, (uint8_t*)"快速充电", 16, 0);

    // 进度条
    uint16_t bar_x = 20, bar_y = 40, bar_w = 280, bar_h = 16;
    LCD_Fill(bar_x, bar_y, bar_x+bar_w-1, bar_y+bar_h-1, COLOR_DARK_GRAY);
    uint16_t fill_w = (charge_progress * bar_w) / 100;
    LCD_Fill(bar_x, bar_y, bar_x+fill_w-1, bar_y+bar_h-1, COLOR_GREEN);

    // 进度百分比
    char progress_str[32];
    sprintf(progress_str, "已充电 %d%%", charge_progress);
    Show_Str(120, 62, (uint8_t*)progress_str, 12, 0);

    // 充电模式列表
    Show_Str(20, 90, (uint8_t*)"充电模式", 12, 0);
    for(uint8_t i=0; i<CHARGE_MODE_NUM; i++) {
        uint16_t y = 110 + i*28;
        uint16_t bg = (i == charge_mode_selected) ? COLOR_BLUE : COLOR_DARK_GRAY;
        LCD_Fill(20, y, 220, y+24, bg);
        Show_Str(30, y+6, (uint8_t*)charge_modes[i].name, 12, 0);
        char power_str[16];
        sprintf(power_str, "%dW", charge_modes[i].power);
        POINT_COLOR = charge_modes[i].color;
        Show_Str(180, y+6, (uint8_t*)power_str, 12, 0);
    }

    // 底部操作提示
    Show_Str(20, 210, (uint8_t*)"↑↓选择模式 OK启动 BACK返回", 10, 0);
}

static void update_charge_mode_selection(uint8_t old_sel, uint8_t new_sel)
{
    for (uint8_t i = 0; i < CHARGE_MODE_NUM; i++) {
        if (i == old_sel || i == new_sel) {
            uint16_t y = 110 + i*28;
            uint16_t bg = (i == new_sel) ? COLOR_BLUE : COLOR_DARK_GRAY;
            LCD_Fill(20, y, SCREEN_WIDTH-20, y+24, bg);
            Show_Str(30, y+6, (uint8_t*)charge_modes[i].name, 12, 0);
            char power_str[16];
            sprintf(power_str, "%dW", charge_modes[i].power);
            POINT_COLOR = charge_modes[i].color;
            Show_Str(180, y+6, (uint8_t*)power_str, 12, 0);
        }
    }
}