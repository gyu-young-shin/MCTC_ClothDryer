#ifndef	__AIR_CLEANER_H
#define	__AIR_CLEANER_H
#include "main.h"

//#define ERR_COMM485		0x04			// 485 통신에러

enum {STOP_MODE = 0, RUN_MODE};
enum {AUTO_MODE = 0, MANUAL_MODE, STER_MODE, SMOKE_MODE, IDLE_MODE};	// 운전선택 모드
enum {DISP_WAIT = 0, DISP_RESERVE, DISP_TEMPSET, DISP_TEMPTIME, DISP_HEATER_TEMP, DISP_HUMIDITY, DISP_OPTION, DISP_FILTER, DISP_DOOROPEN};	
/************************************************************************
        Error Declaration Section
************************************************************************/
#define E1_ERR_SET                0x0001                        // 순환불량
#define E2_ERR_SET                0x0002                        // 풍압 스위치 불량
#define E3_ERR_SET                0x0004                        // 히터과열
#define E4_ERR_SET                0x0008                        // 히터불량
#define E5_ERR_SET                0x0010                        // 연무기 불량
#define E6_ERR_SET                0x0020                        // 연무기 바이메탈 불량
#define E7_ERR_SET                0x0040                        // 환기 불량
#define E8_ERR_SET                0x0080                        // 온도센서 불량
#define E9_ERR_SET                0x0100                        // 토출온도센서 불량
#define EA_ERR_SET                0x0200                        // 습도센서 불량
#define EB_ERR_SET                0x0400                        // 485통신 불량


void InputCheck_Proc(void);
void Output_Proc(void);
void Disp_Temp_Seg(int8_t current_temp);
void Disp_Humi_Seg(uint8_t current_humi);
void Disp_Time_Seg(uint8_t current_time);
void Disp_HighTemp_Seg(void);

void Error_Recovery_Proc(void);
void Disp_Segment(void);
void Stop_Proc(void);
void Cloth_Dryer_Proc(void);
void Auto_Proc(void);
void Ster_Proc(void);
void Smoke_Proc(void);
void Cooling_Proc(void);
void InputKey_Proc(void);
void Check_BldcFan_Speed(void);
void Check_Filter_Change(void);
void Disp_Option(void);
void Disp_ErrCode(void);
void Circula_Error_Check(void);
void Heater_Error_Check(void);
void Smoke_Error_Check(void);
void Liquid_Error_Check(void);
void Ventilation_Error_Check(void);
void Temperature_Error_Check(void);
void HeaterTemper_Error_Check(void);
void HumiSensor_Error_Check(void);
void Operated_Time_Check(void);
void AirPresure_Error_Check(void);
void Error_Check_Proc(void);
void OverHeat_Error_Check(void);
void Autoled_on(void);
void Sterled_on(void);
void Disp_Version(void);
void Disp_Coollingled(void);
void Waitled_on(void);
void Calculate_Humi(void);
void Optionled_on(void);
void Smokeled_on(void);

#endif
