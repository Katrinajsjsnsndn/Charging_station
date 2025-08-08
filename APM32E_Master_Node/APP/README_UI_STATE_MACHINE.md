# UI状态机模块使用说明

## 概述

UI状态机模块 (`ui_state_machine.h` 和 `ui_state_machine.c`) 提供了一个结构化的方法来管理多页面UI应用程序的状态和转换。该模块基于状态机设计模式，提供了清晰的代码组织和良好的扩展性。

## 文件结构

```
APP/
├── ui_state_machine.h          # 状态机头文件
├── ui_state_machine.c          # 状态机实现文件
├── charging_station_ui.h       # 充电站UI接口
├── charging_station_ui.c       # 充电站UI实现
└── README_UI_STATE_MACHINE.md  # 本文档
```

## 核心概念

### 1. 页面状态 (Page State)
```c
typedef enum {
    PAGE_MAIN = 0,           // 主页面
    PAGE_DETAIL,             // 详情页面
    PAGE_CONTROL,            // 控制页面
    PAGE_MENU,               // 菜单页面
    PAGE_STATIONS,           // 站点管理页面
    PAGE_POWER_MANAGEMENT,   // 电源管理页面
    PAGE_BATTERY_TEST,       // 电池测试页面
    PAGE_BATTERY_MAINTENANCE,// 电池维护页面
    PAGE_MONITOR,            // 监控页面
    PAGE_SETTINGS,           // 设置页面
    PAGE_STATION_DETAIL,     // 站点详情页面
    PAGE_COUNT               // 页面总数
} page_state_t;
```

### 2. 按键事件 (Key Event)
```c
typedef enum {
    KEY_EVENT_NONE = 0,      // 无事件
    KEY_EVENT_UP,            // 上键
    KEY_EVENT_DOWN,          // 下键
    KEY_EVENT_LEFT,          // 左键
    KEY_EVENT_RIGHT,         // 右键
    KEY_EVENT_OK,            // 确认键
    KEY_EVENT_RETURN,        // 返回键
    KEY_EVENT_MENU,          // 菜单键
    KEY_EVENT_COUNT          // 事件总数
} key_event_t;
```

### 3. UI上下文 (UI Context)
```c
typedef struct {
    page_state_t current_page;    // 当前页面
    page_state_t previous_page;   // 上一个页面
    int selected_item;            // 选中的项目
    int selected_station;         // 选中的站点
    int selected_mode;            // 选中的模式
    bool page_initialized;        // 页面是否已初始化
    uint32_t last_update_time;    // 最后更新时间
} ui_context_t;
```

## 使用方法

### 1. 在main.c中集成

```c
#include "ui_state_machine.h"

// 在任务创建中使用Lvgl_ui_task
xTaskCreate(Lvgl_ui_task,
           "Lvgl_ui_task",
           4096,
           NULL,
           2,
           &xHandleLVGL);
```

注意：`Lvgl_ui_task`函数在main.c中实现，它使用状态机模块的核心功能。

### 2. 添加新页面

#### 步骤1: 定义页面状态
在 `ui_state_machine.h` 中添加新的页面状态：
```c
typedef enum {
    // ... 现有页面
    PAGE_NEW_FEATURE,        // 新功能页面
    PAGE_COUNT
} page_state_t;
```

#### 步骤2: 实现页面处理函数
在 `ui_state_machine.c` 中实现页面处理函数：
```c
static void new_feature_page_init(ui_context_t* ctx)
{
    // 初始化新页面UI
    lv_scr_load(guider_ui.screen_new_feature);
    lv_refr_now(NULL);
}

static void new_feature_page_update(ui_context_t* ctx)
{
    // 页面动态更新逻辑
}

static void new_feature_page_handle_key(ui_context_t* ctx, key_event_t event)
{
    // 页面内部按键处理
    switch (event) {
        case KEY_EVENT_UP:
            // 处理上键
            break;
        case KEY_EVENT_DOWN:
            // 处理下键
            break;
    }
}

static void new_feature_page_exit(ui_context_t* ctx)
{
    // 清理页面资源
}
```

#### 步骤3: 注册页面处理器
在 `page_handlers` 数组中注册新页面：
```c
static const page_handler_t page_handlers[PAGE_COUNT] = {
    // ... 现有页面
    [PAGE_NEW_FEATURE] = {
        .init = new_feature_page_init,
        .update = new_feature_page_update,
        .handle_key = new_feature_page_handle_key,
        .exit = new_feature_page_exit
    },
};
```

#### 步骤4: 定义状态转换
在 `state_transitions` 数组中添加页面转换规则：
```c
static const state_transition_t state_transitions[] = {
    // ... 现有转换
    {PAGE_MAIN, KEY_EVENT_MENU, PAGE_NEW_FEATURE, NULL},
    {PAGE_NEW_FEATURE, KEY_EVENT_RETURN, PAGE_MAIN, NULL},
    // 结束标记
    {PAGE_COUNT, KEY_EVENT_NONE, PAGE_COUNT, NULL}
};
```

### 3. 动态数据更新

在 `update_dynamic_data` 函数中添加新页面的数据更新：
```c
void update_dynamic_data(ui_context_t* ctx)
{
    // ... 现有更新逻辑
    
    // 新页面数据更新
    if (ctx->current_page == PAGE_NEW_FEATURE) {
        // 更新新页面的动态数据
    }
}
```

## 状态机工作流程

1. **初始化**: `ui_state_machine_init()` 初始化UI上下文和LVGL
2. **事件处理**: `ui_state_machine_process()` 处理按键事件和状态转换
3. **页面更新**: `ui_page_update()` 管理页面初始化和更新
4. **数据更新**: `update_dynamic_data()` 更新动态显示数据

## 优势

### 1. 代码组织
- 每个页面的逻辑独立封装
- 清晰的状态转换规则
- 统一的按键处理机制
- 任务循环在main.c中，便于调试和修改

### 2. 可维护性
- 添加新页面只需实现对应的处理函数
- 状态转换规则集中管理
- 减少代码重复
- 状态机核心逻辑与任务循环分离

### 3. 可扩展性
- 易于添加新的页面和功能
- 支持复杂的页面导航逻辑
- 灵活的事件处理机制
- 其他任务可以方便地调用状态机函数

### 4. 调试友好
- 清晰的状态跟踪
- 独立的页面测试
- 统一的错误处理
- 任务循环在main.c中，便于添加调试代码

## 注意事项

1. **内存管理**: 确保页面切换时正确清理资源
2. **状态同步**: 保持UI状态与内部状态的一致性
3. **按键映射**: 正确映射硬件按键到事件类型
4. **性能优化**: 避免在页面更新函数中进行耗时操作

## 示例

参考 `ui_state_machine.c` 中的 `main_page_*`、`detail_page_*`、`control_page_*` 函数实现，了解如何正确实现页面处理函数。

## 扩展建议

1. **添加页面动画**: 在页面切换时添加过渡动画
2. **实现页面栈**: 支持更复杂的导航历史
3. **添加配置系统**: 支持动态配置页面行为
4. **实现事件系统**: 支持页面间的事件通信
