#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include "debug.h"
#include "rs485.h"
#include "clothdryer.h"

_Bool 		flg_485_send = 0;
_Bool 		urx2_comp;
_Bool 		flg_rs485_error = 0;
_Bool 		comm_init_ok = 0;


uint8_t		urx2_buf[URX2_LEN];
uint8_t		utx2_buf[UTX2_LEN];
uint8_t		utx2_tmp[UTX2_LEN];
uint8_t		utx2_send[UTX2_LEN];

uint8_t		tx2_restart = 1;
uint16_t 	urx2_count = 0;
uint16_t	p2_out = 0;
uint16_t	p2_in = 0;
uint16_t	urx2_tout;

uint8_t		comm_send_count = 0;
uint8_t		comm_init_count = 0;

//==========================================================================================================
extern	_Bool	COMM_CIRFAN_ON;				// 순환 팬
extern	_Bool	COMM_VENTILFAN_ON;			// 배기 팬
extern	_Bool	COMM_HEATER_ON;				// 히터
extern	_Bool	COMM_DUCT;					// 덕트 Open Close
extern	_Bool	COMM_AC220_SPARE1;
extern	_Bool	COMM_UVLAMP_ON;
extern	_Bool	COMM_LEDLAMP_ON;
extern	_Bool	COMM_SMOKE_HEATER_ON;		// SMOKE HEATER
extern	_Bool	COMM_SMOKE_PUMP_ON;			// SMOKE PUMP

extern	uint8_t COMM_SPEED_CIRCULA;			// 순환팬 속도
extern	uint8_t COMM_SPEED_VENTILATION;		// 배기팬 속도

extern	_Bool	COMM_DOOR_SW;
extern	_Bool	COMM_AIRPRESURE_SW;
extern	_Bool	COMM_SMOKE_HEATEROUT_STATE;

extern	_Bool 	flg_debug_test;
extern	_Bool 	flg_smoke_state;
extern	int8_t 	COMM_EXT_TEMP;
extern	uint8_t COMM_EXT_HUMI;
extern	int16_t	COMM_HEATER_TEMP;
extern	uint16_t 	error_code;

extern	int8_t		test_temper;
extern	int16_t		test_heatertemp;
extern	uint8_t		test_humi;


extern	UART_HandleTypeDef huart2;
extern uint16_t MakeCCITT16(uint8_t *data, uint32_t len);
//==========================================================================================================
void USART2_IRQ_Function(void)				// For debug 485
{	
	uint8_t data_char;
	
	uint32_t isrflags   = READ_REG(USART2->ISR);
	uint32_t cr1its     = READ_REG(USART2->CR1);
	uint32_t cr3its     = READ_REG(USART2->CR3);

	uint32_t errorflags;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));
	if (errorflags == 0U)
	{
		/* UART in mode Receiver ---------------------------------------------------*/
		if (((isrflags & USART_ISR_RXNE) != 0U) && ((cr1its & USART_CR1_RXNEIE) != 0U))
		{
			data_char = USART2->RDR & 0x00FF;
		
			if((!urx2_comp) && (urx2_count < URX2_LEN))
			{
				urx2_buf[urx2_count++] = data_char;
				//urx2_comp = 1;
				urx2_tout = 20;
			}
		}
		
		// Transmission
		if (((isrflags & USART_ISR_TXE) != 0U) && ((cr1its & USART_CR1_TXEIE) != 0U))
		{
			if(p2_out != p2_in) 							
			{
				USART2->TDR = utx2_buf[p2_out];
				p2_out++;
				p2_out %= UTX2_LEN; 
				tx2_restart = 0;
			}
			else 
			{
				tx2_restart = 1;
				CLEAR_BIT(USART2->CR1, USART_CR1_TXEIE);
				SET_BIT(USART2->CR1, USART_CR1_TCIE);
			}
		}
		
		if (((isrflags & USART_ISR_TC) != RESET) && ((cr1its & USART_CR1_TCIE) != RESET))		
		{
			RS485_DE_LOW;
			CLEAR_BIT(USART2->CR1, USART_CR1_TCIE);
		}
	}
	else
	{
		__HAL_UART_CLEAR_PEFLAG(&huart2);
		__HAL_UART_CLEAR_FEFLAG(&huart2);
		__HAL_UART_CLEAR_NEFLAG(&huart2);
		__HAL_UART_CLEAR_OREFLAG(&huart2);
	}
}


