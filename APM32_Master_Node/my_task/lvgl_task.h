#ifndef LVGL_TASK_H
#define LVGL_TASK_H



// ���尴��ö��
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
void lvgl_task(void);
void Lvgl_init(void);


#endif