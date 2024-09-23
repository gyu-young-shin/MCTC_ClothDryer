#include "main.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <ctype.h>
#include "debug.h"

_Bool 		flg_debug_test = 0;
_Bool 		flg_smoke_state = 0;

_Bool 		urx1_comp;
uint8_t		urx1_buf[URX1_LEN];
uint8_t		utx1_buf[UTX1_LEN];
uint8_t		utx1_tmp[UTX1_LEN];

uint8_t		tx1_restart = 1;
uint16_t 	urx1_count = 0;
uint16_t	p1_out = 0;
uint16_t	p1_in = 0;
uint16_t	urx1_tout;
uint8_t		flg_getline_comp = 0;

char		in_line[URX1_LEN];

int8_t		test_temper = 25;
int16_t		test_heatertemp = 50;
uint8_t		test_humi = 50;


#define MAX_HEX_STR_LENGTH  32
char hexStr[MAX_HEX_STR_LENGTH];

//--------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------
const	char help[] =
	"\n\n\n\n\n"
	"+-----------------------------------------------------------------------+\n"
	"|                         MC TECH Command Help                          |\n"
	"|-----------------------------------------------------------------------|\n"
	"|  HELP                                                                 |\n"
	"+-----------------------------------------------------------------------+\n";

const SCMD cmd[] = 
{
	"HELP",		cmd_help,
	"TEST",		cmd_test,
	"TEMP",		cmd_temp,
	"HEAT",		cmd_heat,
	"HUMI",		cmd_humi,
	"SMOK",		cmd_smok,
	"HTIME",	cmd_humitime,
};

#define CMD_COUNT (sizeof (cmd) / sizeof (cmd[0]))
//==========================================================================================================
extern	uint16_t	humi_check_timeout;
extern	UART_HandleTypeDef huart1;
//==========================================================================================================
void USART1_IRQ_Function(void)				// For debug 232
{	

	uint8_t data_char;
	
	uint32_t isrflags   = READ_REG(USART1->ISR);
	uint32_t cr1its     = READ_REG(USART1->CR1);
	uint32_t cr3its     = READ_REG(USART1->CR3);

	uint32_t errorflags;

	/* If no error occurs */
	errorflags = (isrflags & (uint32_t)(USART_ISR_PE | USART_ISR_FE | USART_ISR_ORE | USART_ISR_NE | USART_ISR_RTOF));
	if (errorflags == 0U)
	{
		/* UART in mode Receiver ---------------------------------------------------*/
		if (((isrflags & USART_ISR_RXNE) != 0U) && ((cr1its & USART_CR1_RXNEIE) != 0U))
		{
			data_char = USART1->RDR & 0x00FF;
		
			if((!urx1_comp) && (urx1_count < URX1_LEN))
			{
				urx1_buf[urx1_count++] = data_char;
				urx1_comp = 1;
			}
		}
		
		// Transmission
		if (((isrflags & USART_ISR_TXE) != 0U) && ((cr1its & USART_CR1_TXEIE) != 0U))
		{
			if(p1_out != p1_in) 							
			{
				USART1->TDR = utx1_buf[p1_out];
				p1_out++;
				p1_out %= UTX1_LEN; 
				tx1_restart = 0;
			}
			else 
			{
				tx1_restart = 1;
				CLEAR_BIT(USART1->CR1, USART_CR1_TXEIE);
			}
		}
	}
	else
	{
		__HAL_UART_CLEAR_PEFLAG(&huart1);
		__HAL_UART_CLEAR_FEFLAG(&huart1);
		__HAL_UART_CLEAR_NEFLAG(&huart1);
		__HAL_UART_CLEAR_OREFLAG(&huart1);
	}
}


//------------------------------------------------------------------------------------------
void Debug_proc(void) 
{
	if(urx1_comp)
	{
		GetLine_Proc();  					
		UserCommand_Proc();
	}
}

//------------------------------------------------------------------------------------------
void GetLine_Proc(void)												// Line 입력을 받는다.
{
	uint8_t 	temp_char;
	uint32_t	RxCnt_temp = urx1_count;

	temp_char = urx1_buf[urx1_count-1];
	
	if((temp_char == BACKSPACE) || (temp_char == DEL)) 				// 백스페이스, Del Key가 입력 되면
	{   
		urx1_comp = 0;
		if(RxCnt_temp > 1)
		{
			urx1_count				-= 2;
			urx1_buf[urx1_count]	 = 0;
			SendChar(BACKSPACE);
			SendChar(' ');
			SendChar(BACKSPACE);
		}
		else
		{
			urx1_count				= 0;
			urx1_buf[urx1_count] 	= 0;
		}
	  	
    }
	else if((urx1_count >= URX1_LEN)  || (temp_char == CARET)) 
	{
		urx1_comp = 1;
		SendChar(CARET);
		SendChar(LF);
		flg_getline_comp = 1;
		urx1_buf[urx1_count-1] = 0x0;		// Null Terminal Char
		urx1_count--;
    }
	else if(temp_char == CRETURN)
	{
		urx1_comp = 1;
		urx1_buf[urx1_count - 1] = 0;
		urx1_count--;
		flg_getline_comp = 1;
	}
	else if(temp_char == ESC)
	{
		urx1_comp = 1;
		SendChar(CARET);
		SendChar(LF);
		flg_getline_comp 			= 1;
		urx1_buf[0] 				= 0x0;		// Null Terminal Char
		urx1_count 					= 0;
	}
	else 
	{
		urx1_comp = 0;
		SendChar(temp_char);			// Echo 처리
	}
}

