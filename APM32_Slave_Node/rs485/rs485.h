
#ifndef __RS485_H
#define __RS485_H


#include "stdio.h"
#include "string.h"
#include "usart.h"

#define RS485_FRAME_HEAD    0xAA

#define RS485_ADDR_MASTER   0x00
#define RS485_ADDR_SLAVE1   0x01
#define RS485_ADDR_SLAVE2   0x02
#define RS485_ADDR_SLAVE3   0x03

#define CMD_READ_DATA       0x10
#define CMD_READ_RESP       0x11
#define CMD_SET_PARAM       0x20
#define CMD_SET_RESP        0x21
#define CMD_FEEDBACK        0x30
#define CMD_CTRL_RESP       0x31

#define RS485_MAX_DATA_LEN  1
#define RS485_MAX_FRAME_LEN (6 + RS485_MAX_DATA_LEN)

typedef struct {
    uint8_t head;      // 帧头
    uint8_t addr_to;   // 目标地址
    uint8_t addr_from; // 源地址
    uint8_t cmd;       // 命令
    uint8_t len;       // 后续 data[] 的字节数
    uint8_t data[];    // 柔性数组：实际数据紧跟其后
    /* checksum 紧跟在 data[] 后面，不放在结构体里 */
} RS485_Frame_t;

uint8_t RS485_CalcChecksum(const uint8_t *buf, uint8_t len);
void RS485_Master_Receive_Process(void);
void Data_Feedback(void);

#endif
