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

// ���մ�����
void RS485_Master_Receive_Process(void) {
    if (rx_done) 
		{  // ����Ƿ���յ���������

        memcpy(&frame, rx_buffer, sizeof(frame));  // �����յ������ݿ�����֡�ṹ��

        // ��֤֡ͷ
        if (frame.head != RS485_FRAME_HEAD) 
				{
            // ֡ͷ���󣬶�������
            return;
        }

        // ��֤У���
        uint8_t calc_checksum = RS485_CalcChecksum((uint8_t*)&frame, sizeof(frame) - sizeof(frame.checksum));
        if (frame.checksum != calc_checksum) {
            // У��ʹ��󣬶�������
            return;
        }

        // �����������ʹ���
        switch (frame.cmd) 
			 {
            case CMD_SET_PARAM:  // ���ò�������
                // �������ò�������
                // ����ʵ������ʵ�־����߼�
                // ���磺���� frame.data �����豸״̬
                break;

            case CMD_READ_DATA:  // ��Ӧ�ɹ�����
//                // ������Ӧ�ɹ�����
//                // ����ʵ������ʵ�־����߼�
                break;

            default:
                // δ֪�����������
                break;
        }
			        rx_done = 0;  // ���������ɱ�־
 
    }
}