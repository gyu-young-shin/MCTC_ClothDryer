#include "main.h"
#include "FlashRom.h"
#include "debug.h"

TE2PDataRec Flashdatarec;
uint32_t Start_Address, offset_data, offset_var;
volatile uint32_t *p_data;

//=====================================================================
// 내부 플래쉬
void FlashRom_Init(void)
{
	FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;
	
	Start_Address = FLASH_DATA_START_ADDR;
	
	// 11번째 Flash Secor의 첫번째 Word Data를 읽는다.
	offset_data = *(volatile uint32_t *)(Start_Address);

	if(offset_data == 0xFFFFFFFF)			// 해당값과 틀리다면 아무것도 안 써졌음.
	{
		Flashdatarec.e2p_offset_addr = 0x00;
		
        Flashdatarec.e2p_auto_temp = 70;       	// 자동운전 설정온도
        Flashdatarec.e2p_auto_humi = 30;       	// 자동운전 설정습도  30%
        Flashdatarec.e2p_manual_temp = 70;      // 수동운전 설정온도
        Flashdatarec.e2p_manual_time = 30;      // 수동운전 설정시간 30분
        Flashdatarec.e2p_ster_temp = 70;       	// 살균 설정온도
        Flashdatarec.e2p_ster_time = 15;       	// 살균 설정시간 15분

		Flashdatarec.e2p_heater_oper_percent = 5;	// 환기 동작중 히터의 가동율 N% 한번이라도 설정온도 도달 이후부터 3사이클 반복후에 에러 체크
		Flashdatarec.e2p_cirfan_diff_temp = 20;	// 순환 불량 체크시 실내온도와 토출구 온도의 차값 토출구 온도 20도 이상
		Flashdatarec.e2p_heater_diff_temp = 3;	// 히터 불량 체크시 실내온도와 토출구 온도의 차값 1도이하(히터 작동 3분후 부터 체크)
		Flashdatarec.e2p_smokbimetal_time = 40;	// 초단위 연무펌프 동작후 40초 이내에 연무히터 고온신호가 해제되지 않으면 연무액 없음으로 처리한다.
		
		Flashdatarec.e2p_under_err_humi = 5;	// 습도센서 에러 N% 이하
		Flashdatarec.e2p_over_err_humi = 95;	// 습도센서 에러 N% 이상
		Flashdatarec.e2p_opertime_set = 30;		// 필터교환 시기 설정 시간 (1-99) 단위 10시간
		Flashdatarec.e2p_operated_time = 0;		// 필터교환 시기 알려주는 동작시간

		__disable_irq();						// Global Interrupt Disable
		HAL_FLASH_Unlock();						// 내부 Flash 사용하기 위해 Unlock 한다
		
		__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 

		// 나머지 전체 영역을 지운다.
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
		offset_data = sizeof(TE2PDataRec);		// 2번째 부터 읽는다.
		// 마지막 데이터를 읽는다.
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
	HAL_FLASH_Unlock();				// 내부 Flash 사용하기 위해 Unlock 한다
	
	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPERR); 

	// Write할 주소가 마지막으로부터 데이터 크기를 뺀 것보다 크거나 같다면
	if(Start_Address >= (FLASH_DATA_START_ADDR + FLASH_DATA_SIZE - (sizeof(TE2PDataRec) * 4)))
	{
		// 나머지 전체 영역을 지운다.
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
		Flashdatarec.e2p_offset_addr += sizeof(TE2PDataRec);  // Write 할 주소 증가
	
	
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

