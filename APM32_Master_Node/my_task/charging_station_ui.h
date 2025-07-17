#ifndef CHARGING_STATION_UI_H
#define CHARGING_STATION_UI_H

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
// UI主任务初始化与循环
void charging_station_ui_init(void);
void charging_station_ui_task(void);

// 外部接口：用于其他任务/协议更新UI数据
void set_station_status(uint8_t station_id, uint8_t status);
void set_station_voltage(uint8_t station_id, float voltage);
void set_station_current(uint8_t station_id, float current);
void set_station_power(uint8_t station_id, float power);
void set_station_progress(uint8_t station_id, uint8_t progress);
void set_battery_connected(uint8_t station_id, uint8_t connected);
void set_system_power(uint16_t total_power, uint16_t used_power);
// 定义按键枚举
typedef enum {
    KEY_NONE = 0,
    KEY_RETURN = 1,
    KEY_MENU = 2,
    KEY_OK = 3,
    KEY_LEFT = 4,
    KEY_RIGHT = 5,
    KEY_DOWN = 6,
    KEY_UP = 7
} KeyEnum;
// 状态定义（可用于外部调用）
#define STATION_STATUS_IDLE        0
#define STATION_STATUS_CHARGING    1
#define STATION_STATUS_ERROR       2
#define STATION_STATUS_COMPLETE    3

// 充电模式定义（如需外部使用，可补充）
#define CHARGE_MODE_STANDARD       0
#define CHARGE_MODE_FAST           1
#define CHARGE_MODE_DISCHARGE      2
#define CHARGE_MODE_MAINTENANCE    3

#endif // CHARGING_STATION_UI_H
