#ifndef	__FLASHROM_H
#define	__FLASHROM_H
#include "main.h"
// STM32F030R8T6
#define FLASH_DATA_START_ADDR   ((uint32_t)0x0800C000)   /* Start @ of user Flash area */
#define FLASH_DATA_SIZE   		0x4000   	// 16K Byte

typedef struct
{
	uint32_t   	e2p_offset_addr;		// Offset Address

	int8_t      e2p_auto_temp;       	// �ڵ����� �����µ�
	uint8_t     e2p_auto_humi;       	// �ڵ����� ��������  50%
	int8_t      e2p_manual_temp;       	// �������� �����µ�
	uint8_t     e2p_manual_time;       	// �������� �����ð�

	int8_t      e2p_ster_temp;			// ��� �����µ�
	uint8_t     e2p_ster_time;			// ��� �����ð�
	uint8_t     e2p_cirfan_diff_temp;	// ��ȯ �ҷ� üũ�� �ǳ��µ��� ���ⱸ �µ��� ���� ���ⱸ �µ� 10�� �̻�
	uint8_t     e2p_heater_diff_temp;	// ���� �ҷ� üũ�� �ǳ��µ��� ���ⱸ �µ��� ���� 1������(���� �۵� 3���� ���� üũ)

	uint8_t     e2p_smokbimetal_time;	// �ʴ��� �������� ������ 40�� �̳��� �������� ��½�ȣ�� �������� ������ ������ �������� ó���Ѵ�.
	uint8_t     e2p_heater_oper_percent;	// ȯ�� ������ ������ ������ N% �ѹ��̶� �����µ� ���� ���ĺ��� 3����Ŭ �ݺ��Ŀ� ���� üũ
	uint8_t     e2p_under_err_humi;		// �������� ���� N% ����
	uint8_t     e2p_over_err_humi;		// �������� ���� N% �̻�
	
	uint16_t   	e2p_opertime_set;		// ���ͱ�ȯ�ñ� ���� �ð� (1-99) ���� 10�ð�
	uint16_t   	pads;
	
	uint32_t   	e2p_operated_time;		// ���ͱ�ȯ �ñ� �˷��ִ� ���۽ð�
} TE2PDataRec;


void FlashRom_Init(void);
void FlashRom_WriteData(void);

#endif

