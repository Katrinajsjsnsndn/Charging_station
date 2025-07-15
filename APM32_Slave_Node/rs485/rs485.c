#include "rs485.h"
#include "charge_control.h"
extern uint16_t dac_set;
void Data_Feedback(void);

uint8_t tx_buf[RS485_MAX_FRAME_LEN];
uint8_t rx_buf[RS485_MAX_FRAME_LEN];
uint8_t data_feedback_flag;

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

/* ----------------------------------------------------------
 * 2. ���մ�������ͷ-len-data-checksum����ʽ����
 * ---------------------------------------------------------- */
void RS485_Master_Receive_Process(void)
{
    if (!rx_done) return;

    /* ��С���ȣ��̶�ͷ 5 �ֽ� + ���� 1 �ֽ� data + 1 �ֽ� checksum */
    if (rx_len < 7) goto frame_err;

    RS485_Frame_t *pHdr = (RS485_Frame_t *)rx_buffer;

    /* 2.1 ֡ͷ��� */
    if (pHdr->head != RS485_FRAME_HEAD) goto frame_err;

    /* 2.2 ���ȼ�� */
    uint16_t expect_len = 5 + pHdr->len + 1;   /* 5�̶� + data[len] + checksum */
    if (rx_len != expect_len) goto frame_err;

    /* 2.3 У��ͼ�飺У�鷶Χ = ����֡�������һ���ֽڣ� */
    uint8_t calc_cs = RS485_CalcChecksum(rx_buffer, expect_len - 1);
    if (calc_cs != rx_buffer[expect_len - 1]) goto frame_err;

    /* 2.4 ����ַ� */
    switch (pHdr->cmd)
    {
        case CMD_SET_PARAM:
            if (pHdr->len >= 1)          /* ���� 1 �ֽڲ��� */
            {
                uint8_t param = rx_buffer[5];   /* data ��ʼλ�� */
                if (param == 1)
                {
                    Enable_Charging();
                    MCP4725_WriteData_Digital(dac_set);
                }
                else if (param == 0)
                {
                    Disable_Charging();
                }
                else if (param == 3)
                {
                    data_feedback_flag = 1;
                }
            }
            break;

        case CMD_READ_DATA:
            /* TODO����Э��ظ� */
            break;

        default:
            break;
    }

frame_err:
    /* 3. ����������һ�� DMA ���� */
    rx_done = 0;
    rx_len  = 0;
    HAL_UART_Receive_DMA(&huart2, rx_buffer, BUFFER_SIZE);
}
uint8_t text_num0=12,text_num1=13;
void Data_Feedback()
{
	if(data_feedback_flag!=1)
	{
		return;
	}
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET);
		uint8_t txBuf[15];               // 128 �ֽ���ʱ������
		RS485_Frame_t *f = (RS485_Frame_t *)txBuf;

		f->head     = 0xAA;
		f->addr_to  = 0x00;
		f->addr_from= 0x01;
		f->cmd      = 0x03;
		f->len      = 3;                  // Ҫ�� 6 �ֽ�����
		f->data[0]  = 0x11;               // �� 1 �ֽ�
		f->data[1]  = 0x22;               // �� 2 �ֽ�
		f->data[2]  = 0x66;               // �� 6 �ֽ�

		uint8_t checksum = RS485_CalcChecksum(txBuf, sizeof(RS485_Frame_t) + f->len);
		txBuf[sizeof(RS485_Frame_t) + f->len] = checksum;

		HAL_UART_Transmit(&huart2, txBuf, sizeof(RS485_Frame_t) + f->len + 1,100);
		
}