//------------------------------------------------------------------------------------------
void UserCommand_Proc(void)						// 사용자 명령어 처리
{
	uint32_t i;
	char *sp, *next;

	sp = 0;
	next = 0;

	if(flg_getline_comp)
	{
		if(urx1_count)
		{
			for(i=0; i<urx1_count; i++)
				urx1_buf[i] = toupper(urx1_buf[i]);
			strcpy(in_line, (char *)urx1_buf);
			
			sp = get_entry(&in_line[0], &next);

			if(*sp != 0)
			{
				for(i = 0; i < CMD_COUNT; i++) 
				{
					if(strcmp (sp, (const char *)&cmd[i].val)) 
						continue;
					cmd[i].func(next);                  
					break;
				}
				
				if(i == CMD_COUNT)
					mprintf ("\nCommand error\n");
			}
		}		
		
		mprintf("\nMcTech>");				// Prompt Print
		flg_getline_comp = 0;
		urx1_comp = 0;
		urx1_count = 0;
	}
}

//------------------------------------------------------------------------------------------
char *get_entry (char *cp, char **pnext) 
{
   char *sp;

   if(*cp == 0) 
   {
      *pnext = cp;
      return (cp);
   }

   for( ; *cp == ' '; cp++) 							// 앞부분 공백 처리 이후 cp는 첫 글자를 가리킴
   {
      *cp = 0;
   }
 
   for(sp=cp; (*sp != ' ') && (*sp != 0); sp++);

   if(*sp != 0)
   {
		for(; *sp == ' '; sp++)
			*sp = 0;
   }
   *pnext = sp;                /* &next entry                  */
   return (cp);
}

//------------------------------------------------------------------------------------------
void SendChar(uint8_t send_c) 
{
	uint16_t pin_temp;

	pin_temp = p1_in + 1;
	pin_temp %= UTX1_LEN;

	while(pin_temp == p1_out)
	{
		HAL_Delay(1);
	}
	
	utx1_buf[p1_in] = send_c;
	p1_in = pin_temp;

	if(tx1_restart) 
	{
		tx1_restart = 0;
		SET_BIT(USART1->CR1, USART_CR1_TXEIE);
	}
}

//------------------------------------------------------------------------------------------
void mprintf(const char *format, ...)
{
    uint16_t i;
	__va_list	arglist;

	va_start(arglist, format);
    vsprintf((char *)utx1_tmp, format, arglist);
	va_end(arglist);
	i = 0;
	
	while(utx1_tmp[i] && (i < UTX1_LEN))	// Null 문자가 아닌동안 개행문자를 개행문자 + Carrige Return 으로 변환
	{
		if(utx1_tmp[i] == '\n')
			SendChar('\r');
		SendChar(utx1_tmp[i++]);
	}
}

//------------------------------------------------------------------------------------------
void cmd_help(char *par)
{
	mprintf(help);
}
//------------------------------------------------------------------------------------------
void cmd_test(char *par)
{
	char *data_var, *next;

	data_var = get_entry(par, &next);
	
	if(*data_var == 0)
	{
		mprintf("Use test [1/0]\n");
		mprintf("Test: %d\n", flg_debug_test);
	}
	else
	{
		if(atoi(data_var) == 1)
			flg_debug_test = 1;
		else
			flg_debug_test = 0;
	}
}
//------------------------------------------------------------------------------------------
void cmd_temp(char *par)
{
	char *data_var, *next;

	data_var = get_entry(par, &next);
	
	if(*data_var == 0)
	{
		mprintf("Temper: %d\n", test_temper);
	}
	else
	{
		test_temper = atoi(data_var);
	}
}
//------------------------------------------------------------------------------------------
void cmd_heat(char *par)
{
	char *data_var, *next;

	data_var = get_entry(par, &next);
	
	if(*data_var == 0)
	{
		mprintf("Heater: %d\n", test_heatertemp);
	}
	else
	{
		test_heatertemp = atoi(data_var);
	}
}
//------------------------------------------------------------------------------------------
void cmd_humi(char *par)
{
	char *data_var, *next;

	data_var = get_entry(par, &next);
	
	if(*data_var == 0)
	{
		mprintf("Humi: %d\n", test_humi);
	}
	else
	{
		test_humi = atoi(data_var);
	}
}
//------------------------------------------------------------------------------------------
void cmd_humitime(char *par)
{
	char *data_var, *next;

	data_var = get_entry(par, &next);
	
	if(*data_var == 0)
	{
		mprintf("HTIME: %d\n", humi_check_timeout);
	}
	else
	{
		humi_check_timeout = atoi(data_var);
	}
}
//------------------------------------------------------------------------------------------
void cmd_smok(char *par)
{
	char *data_var, *next;

	data_var = get_entry(par, &next);
	
	if(*data_var == 0)
	{
		mprintf("Smok: %d\n", flg_smoke_state);
	}
	else
	{
		flg_smoke_state = atoi(data_var);
	}
}

char* hex2Str(unsigned char * data, size_t dataLen)
{
	unsigned char * pin = data;
	const char * hex = "0123456789ABCDEF";
	char * pout = hexStr;
	uint8_t i;
    
    if( dataLen > (MAX_HEX_STR_LENGTH/2) )
    {
		dataLen = (MAX_HEX_STR_LENGTH/2);
    }
    
    if(dataLen == 0)
    {
		pout[0] = 0;     
    } 
    else     
    {
		for(i = 0; i < dataLen - 1; i++)
		{
		  *pout++ = hex[(*pin>>4)&0x0F];
		  *pout++ = hex[(*pin++)&0x0F];
		}
		*pout = 0;
    }    
    
    return hexStr;
}


