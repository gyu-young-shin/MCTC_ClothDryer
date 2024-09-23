#include "main.h"
#include "buzzer.h"
#include "debug.h"
#include "clothdryer.h"
#include "flashrom.h"
#include "segment.h"

_Bool	flg_toggle = 0;
_Bool 	flg_toggle_500ms = 0;
_Bool 	flg_toggle_5sec = 0;

_Bool	flg_485_excute = 0;
_Bool	flg_oper_time_save = 0;

uint8_t callback_1ms = 0;
uint8_t callback_10ms = 0;
uint8_t callback_100ms = 0;
uint8_t callback_500ms = 0;
uint8_t callback_1min = 0;
uint8_t callback_30ms = 0;

uint8_t  menu_key[KEY_COUNT] = {0,};
uint8_t  keyin_buf[KEY_COUNT][3];
uint8_t  key_pushed[KEY_COUNT] = {0,};
uint8_t  key_continued[KEY_COUNT] = {0,};
uint8_t  key_idx = 0;
uint8_t  key_cont_count[KEY_COUNT] = {0,};

uint8_t	 test_digit = 0;
void Key_Scan(void);
//======================================================================
extern	TE2PDataRec Flashdatarec;

extern	_Bool 		urx2_comp;
extern  _Bool 		flg_485_send;
extern  _Bool		flg_e2p_save;
extern  _Bool		flg_cirfan_out;				// 순환 팬
extern  _Bool		flg_ventilfan_out;			// 배기 팬
extern  _Bool		flg_heater_out;				// 히터
extern  _Bool		flg_duct_out;				// 덕트 Open Close
extern  _Bool		flg_uvlamp_out;
extern  _Bool		flg_check_humi_start;
extern  _Bool	 	flg_heater_runrate_first;
extern  _Bool	 	flg_reserve;

extern	uint8_t 	optkey_timeout;
extern	uint16_t	urx2_tout;
extern	uint8_t 	op_mode;
extern	int8_t		COMM_EXT_TEMP;

extern	uint8_t		temper_timeout;
extern	uint8_t 	keycont_timeout;
extern	uint8_t 	e2p_save_time;
extern	uint8_t 	disp_timeout;
extern	uint8_t 	disp_mode;
extern	uint16_t 	oper_timeout;
extern	uint8_t 	buzzer_play_timeout;
extern	uint16_t 	oper_total_timeout;
extern	uint16_t	humi_check_timeout;
extern	uint16_t 	smoke_run_timeout;
extern	uint16_t 	ster_run_timeout;
extern	uint8_t		circula_fan_off_timeout;

extern	_Bool		COMM_CIRFAN_ON;

extern	uint8_t		temper_count;
extern	uint8_t		key_timeout;
extern	uint8_t		error_timeout;
extern	uint16_t 	cir_err_timeout;
extern	uint16_t 	heater_err_timeout;
extern	uint16_t 	smokeheater_err_timeout;
extern	uint16_t 	liquidout_err_timeout;
extern	uint8_t		airpresure_timeout;
extern	uint16_t 	airpresure_err_timeout;

extern	uint16_t 	heater_running_time;	// 히터 가동시간
extern	uint16_t 	heater_run_total_time;	// 히터 기동부터 가동율 계산 총시간
extern	uint8_t  	sensor_check_timeout;
extern	uint16_t 	airpresure_chk_timeout;	// 풍압스위치 Check Timeout
extern	uint16_t 	heatertemp_check_timeout;

extern	_Bool		COMM_SMOKE_HEATEROUT_STATE;
extern	uint32_t	reserve_clock;
extern	uint8_t  	reserve_timeout;
extern	uint8_t 	prev_dispmode;
extern	uint8_t 	op_kind;
extern	uint8_t  	coolled_timeout;
extern	uint8_t  	playend_count;
extern	_Bool		flg_dryend;