//------------------------------------------------------------------------------------------
void Rs485_proc(void) 
{
	uint8_t i;
	uint16_t Rcv_Crc, cmp_Crc;
	
	if(flg_485_send)
	{
		flg_485_send = 0;
		
		utx2_send[0] = 0x99;
		utx2_send[1] = 0;
		utx2_send[1] = (COMM_CIRFAN_ON << 7) | (COMM_VENTILFAN_ON << 6) | (COMM_HEATER_ON << 5) | 
						(COMM_DUCT << 4) | (COMM_AC220_SPARE1 << 3)  | (COMM_UVLAMP_ON << 2) |
						(COMM_LEDLAMP_ON << 1) | COMM_SMOKE_HEATER_ON;
		// utx2_send[1] = (COMM_CIRFAN_ON << 7) | (COMM_VENTILFAN_ON << 6) | (COMM_HEATER_ON << 5) | 
		// 				(COMM_DUCT << 4) | (COMM_AC220_SPARE1 << 3)  | (COMM_UVLAMP_ON << 2) |
		// 				(1 << 1) | COMM_SMOKE_HEATER_ON;
		utx2_send[2] = 0;
		utx2_send[2] = (COMM_SMOKE_PUMP_ON << 7);
		
		utx2_send[3] = COMM_SPEED_CIRCULA;
		utx2_send[4] = COMM_SPEED_VENTILATION;
		
		cmp_Crc = MakeCCITT16(utx2_send, 5);
		utx2_send[5] = (uint8_t)((cmp_Crc & 0xFF00) >> 8);
		utx2_send[6] = (uint8_t)(cmp_Crc & 0x00FF);
		
		for(i=0; i<7; i++)
			SendChar2(utx2_send[i]);
		
		//mprintf("Send Data = %x bldc1 = %d, bldc2=%d\n", utx2_send[1], COMM_BLDC1_ON, COMM_BLDC2_ON);
		comm_send_count++;
		if(comm_send_count > 10)
		{
			comm_send_count = 10;
			error_code |= EB_ERR_SET;
			flg_rs485_error = 1;
		}
		
		if(comm_init_count < 10)
		{
			comm_init_count++;
			if(comm_init_count >= 10)
			{
				comm_init_ok = 1;
			}
		}
	}
	
	if(urx2_comp)
	{
		if(urx2_count > 5)
		{
			Rcv_Crc = MakeCCITT16(urx2_buf, urx2_count - 2);	// CRC를 남기고 모두 계산한다.
			cmp_Crc = (uint16_t)(urx2_buf[urx2_count-2] << 8) | urx2_buf[urx2_count-1];

			if((Rcv_Crc == cmp_Crc) && (urx2_buf[0] == 0x88))
			{
				comm_send_count = 0;
				flg_rs485_error = 0;

				if(flg_debug_test)
				{
					COMM_EXT_TEMP = test_temper;
					COMM_EXT_HUMI = test_humi;
					COMM_HEATER_TEMP = test_heatertemp;
					COMM_SMOKE_HEATEROUT_STATE = flg_smoke_state;
				}
				else
				{
					COMM_EXT_TEMP = urx2_buf[1];
					COMM_EXT_HUMI = urx2_buf[2];
					COMM_HEATER_TEMP = (int16_t)(urx2_buf[3] << 8);
					COMM_HEATER_TEMP |= urx2_buf[4];
					COMM_SMOKE_HEATEROUT_STATE = (urx2_buf[5] & 0x20) ? 1:0;
				}
				
				COMM_DOOR_SW = (urx2_buf[5] & 0x80) ? 1:0;
				COMM_AIRPRESURE_SW = (urx2_buf[5] & 0x40) ? 1:0;
			}
		}
		urx2_count = 0;
		urx2_comp = 0;
	}
	
//	COMM_EXT_TEMP = 36;
//	COMM_EXT_HUMI = 65;

//	COMM_DOOR_SW = 0;
//	COMM_WIND_SW = 0;
}

//------------------------------------------------------------------------------------------
void SendChar2(uint8_t send_c) 
{
	uint16_t pin_temp;

	pin_temp = p2_in + 1;
	pin_temp %= UTX2_LEN;

	while(pin_temp == p2_out)
	{
		HAL_Delay(1);
	}
	
	utx2_buf[p2_in] = send_c;
	p2_in = pin_temp;

	if(tx2_restart) 
	{
		tx2_restart = 0;
		RS485_DE_HIGH;
		SET_BIT(USART2->CR1, USART_CR1_TXEIE);
	}
}

//------------------------------------------------------------------------------------------
void mprintf2(const char *format, ...)
{
    uint16_t i;
	__va_list	arglist;

	va_start(arglist, format);
    vsprintf((char *)utx2_tmp, format, arglist);
	va_end(arglist);
	i = 0;
	
	while(utx2_tmp[i] && (i < UTX2_LEN))	// Null 문자가 아닌동안 개행문자를 개행문자 + Carrige Return 으로 변환
	{
		if(utx2_tmp[i] == '\n')
			SendChar2('\r');
		SendChar2(utx2_tmp[i++]);
	}
}

//------------------------------------------------------------------------------------------


