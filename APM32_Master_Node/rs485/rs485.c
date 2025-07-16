#include "rs485.h"

uint8_t tx_buf[RS485_MAX_FRAME_LEN];
uint8_t rx_buf[RS485_MAX_FRAME_LEN];
/**
 * @brief  主机向从机发送“设置参数”命令
 * @param  slave_addr   目标从机地址
 * @param  p_data       指向要发送的数据缓冲区
 * @param  data_len     数据长度（≤ 255）
 */
void RS485_Master_Send_Turn(uint8_t slave_addr,
                            const uint8_t *p_data,
                            uint8_t data_len)
{
    /* 1. 切到发送模式 */
    HAL_GPIO_WritePin(RS485_EN_GPIO_Port, RS485_EN_Pin, GPIO_PIN_SET);

    /* 2. 固定头 */
    tx_buf[0] = RS485_FRAME_HEAD;
    tx_buf[1] = slave_addr;
    tx_buf[2] = RS485_ADDR_MASTER;
    tx_buf[3] = CMD_SET_PARAM;
    tx_buf[4] = data_len;               /* len */

    /* 3. 拷贝数据 */
    memcpy(&tx_buf[5], p_data, data_len);

    /* 4. 计算校验：固定头 5 字节 + data 字节 */
    uint8_t cs = RS485_CalcChecksum(tx_buf, 5 + data_len);
    tx_buf[5 + data_len] = cs;

    /* 5. 启动 DMA 发送 */
    HAL_UART_Transmit(&huart2, tx_buf, 6 + data_len,10);   /* 5 + len + checksum */
}
uint8_t RS485_CalcChecksum(const uint8_t *buf, uint8_t len) 
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < len; i++) {
        sum += buf[i];
    }
    return (uint8_t)sum;
}