extern	uint16_t  	humi_err_timeout;
extern	uint16_t  	oper_remain_time;
//======================================================================
void MySysTick_Handler(void)
{
	callback_1ms++;
	callback_30ms++;
	
	if(urx2_tout)
	{
		urx2_tout--;
		if(urx2_tout == 0)
			urx2_comp = 1;
	}
	
	if(callback_1ms == 10)
	{
		callback_1ms = 0;
		callback_10ms++;
		
		if(keycont_timeout)
			keycont_timeout--;

		if(coolled_timeout)
			coolled_timeout--;
		
		Buzzer_Sequence();          // Buzzer
	}
	
	if(callback_30ms == 30)
	{
		callback_30ms = 0;
		Key_Scan();
	}
}
//--------------------------------------------------------------------------------------
void Timer_Proc(void)     
{
    if(callback_10ms >= 10)			// Every 100ms
    {
        callback_10ms = 0;
        callback_100ms++;
        callback_500ms++;
	
		Send_LED_Data();			// LED Driver로 Segment 및 LED Data를 전송
		
		if(temper_timeout)			// Temper 감지시간
			temper_timeout--;
		
		if(e2p_save_time)
		{
			e2p_save_time--;
			if(e2p_save_time == 0)
				flg_e2p_save = 1;
		}
		
		if(disp_timeout)
		{
			disp_timeout--;
			if(disp_timeout == 0)
			{
				if(op_mode == RUN_MODE)
				{
					disp_mode = prev_dispmode;
					if(op_kind == AUTO_MODE)
						Autoled_on();
					else if(op_kind == STER_MODE)
						Sterled_on();
				}
				else
					disp_mode = DISP_WAIT;
			}
		}
		
		if(buzzer_play_timeout)
			buzzer_play_timeout--;
		
		if(smoke_run_timeout)
			smoke_run_timeout--;
		
		if(op_mode == RUN_MODE)
		{
			if(circula_fan_off_timeout)
			{
				circula_fan_off_timeout--;
				if(circula_fan_off_timeout == 0)
					COMM_CIRFAN_ON = 0;
			}
		}
		
		if(key_timeout)
			key_timeout--;
		
		if(airpresure_timeout)
			airpresure_timeout--;
		
		if(airpresure_chk_timeout)
			airpresure_chk_timeout--;
		
		if(sensor_check_timeout)
			sensor_check_timeout--;
		
		if(heatertemp_check_timeout)
			heatertemp_check_timeout--;
		
		flg_485_excute = !flg_485_excute;
		if(flg_485_excute)
			flg_485_send = 1;
		
		if(reserve_timeout)						// 예약 입력시간 타임아웃
			reserve_timeout--;
    }

    if(callback_500ms == 5)      // 0.5SEC
    {
        callback_500ms = 0;
        flg_toggle_500ms = !flg_toggle_500ms;
		HAL_GPIO_TogglePin(OPER_LED1_GPIO_Port, OPER_LED1_Pin);
		
		//mprintf("%d,%d,%d,%d,%d,%d,%d\n", key_pushed[0], key_pushed[1], key_pushed[2], key_pushed[3], key_pushed[4], key_pushed[5], key_pushed[6]);
		//mprintf("SMOKE State: %d\n", COMM_SMOKE_HEATEROUT_STATE);
    }

    if(callback_100ms == 10)      // 1SEC
    {
        callback_100ms = 0;
        callback_1min++;
		
		if(!(callback_1min % 5))
			flg_toggle_5sec = !flg_toggle_5sec;
		
		if(oper_timeout)
			oper_timeout--;
		
		if(op_mode == STOP_MODE)
		{
			if(COMM_EXT_TEMP > 50)
			{
				flg_cirfan_out = 0;				// 순환 팬
				flg_ventilfan_out = 1;			// 배기 팬
				flg_heater_out = 0;				// 히터
				flg_duct_out = 1;				// 덕트 Open Close
				flg_uvlamp_out = 0;
			}
			else
			{
				flg_cirfan_out = 0;				// 순환 팬
				flg_ventilfan_out = 0;			// 배기 팬
				flg_heater_out = 0;				// 히터
				flg_duct_out = 0;				// 덕트 Open Close
				flg_uvlamp_out = 0;
			}
		}
		
		if(oper_total_timeout)				// for cert
			oper_total_timeout--;
		
		//if(flg_check_humi_start)
		if(humi_check_timeout)
			humi_check_timeout--;
		
		if(ster_run_timeout)
			ster_run_timeout--;
		
		if(optkey_timeout)
		{
			optkey_timeout--;
			if(optkey_timeout == 0)
			{
				disp_mode = DISP_WAIT;
				Waitled_on();
			}
		}
		
		if(error_timeout)
			error_timeout--;
		
		if(cir_err_timeout)
			cir_err_timeout--;

		if(heater_err_timeout)
			heater_err_timeout--;
		
		if(smokeheater_err_timeout)
			smokeheater_err_timeout--;
		
		if(liquidout_err_timeout)
			liquidout_err_timeout--;
		
		if(airpresure_err_timeout)
			airpresure_err_timeout--;
		
		if((op_mode == RUN_MODE) && (flg_heater_runrate_first == 1))
		{
			if(flg_heater_out)
				heater_running_time++;
			
			heater_run_total_time++;
		}
		
		// 필터교환시기 알려주는 동작 시간
		if(flg_cirfan_out)
		{
			Flashdatarec.e2p_operated_time++;
			flg_oper_time_save = 1;
		}
		else
		{
			if(flg_oper_time_save)
			{
				flg_oper_time_save = 0;
				flg_e2p_save = 1;
			}
		}
		
		if(reserve_clock)
			reserve_clock--;
		
		if(reserve_clock == 0)
		{
			if(flg_reserve)
			{
				if(op_mode == STOP_MODE)
					menu_key[AUTO_KEY] = 1;		// 자동 모드로 동작
			}
			
			reserve_clock = 86400;			// 24시간
		}
		
		if(flg_dryend)
		{
			if(playend_count)
			{
				playend_count--;
				Play_Buzzer(9);
			}
			else
				flg_dryend = 0;
		}
		
		if(humi_err_timeout)
			humi_err_timeout--;
		
		if(oper_remain_time)
			oper_remain_time--;
	}

    if(callback_1min >= 60)   // 1Min
    {
        callback_1min = 0;

    }
}
//----------------------------------------------------------------------------
void Key_Scan(void)
{
    uint8_t i;

	keyin_buf[RUNSTOP_KEY][key_idx] = POWER_BTN;
	keyin_buf[AUTO_KEY][key_idx] = AUTO_BTN;
	keyin_buf[RESERVE_KEY][key_idx] = RESERV_BTN;
	keyin_buf[STER_KEY][key_idx] = STER_BTN;
	keyin_buf[SMOKE_KEY][key_idx] = SMOKE_BTN;
	keyin_buf[UP_KEY][key_idx] = UP_BTN;
	keyin_buf[DOWN_KEY][key_idx] = DOWN_BTN;

    key_idx++;
	if(key_idx >= 3)
		key_idx = 0;

    for(i=0; i < KEY_COUNT; i++)
    {
        if((keyin_buf[i][0] == 0) && (keyin_buf[i][1] == 0) && (keyin_buf[i][2] == 0))	// Key가 눌렸을 경우
        {
            if(key_pushed[i] == 0)                        // Key Pushed Flag 처음일 경우
            {
                key_pushed[i] = 1;
                menu_key[i] = 1;
                key_cont_count[i] = 0;
            }
            else
            {
                if(key_continued[i] == 0)
                {
                    if (key_cont_count[i] < 50)                                // 500ms
                        key_cont_count[i]++;
                    else
                    {
                        key_continued[i] = 1;
                        key_cont_count[i] = 0;
                    }
                }
            }
        }
        else
        {
			key_pushed[i] = 0;
            key_continued[i] = 0;
            key_cont_count[i] = 0;
        }
    }
}


