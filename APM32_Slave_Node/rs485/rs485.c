#include "rs485.h"
#include "charge_control.h"
extern uint16_t dac_set;
extern float current_set;
void Data_Feedback(void);

uint8_t tx_buf[RS485_MAX_FRAME_LEN];
uint8_t rx_buf[RS485_MAX_FRAME_LEN];
uint8_t data_feedback_flag=1;

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
										current_set=1;
										dac_set=(uint16_t)(((current_set*0.2f)/3.3f)*4095);
                    MCP4725_WriteData_Digital(dac_set);
                }
                else if (param == 2)
                { 
										Enable_Charging();
										current_set=1.5;
										dac_set=(uint16_t)(((current_set*0.2f)/3.3f)*4095);
                    MCP4725_WriteData_Digital(dac_set);
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
extern float read_current,read_val;
void Data_Feedback() {
    if (data_feedback_flag != 1) {
        return;
    }

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_SET); // ʹ�� RS485 ����
    
    uint8_t txBuf[20]; // ���������㹻������ 11 �ֽڣ�֡ͷ + 2��float + У��ͣ�
    RS485_Frame_t *f = (RS485_Frame_t *)txBuf;

    // ���֡ͷ
    f->head      = 0xAA;
    f->addr_to   = 0x00;
    f->addr_from = 0x01;
    f->cmd       = CMD_READ_DATA; 
    f->len       = 8;    // 2��floatռ8�ֽ�

    // �� float ת��Ϊ�ֽ�����С��ģʽ��
    memcpy(&f->data[0], &read_current, 4); // ���� read_current ��4�ֽ�
    memcpy(&f->data[4], &read_val, 4);     // ���� read_val ��4�ֽ�

    // ����У��ͣ�֡ͷ + ���ݣ�
    uint8_t checksum = RS485_CalcChecksum(txBuf, sizeof(RS485_Frame_t) + f->len);
    txBuf[sizeof(RS485_Frame_t) + f->len] = checksum;

    // �������ݣ�֡ͷ + ���� + У��ͣ�
    HAL_UART_Transmit(&huart2, txBuf, sizeof(RS485_Frame_t) + f->len + 1, 100);
    // �ȴ�������ɣ���ʱʱ����ݲ����ʵ�����
			while (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_TC) == RESET) {
					// ��ѡ����ʱ����
			}

			HAL_GPIO_WritePin(GPIOA, GPIO_PIN_15, GPIO_PIN_RESET); // ��ȫ�л�
}