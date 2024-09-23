#include "main.h"
#include "FlashRom.h"
#include "debug.h"

TE2PDataRec Flashdatarec;
uint32_t Start_Address, offset_data, offset_var;
volatile uint32_t *p_data;

//=====================================================================
// ���� �÷���
void FlashRom_Init(void)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
	
	Start_Address = FLASH_DATA_START_ADDR;
	
	// 11��° Flash Secor�� ù��° Word Data�� �д´�.
	offset_data = *(volatile uint32_t *)(Start_Address);

	if(offset_data == 0xFFFFFFFF)			// �ش簪�� Ʋ���ٸ� �ƹ��͵� �� ������.
	{
		Flashdatarec.e2p_offset_addr = 0x00;
		
        Flashdatarec.e2p_auto_temp = 70;       	// �ڵ����� �����µ�
        Flashdatarec.e2p_auto_humi = 30;       	// �ڵ����� ��������  30%
        Flashdatarec.e2p_manual_temp = 70;      // �������� �����µ�
        Flashdatarec.e2p_manual_time = 30;      // �������� �����ð� 30��
        Flashdatarec.e2p_ster_temp = 70;       	// ��� �����µ�
        Flashdatarec.e2p_ster_time = 15;       	// ��� �����ð� 15��

		Flashdatarec.e2p_heater_oper_percent = 5;	// ȯ�� ������ ������ ������ N% �ѹ��̶� �����µ� ���� ���ĺ��� 3����Ŭ �ݺ��Ŀ� ���� üũ
		Flashdatarec.e2p_cirfan_diff_temp = 20;	// ��ȯ �ҷ� üũ�� �ǳ��µ��� ���ⱸ �µ��� ���� ���ⱸ �µ� 20�� �̻�
		Flashdatarec.e2p_heater_diff_temp = 3;	// ���� �ҷ� üũ�� �ǳ��µ��� ���ⱸ �µ��� ���� 1������(���� �۵� 3���� ���� üũ)
		Flashdatarec.e2p_smokbimetal_time = 40;	// �ʴ��� �������� ������ 40�� �̳��� �������� ��½�ȣ�� �������� ������ ������ �������� ó���Ѵ�.
		
		Flashdatarec.e2p_under_err_humi = 5;	// �������� ���� N% ����
		Flashdatarec.e2p_over_err_humi = 95;	// �������� ���� N% �̻�
		Flashdatarec.e2p_opertime_set = 30;		// ���ͱ�ȯ �ñ� ���� �ð� (1-99) ���� 10�ð�
		Flashdatarec.e2p_operated_time = 0;		// ���ͱ�ȯ �ñ� �˷��ִ� ���۽ð�

		__disable_irq();						// Global Interrupt Disable
		HAL_FLASH_Unlock();						// ���� Flash ����ϱ� ���� Unlock �Ѵ�
		
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 

		// ������ ��ü ������ �����.
		offset_data = 0;
		while(offset_data < FLASH_DATA_SIZE)
		{
			EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
			EraseInitStruct.PageAddress = (FLASH_DATA_START_ADDR + offset_data); //User defined addr
			EraseInitStruct.NbPages     = 1;		
			
			if(HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
			{
				HAL_FLASH_Lock(); 
				__enable_irq();
				
				mprintf("Flash Erase Error!!\n");
				return;
			}
			offset_data += 0x400;		// 1K
		}
		
		p_data = (volatile uint32_t *)(&Flashdatarec);

		while(Start_Address < (FLASH_DATA_START_ADDR + sizeof(Flashdatarec)))
		{
			if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Start_Address, *p_data) != HAL_OK)
			{
				HAL_FLASH_Lock(); 
				__enable_irq();

				mprintf("Flash Program Error!!\n");
				return;
			}
			
			Start_Address += 4;
			p_data++;
		}
		
		HAL_FLASH_Lock(); 
		__enable_irq();
	}
	else
	{
		offset_data = sizeof(TE2PDataRec);		// 2��° ���� �д´�.
		// ������ �����͸� �д´�.
		while(offset_data < (FLASH_DATA_SIZE - sizeof(TE2PDataRec)))
		{
			offset_var = *(volatile uint32_t *)(FLASH_DATA_START_ADDR + offset_data);
			if(offset_var == 0xFFFFFFFF)
			{
				offset_data -= sizeof(TE2PDataRec);
				break;
			}
			else
				offset_data += sizeof(TE2PDataRec);
		}

		p_data = (volatile uint32_t *)(&Flashdatarec);
		
		Start_Address = FLASH_DATA_START_ADDR + offset_data;
		
		while(Start_Address < (FLASH_DATA_START_ADDR + offset_data + sizeof(TE2PDataRec)))
		{
			*p_data = *(volatile uint32_t *)Start_Address;
			Start_Address += 4;
			p_data++;
		}
	}
	
	//Flashdatarec.e2p_filter_changetime = 126000;
}

void FlashRom_WriteData(void)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
	
	volatile uint32_t Start_Address;
	volatile uint32_t *p_data;
	
	Start_Address = FLASH_DATA_START_ADDR + Flashdatarec.e2p_offset_addr + sizeof(TE2PDataRec);
	
	__disable_irq();			// Global Interrupt Disable
	HAL_FLASH_Unlock();				// ���� Flash ����ϱ� ���� Unlock �Ѵ�
	
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 

	// Write�� �ּҰ� ���������κ��� ������ ũ�⸦ �� �ͺ��� ũ�ų� ���ٸ�
	if(Start_Address >= (FLASH_DATA_START_ADDR + FLASH_DATA_SIZE - (sizeof(TE2PDataRec) * 4)))
	{
		// ������ ��ü ������ �����.
		offset_data = 0;
		while(offset_data < FLASH_DATA_SIZE)
		{
			EraseInitStruct.TypeErase   = FLASH_TYPEERASE_PAGES;
			EraseInitStruct.PageAddress = (FLASH_DATA_START_ADDR + offset_data); //User defined addr
			EraseInitStruct.NbPages     = 1;		

			if(HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
			{
				HAL_FLASH_Lock(); 
				__enable_irq();
				
				mprintf("Flash Erase Error!!\n");
				return;
			}
			
			offset_data += 0x400;		// 1K
		}
		Flashdatarec.e2p_offset_addr = 0;
	}
	else
		Flashdatarec.e2p_offset_addr += sizeof(TE2PDataRec);  // Write �� �ּ� ����
	
	
	Start_Address = FLASH_DATA_START_ADDR + Flashdatarec.e2p_offset_addr;
	p_data = (volatile uint32_t *)(&Flashdatarec);
	
	while(Start_Address < (FLASH_DATA_START_ADDR + Flashdatarec.e2p_offset_addr + sizeof(TE2PDataRec)))
	{
		if(HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Start_Address, *p_data) != HAL_OK)
		{
			HAL_FLASH_Lock(); 
			__enable_irq();

			mprintf("Flash Program Error!!\n");
			return;
		}
		
		Start_Address += 4;
		p_data++;
	}

	HAL_FLASH_Lock(); 
	__enable_irq();
}

