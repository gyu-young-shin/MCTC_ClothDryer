#ifndef	__DEBUG_H
#define	__DEBUG_H

#define	URX1_LEN		32
#define	UTX1_LEN		128


#define BACKSPACE			0x08			// Backspace
#define CRETURN				0x0D			// Carriage Return
#define LF					0x0A			// Line Feed
#define ESC					0x1B			// Escape
#define DEL					0x7F			// Del
#define CARET    			0x0D     		

typedef struct scmd
{
	char val[10];
	void (*func)(char* par);
}SCMD;

void USART1_IRQ_Function(void);
void Debug_proc(void);

char* get_entry(char *cp, char **pnext);
void GetLine_Proc(void);									// Line 입력을 받는다.
void UserCommand_Proc(void);						// 사용자 명령어 처리
void SendChar(unsigned char send_c);
void mprintf(const char *format, ...);					// For RS232 Debug
char* hex2Str(unsigned char * data, size_t dataLen);
void cmd_test(char *par);
void cmd_temp(char *par);
void cmd_heat(char *par);
void cmd_humi(char *par);
void cmd_smok(char *par);
void cmd_humitime(char *par);

void cmd_help(char *par);


#endif
