#ifndef	__FLASHROM_H
#define	__FLASHROM_H
#include "main.h"
// STM32F030R8T6
#define FLASH_DATA_START_ADDR   ((uint32_t)0x0800C000)   /* Start @ of user Flash area */
#define FLASH_DATA_SIZE   		0x4000   	// 16K Byte

typedef struct
{
	uint32_t   	e2p_offset_addr;		// Offset Address

	int8_t      e2p_auto_temp;       	// 자동운전 설정온도
	uint8_t     e2p_auto_humi;       	// 자동운전 설정습도  50%
	int8_t      e2p_manual_temp;       	// 수동운전 설정온도
	uint8_t     e2p_manual_time;       	// 수동운전 설정시간

	int8_t      e2p_ster_temp;			// 살균 설정온도
	uint8_t     e2p_ster_time;			// 살균 설정시간
	uint8_t     e2p_cirfan_diff_temp;	// 순환 불량 체크시 실내온도와 토출구 온도의 차값 토출구 온도 10도 이상
	uint8_t     e2p_heater_diff_temp;	// 히터 불량 체크시 실내온도와 토출구 온도의 차값 1도이하(히터 작동 3분후 부터 체크)

	uint8_t     e2p_smokbimetal_time;	// 초단위 연무펌프 동작후 40초 이내에 연무히터 고온신호가 해제되지 않으면 연무액 없음으로 처리한다.
	uint8_t     e2p_heater_oper_percent;	// 환기 동작중 히터의 가동율 N% 한번이라도 설정온도 도달 이후부터 3사이클 반복후에 에러 체크
	uint8_t     e2p_under_err_humi;		// 습도센서 에러 N% 이하
	uint8_t     e2p_over_err_humi;		// 습도센서 에러 N% 이상
	
	uint16_t   	e2p_opertime_set;		// 필터교환시기 설정 시간 (1-99) 단위 10시간
	uint16_t   	pads;
	
	uint32_t   	e2p_operated_time;		// 필터교환 시기 알려주는 동작시간
} TE2PDataRec;


void FlashRom_Init(void);
void FlashRom_WriteData(void);

#endif

