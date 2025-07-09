
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
#define CMD_CTRL            0x30
#define CMD_CTRL_RESP       0x31

#define RS485_MAX_DATA_LEN  32
#define RS485_MAX_FRAME_LEN (6 + RS485_MAX_DATA_LEN)

typedef struct {
    uint8_t head;
    uint8_t addr_to;
    uint8_t addr_from;
    uint8_t cmd;
    uint8_t len;
    uint8_t data[RS485_MAX_DATA_LEN];
    uint8_t checksum;
} RS485_Frame_t;

uint8_t RS485_CalcChecksum(const uint8_t *buf, uint8_t len);
void RS485_Master_Receive_Process(void);

#endif
