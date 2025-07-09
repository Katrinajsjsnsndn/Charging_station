#include "rs485.h"

uint8_t tx_buf[RS485_MAX_FRAME_LEN];
uint8_t rx_buf[RS485_MAX_FRAME_LEN];

void RS485_Master_SendReadCmd(uint8_t slave_addr) {
    RS485_Frame_t frame;
    frame.head = RS485_FRAME_HEAD;
    frame.addr_to = slave_addr;
    frame.addr_from = RS485_ADDR_MASTER;
    frame.cmd = CMD_READ_DATA;
    frame.len = 0;
    frame.checksum = RS485_CalcChecksum((uint8_t*)&frame, 5);

    memcpy(tx_buf, &frame, 6); 
    HAL_UART_Transmit(&huart2, tx_buf, 6, 100);
}
uint8_t RS485_CalcChecksum(const uint8_t *buf, uint8_t len) 
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < len; i++) {
        sum += buf[i];
    }
    return (uint8_t)sum;
}

RS485_Frame_t frame;

// 接收处理函数
void RS485_Master_Receive_Process(void) {
    if (rx_done) 
		{  // 检查是否接收到完整数据

        memcpy(&frame, rx_buffer, sizeof(frame));  // 将接收到的数据拷贝到帧结构中

        // 验证帧头
        if (frame.head != RS485_FRAME_HEAD) 
				{
            // 帧头错误，丢弃数据
            return;
        }

        // 验证校验和
        uint8_t calc_checksum = RS485_CalcChecksum((uint8_t*)&frame, sizeof(frame) - sizeof(frame.checksum));
        if (frame.checksum != calc_checksum) {
            // 校验和错误，丢弃数据
            return;
        }

        // 根据命令类型处理
        switch (frame.cmd) 
			 {
            case CMD_SET_PARAM:  // 设置参数命令
                // 处理设置参数命令
                // 根据实际需求实现具体逻辑
                // 例如：根据 frame.data 设置设备状态
                break;

            case CMD_READ_DATA:  // 响应成功命令
//                // 处理响应成功命令
//                // 根据实际需求实现具体逻辑
                break;

            default:
                // 未知命令，丢弃数据
                break;
        }
			        rx_done = 0;  // 清除接收完成标志
 
    }
}