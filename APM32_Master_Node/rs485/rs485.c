#include "rs485.h"

uint8_t tx_buf[RS485_MAX_FRAME_LEN];
uint8_t rx_buf[RS485_MAX_FRAME_LEN];

void RS485_Master_Send_Turn(uint8_t slave_addr,uint8_t device_switch) 
{
		HAL_GPIO_WritePin(RS485_EN_GPIO_Port,RS485_EN_Pin,1);

    RS485_Frame_t frame;
    frame.head = RS485_FRAME_HEAD;
    frame.addr_to = slave_addr;
    frame.addr_from = RS485_ADDR_MASTER;
    frame.cmd = CMD_SET_PARAM;
    frame.len = 1;
		frame.data =device_switch;
    frame.checksum = RS485_CalcChecksum((uint8_t*)&frame, 6);

    memcpy(tx_buf, &frame, 7); 
    HAL_UART_Transmit_DMA(&huart2, tx_buf, 7);
}
uint8_t RS485_CalcChecksum(const uint8_t *buf, uint8_t len) 
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < len; i++) {
        sum += buf[i];
    }
    return (uint8_t)sum;
}