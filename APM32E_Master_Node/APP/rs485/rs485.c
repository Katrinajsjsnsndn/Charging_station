#include "rs485.h"
#include "charging_station_ui.h"

uint8_t tx_buf[RS485_MAX_FRAME_LEN];
uint8_t rx_buf[RS485_MAX_FRAME_LEN];


extern uint8_t usart2_rx_buffer[];
extern volatile uint16_t usart2_rx_len;
extern volatile uint8_t usart2_rx_done;
#define USART2_RX_BUFFER_SIZE 128

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
    GPIO_SetBit(GPIOA, GPIO_PIN_11);

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

    // 3. 逐字节发送
    uint16_t total_len = 6 + data_len;
    for (uint16_t i = 0; i < total_len; i++)
    {
        // 等待发送缓冲区空
        USART_TxData(USART2, tx_buf[i]);
			  while (USART_ReadStatusFlag(USART2, USART_FLAG_TXBE) == RESET);
    }
				vTaskDelay(2);

    // 4. 关闭RS485发送（拉低EN）
     GPIO_ResetBit(GPIOA, GPIO_PIN_11);
}
uint8_t RS485_CalcChecksum(const uint8_t *buf, uint8_t len) 
{
    uint16_t sum = 0;
    for (uint8_t i = 0; i < len; i++) {
        sum += buf[i];
    }
    return (uint8_t)sum;
}

RS485_Frame_t *pHdr;
// 接收处理函数
float receive_current, receive_val;
extern Station_Info_t stations[STATION_NUM] ;

/* ----------------------------------------------------------
 * 2. 接收处理：按“头-len-data-checksum”方式解析
 * ---------------------------------------------------------- */
void RS485_Master_Receive_Process(void)
{
    if (!usart2_rx_done) return;

    /* 最小长度：固定头 5 字节 + 至少 1 字节 data + 1 字节 checksum */
    if (usart2_rx_len < 7) goto frame_err;

     pHdr = (RS485_Frame_t *)usart2_rx_buffer;

    /* 2.1 帧头检查 */
    if (pHdr->head != RS485_FRAME_HEAD) goto frame_err;

    /* 2.2 长度检查 */
    uint16_t expect_len = 5 + pHdr->len + 1;   /* 5固定 + data[len] + checksum */
    if (usart2_rx_len != expect_len) goto frame_err;

    /* 2.3 校验和检查：校验范围 = 整个帧（除最后一个字节） */
    uint8_t calc_cs = RS485_CalcChecksum(usart2_rx_buffer, expect_len - 1);
    if (calc_cs != usart2_rx_buffer[expect_len - 1]) goto frame_err;

   /* 5. 命令分发 */
    switch (pHdr->cmd) {
        case CMD_SET_PARAM:
            if (pHdr->len >= 1) {
                uint8_t param = pHdr->data[0]; // 直接通过结构体访问
                switch (param) {
                    case 0: /* 处理 param=0 */ break;
                    case 1: /* 处理 param=1 */ break;
                    case 3: /* 处理 param=3 */ break;
                    default: /* 无效参数 */ break;
                }
            }
            break;

        case CMD_READ_DATA:
            // 示例：回复当前数据（read_current 和 read_val）
							memcpy(&receive_current, &pHdr->data[0], 4); // 解析第1个float（read_current）
							memcpy(&receive_val, &pHdr->data[4], 4);     // 解析第2个float（read_val）

            break;

        default:
            /* 未知命令 */
            break;
    }
		if(pHdr->addr_from==0x01)
		{
				stations[0].battery_connected = 1;
			
				stations[0].current = receive_current;
				stations[0].voltage = receive_val;
				stations[0].power = receive_current * receive_val;

				// 状态判断
				if(stations[0].battery_connected) 
				{
						if(stations[0].current > 0.1f) {
								stations[0].status = 1; // 充电中
							
						} else {
								stations[0].status = 0; // 空闲
						}
				} 
				else 
				{
						stations[0].status = 0; // 未连接
				}
	

		}
frame_err:
    /* 3. 重新启动下一轮 DMA 接收 */
    usart2_rx_done = 0;
    usart2_rx_len  = 0;
    //HAL_UART_Receive_DMA(&huart2, rx_buffer, BUFFER_SIZE);
}



