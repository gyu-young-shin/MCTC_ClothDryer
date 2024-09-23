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
#define LEDC_AUTO		0x80				// 자동
//---------------------------------------------------------   Address 1
#define LEDC_RESERVE   	0x01				// 예약
#define LEDC_DRY    	0x02				// 건조
#define LEDC_TEMP  		0x04				// 온도
#define LEDC_TIME   	0x08				// 시간
//---------------------------------------------------------   Address 2
#define LEDC_STABIL		0x80				// 살균
//---------------------------------------------------------   Address 3
#define LEDC_DEODRAN    0x01				// 탈취
#define LEDC_SMOKE      0x02				// +향
#define LEDC_MINUTE 	0x04				// 분
#define LEDC_TIMEREMAIN	0x08				// 남은시간
//---------------------------------------------------------   Address 4
#define LEDT_POWER 		0x80				// 터치 전원
//---------------------------------------------------------   Address 5
#define LEDT_AUTO    	0x01				// 터치 자동
#define LEDT_RESERVE	0x02				// 터치 예약
#define LEDT_STABIL		0x04				// 터치 살균/탈취
#define LEDT_SMOKE		0x08				// 터치 +향
//---------------------------------------------------------   Address 6
#define LEDT_UP			0x80				// 터치 올림
//---------------------------------------------------------   Address 7
#define LEDT_DOWN  		0x01				// 터치 내림
#define LEDT_STABIL2	0x02				// 터치 살균/탈취 2
#define LEDC_TIMEREMAIN2 0x04				// 남은시간 2
//---------------------------------------------------------   Address 8
#define LEDI_POWER		0x01					// 아이콘 전원
#define LEDI_AUTO		0x02					// 아이콘 자동
#define LEDI_RESERVE	0x04					// 아이콘 예약
#define LEDI_STABIL		0x08					// 아이콘 살균
#define LEDI_SMOKE		0x10					// 아이콘 +향
#define LEDI_UP			0x20					// 아이콘 올림
#define LEDL_DOWN		0x40					// 아이콘 내림


void Led_data_set(uint8_t segnum, uint8_t led_data, uint8_t onoff);
void Seg_data_set(uint8_t segnum, uint8_t seg_data, uint8_t dot);

void LD1963_Init(void);
void Send_LED_Data(void);
void Spi_Send_Data(uint8_t *p_tx_buffer, uint8_t tx_len, uint8_t flg_nss);
void uDelay(volatile uint32_t d_count);

#endif

