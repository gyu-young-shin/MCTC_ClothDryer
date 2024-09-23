#ifndef __SEGMENT_H
#define __SEGMENT_H
#include "main.h"

#define FND_0   0x3F
#define FND_1   0x06
#define FND_2   0x5B
#define FND_3   0x4F
#define FND_4   0x66
#define FND_5   0x6D
#define FND_6   0x7D
#define FND_7   0x07
#define FND_8   0x7F
#define FND_9   0x6F
#define FND_A   0x77
#define FND_b   0x7C
#define FND_C   0x39
#define FND_c   0x58
#define FND_u   0x1C
#define FND_d   0x5E
#define FND_E   0x79
#define FND_F   0x71
#define FND_P   0x73
#define FND_n   0x54
#define FND_o   0x5C
#define FND_r   0x50
#define FND_L   0x38
#define FND_i   0x04
#define FND_H   0x76
#define FND_h   0x74
#define FND_t   0x78
#define FND_S   0x6D
#define FND_BAR 0x40
#define FND_UBAR 0x08
#define BLANK   0x00
#define FND_DOT 0x80

//---------------------------------------------------------   Address 0
#define LEDC_AUTO		0x80				// �ڵ�
//---------------------------------------------------------   Address 1
#define LEDC_RESERVE   	0x01				// ����
#define LEDC_DRY    	0x02				// ����
#define LEDC_TEMP  		0x04				// �µ�
#define LEDC_TIME   	0x08				// �ð�
//---------------------------------------------------------   Address 2
#define LEDC_STABIL		0x80				// ���
//---------------------------------------------------------   Address 3
#define LEDC_DEODRAN    0x01				// Ż��
#define LEDC_SMOKE      0x02				// +��
#define LEDC_MINUTE 	0x04				// ��
#define LEDC_TIMEREMAIN	0x08				// �����ð�
//---------------------------------------------------------   Address 4
#define LEDT_POWER 		0x80				// ��ġ ����
//---------------------------------------------------------   Address 5
#define LEDT_AUTO    	0x01				// ��ġ �ڵ�
#define LEDT_RESERVE	0x02				// ��ġ ����
#define LEDT_STABIL		0x04				// ��ġ ���/Ż��
#define LEDT_SMOKE		0x08				// ��ġ +��
//---------------------------------------------------------   Address 6
#define LEDT_UP			0x80				// ��ġ �ø�
//---------------------------------------------------------   Address 7
#define LEDT_DOWN  		0x01				// ��ġ ����
#define LEDT_STABIL2	0x02				// ��ġ ���/Ż�� 2
#define LEDC_TIMEREMAIN2 0x04				// �����ð� 2
//---------------------------------------------------------   Address 8
#define LEDI_POWER		0x01					// ������ ����
#define LEDI_AUTO		0x02					// ������ �ڵ�
#define LEDI_RESERVE	0x04					// ������ ����
#define LEDI_STABIL		0x08					// ������ ���
#define LEDI_SMOKE		0x10					// ������ +��
#define LEDI_UP			0x20					// ������ �ø�
#define LEDL_DOWN		0x40					// ������ ����


void Led_data_set(uint8_t segnum, uint8_t led_data, uint8_t onoff);
void Seg_data_set(uint8_t segnum, uint8_t seg_data, uint8_t dot);

void LD1963_Init(void);
void Send_LED_Data(void);
void Spi_Send_Data(uint8_t *p_tx_buffer, uint8_t tx_len, uint8_t flg_nss);
void uDelay(volatile uint32_t d_count);

#endif

