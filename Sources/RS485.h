#ifndef	__RS485_H
#define	__RS485_H

#define	URX2_LEN		32
#define	UTX2_LEN		32

void Rs485_proc(void);
void USART2_IRQ_Function(void);				// For debug 485
void SendChar2(unsigned char send_c);
void mprintf2(const char *format, ...);					// For RS485 Debug

#endif
