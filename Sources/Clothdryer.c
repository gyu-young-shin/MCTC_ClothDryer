#include "main.h"
#include "debug.h"
#include "clothdryer.h"
#include "buzzer.h"
#include "segment.h"
#include "flashrom.h"

_Bool		COMM_CIRFAN_ON = 0;				// ¼øÈ¯ ÆÒ
_Bool		COMM_VENTILFAN_ON = 0;			// ¹è±â? ÆÒ
_Bool		COMM_HEATER_ON = 0;				// ÆÒÆÒ
_Bool		COMM_DUCT = 0;					// ´öÆ® Open Close
_Bool		COMM_AC220_SPARE1 = 0;			// AC SPARE
_Bool		COMM_UVLAMP_ON = 0;				// UV LAMP
_Bool		COMM_LEDLAMP_ON = 0;			// LED LAMP
_Bool		COMM_SMOKE_HEATER_ON = 0;		// SMOKE HEATER
_Bool		COMM_SMOKE_PUMP_ON = 0;			// SMOKE PUMP

uint8_t 	COMM_SPEED_CIRCULA = 0;			// ¼øÈ¯ÆÒ ï¿½Óµï¿½
uint8_t 	COMM_SPEED_VENTILATION = 0;		// ÆÒ¹è±â? ï¿½Óµï¿½

_Bool		COMM_DOOR_SW = 0;
_Bool		COMM_AIRPRESURE_SW = 0;			// Ç³ÆÒ ÆÒÆÒÄ¡
_Bool		COMM_SMOKE_HEATEROUT_STATE = 0;

int8_t 		COMM_EXT_TEMP = 0;
int16_t		COMM_HEATER_TEMP = 0;
uint8_t 	COMM_EXT_HUMI = 0;

_Bool		disp_toggle = 0;
_Bool		temper_open_error = 0;
_Bool		flg_key_start = 0;
_Bool		flg_temperchk = 0;
_Bool		flg_airpresurechk = 0;
_Bool		flg_temptime_sel = 0;
_Bool		flg_e2p_save = 0;
_Bool		flg_set_temp_key = 0;
_Bool		flg_set_humi_key = 0;
_Bool		flg_temper_open_check = 0;
_Bool		flg_cwd = 0;					// 0 CWD, 1: CCWD
_Bool		flg_first_error = 0;			// Ã¹¹øÂ° ¿¡·¯


_Bool		flg_cirfan_out = 0;				// ¼øÈ¯ ÆÒ
_Bool		flg_ventilfan_out = 0;			// ¹è±â? ÆÒ
_Bool		flg_heater_out = 0;				// ÆÒÆÒ
_Bool		flg_duct_out = 0;				// ´öÆ® Open Close
_Bool		flg_uvlamp_out = 0;
_Bool		flg_smoke_heater_out = 0;
_Bool		flg_smoke_pump_out = 0;
_Bool		flg_dry_running = 0;
_Bool		flg_smoke_select = 0;		// ÆÒÐ»ï¿? ÆÒÆÒ
_Bool		flg_goal_temp_ok = 0;		// ÆÒÇ¥ï¿½Âµï¿½ ÆÒÆÒ ÆÒÆÒ
_Bool		flg_heater_onoff = 0;		// ÆÒÆÒ On Off ÆÒÆÒ
_Bool		flg_airpresure_in = 0;		// Ç³ï¿½Ð½ï¿½ÆÒÄ¡ ï¿½Ô·ï¿½
_Bool		flg_hsen_error = 0;			// ÆÒÆÒÆÒÆÒ ÆÒÆÒ


_Bool		flg_check_humi_start = 0;		// ÆÒÆÒ ÆÒÆÒ ÆÒÆÒ ï¿½Ã·ï¿½ÆÒ
_Bool		flg_check_humi_first = 0;		// ÆÒÆÒ ÆÒÆÒ ÆÒÆÒ ÆÒÆÒ
_Bool		flg_check_ster_first = 0;		// ¹è±â? ÆÒ¹è±â½Ã°ï¿½ Ä«´öÆ® ï¿½Ã·ï¿½ÆÒ
_Bool		flg_first_door_open = 0;		// Ã³ÆÒ ÆÒÆÒÆÒ Ã¼Å©
_Bool		flg_circula_fan_backup = 0;		// ¼øÈ¯ÆÒ ¹è±â½Çµï¿½ ¹è±â? ï¿½Ã·ï¿½ÆÒ

_Bool		flg_option_start = 0;
_Bool		flg_cont_proc = 0;
_Bool	 	before_status = 0;
_Bool	 	flg_errscan = 1;
_Bool	 	flg_err_first = 0;

_Bool	 	flg_cirtimeout_first = 0;
_Bool	 	flg_heater_checktimeout_first = 0;
_Bool	 	flg_smokeheater_timeout_first = 0;
_Bool	 	flg_liquid_timeout_first = 0;
_Bool	 	flg_airpresure_timeout_first = 0;

_Bool	 	flg_heater_runrate_first = 0;
_Bool	 	flg_heatertemp_check_first = 0;

_Bool	 	flg_reserve = 0;
_Bool		flg_coolling = 0;
_Bool		flg_coolled_amulating = 0;


uint8_t		key_timeout = 0;
uint8_t		opt_num = 1;
uint8_t		optkey_timeout = 0;										// key timeout

uint8_t		circula_fan_speed_level = 10;
uint8_t		ventilation_fan_speed_level = 10;
uint8_t		circula_fan_off_timeout = 0;

uint8_t		circula_fan_speed_backup = 0;

uint8_t		temper_timeout = 0;
uint8_t		airpresure_timeout = 0;

uint8_t		bldcdelay_timeout = 0;
uint16_t	error_code = 0;
uint16_t  	err_mask = 0x0001;
uint8_t 	op_mode = STOP_MODE;
uint8_t 	op_kind = AUTO_MODE;
uint8_t 	disp_mode = DISP_WAIT;
uint8_t 	prev_dispmode = DISP_WAIT;
uint8_t 	prev_mode = DISP_WAIT;
uint8_t 	disp_subnum = 0;

uint8_t 	buzzer_play_timeout = 0;
uint8_t 	keycont_timeout = 0;
uint8_t 	e2p_save_time = 0;
uint8_t 	disp_timeout = 0;
uint16_t	humi_check_timeout = 0;		// ÆÒÆÒÆÒ ÆÒÆÒÆÒ 10ÆÒ ï¿½Ì»ï¿½ ÆÒÆÒ ï¿½Ã°ï¿½
uint8_t		compare_check_humi = 0;		// ¹è±â½Ï±ï¿½ ÆÒÆÒ ÆÒÆÒÆÒ ÆÒÆÒ
uint8_t		humi_remain_time = 0;		// ÆÒÆÒÆÒ ÆÒÆÒ ï¿½Ã°ï¿½

uint8_t		oper_step = 0;
uint8_t		smoke_step = 0;
uint8_t		cool_step = 0;
uint16_t 	oper_timeout = 0;
uint16_t 	oper_total_timeout = 0;
uint16_t 	ster_run_timeout = 0;
uint16_t 	smoke_run_timeout = 0;
uint8_t  	error_timeout = 0;
uint8_t  	cycle_count = 0;
uint8_t  	sensor_check_timeout = 10;

uint16_t 	cir_err_timeout = 0;		// ¼øÈ¯ºÒ·® Timeout
uint16_t 	heater_err_timeout = 0;		// È÷ÅÍºÒ·® Timeout
uint16_t 	smokeheater_err_timeout = 0;// ¿¬¹«±âºÒ·® Timeout
uint16_t 	liquidout_err_timeout = 0;	// ¿¬¹«¾× ¾øÀ½ Timeout
uint16_t 	airpresure_err_timeout = 0;	// Ç³ï¿½Ð½ï¿½ÆÒÄ¡ ï¿½Ò·ï¿½ Timeout
uint16_t 	airpresure_chk_timeout = 0;	// Ç³ï¿½Ð½ï¿½ÆÒÄ¡ Check Timeout
uint16_t 	heater_running_time = 0;	// ÆÒÆÒ ÆÒ¹è±â½Ã°ï¿½
uint16_t 	heater_run_total_time = 0;	// ÆÒÆÒ ï¿½âµ¿ÆÒÆÒ ÆÒÆÒÆÒ ¹è±â? ï¿½Ñ½Ã°ï¿½
uint16_t 	heatertemp_check_timeout = 0;

int16_t		prev_heater_temper = 0;
uint16_t 	coolling_timeout = 0;
int16_t		prev_cooling_temper = 0;
uint32_t	reserve_clock = 0;			// 24ï¿½Ã°ï¿½ ÆÒÆÒ ÆÒÆÒ
uint8_t  	reserve_hour = 0;
uint8_t  	reserve_minute = 0;
uint8_t  	reserve_timeout = 0;
uint16_t  	oper_remain_time = 99;
uint8_t  	coolled_timeout = 0;
uint8_t  	coolled_step = 0;
uint16_t  	humi_err_timeout = 0;

uint8_t  	playend_count = 0;
_Bool		flg_dryend = 0;

//=============================================================================
extern _Bool 	flg_toggle_500ms;
extern _Bool 	flg_rs485_error;
extern _Bool 	flg_toggle_5sec;

extern uint8_t  menu_key[KEY_COUNT];
extern uint8_t  key_pushed[KEY_COUNT];
extern uint8_t  key_continued[KEY_COUNT];
extern TE2PDataRec Flashdatarec;
extern uint8_t 	leddata[10];
//================================================================================================================
void Cloth_Dryer_Proc(void)
{
	if(flg_e2p_save)
	{
		flg_e2p_save = 0;
		FlashRom_WriteData();
	}
	
	Error_Recovery_Proc();
	Calculate_Humi();
	
	if(op_mode == RUN_MODE)
	{
		//----------------------------------------------------------------  ÆÒÆÒ
		if(error_code == 0)
		{
			flg_first_error = 0;								// Ã¹¹øÂ° ¿¡·¯
			
			switch(op_kind)										// ¿îÀü ¼±ÅÃ¸ðµå
			{
				case AUTO_MODE:									// ÀÚµ¿¿îÀü
					Auto_Proc();

					break;
				case STER_MODE:									// »ì±Õ/Å»Ãë¸ðµå
					Ster_Proc();
					break;
				case IDLE_MODE:									// IDLE ¸ðµå¿¡¼­´Â +Çâ ¹öÆ°ÀÌ ´­·ÁÀÖ´Â°É Á¶»çÇÑ´Ù.
					Cooling_Proc();								// ³Ã°¢
					if(flg_smoke_select == 1)
					{
						if(cool_step == 100)
							Smoke_Proc();
					}
					else
					{
						Stop_Proc();
					}
					break;
				default:
					break;
			}
		}
		else
		{
			// flg_cirfan_out = 0;				// ¼øÈ¯ ÆÒ
			// flg_ventilfan_out = 0;			// ¹è±â? ÆÒ
			// flg_heater_out = 0;				// ÆÒÆÒ
			// flg_smoke_heater_out = 0;		// ÆÒÆÒ ÆÒÆÒ
			// flg_smoke_pump_out = 0;			// ÆÒÆÒ ÆÒÆÒ
			// flg_duct_out = 0;				// ´öÆ® Close
			// flg_uvlamp_out = 0;
			// Stop_Proc();
			if(flg_first_error == 0)
			{
				flg_first_error = 1;
				cool_step = 0;
			}
			
			Cooling_Proc();					// ³Ã°¢
			
			if(coolled_step == 100)
			{
				flg_cirfan_out = 0;				// ¼øÈ¯ ÆÒ
				flg_ventilfan_out = 0;			// ¹è±â ÆÒ
				flg_heater_out = 0;				// È÷ÅÍ
				flg_smoke_heater_out = 0;		// ¿¬¹« È÷ÅÍ
				flg_smoke_pump_out = 0;			// ¿¬¹« ÆßÇÁ
				flg_duct_out = 0;				// ´öÆ® Close
				flg_uvlamp_out = 0;
				Stop_Proc();
			}
		}
	}
	else								// 2021-08-25 ï¿½Ú±ï¿½ÆÒ ÆÒÆÒ ÆÒÃ» ÆÒÆÒÆÒ ÆÒÆÒÆÒ ï¿½Ù·ï¿½ ÆÒÆÒ
	{
		Cooling_Proc();
	}
}
//-----------------------------------------------------------------------------------------------------------------
void Calculate_Humi(void)
{
	if(COMM_EXT_HUMI >= 70)
		humi_remain_time = 99;
	else if(COMM_EXT_HUMI >= 60)
		humi_remain_time = 80;
	else if(COMM_EXT_HUMI >= 50)
		humi_remain_time = 60;
	else if(COMM_EXT_HUMI >= 40)
		humi_remain_time = 40;
	else if(COMM_EXT_HUMI >= 30)
		humi_remain_time = 30;
	else if(COMM_EXT_HUMI >= 10)
		humi_remain_time = 20;
	else 
		humi_remain_time = 10;
}
//-----------------------------------------------------------------------------------------------------------------
// ÀÚµ¿¿îÀü 
void Auto_Proc(void)
{
	uint8_t tmp;
	
	switch(oper_step)
	{
		case 0:
			flg_cirfan_out = 0;				// ¼øÈ¯ ÆÒ
			ventilation_fan_speed_level = 10;			// 2021-08-30 ï¿½Ú±ï¿½ÆÒ ÆÒÆÒ ÆÒÃ»ÆÒÆÒ ÆÒÆÒ 10ÆÒÆÒ ÆÒÆÒ
			flg_ventilfan_out = 1;			// ¹è±â ÆÒ
			flg_heater_out = 0;				// ÆÒÆÒ
			flg_smoke_heater_out = 0;		// ÆÒÆÒ ÆÒÆÒ
			flg_smoke_pump_out = 0;			// ÆÒÆÒ ÆÒÆÒ
			flg_duct_out = 1;				// ´öÆ® Open Close
			flg_uvlamp_out = 0;
			oper_timeout = 20;				// 20ÆÒ ÆÒÆÒÆÒÆÒ ÆÒÆÒ
			oper_step = 1;
			oper_total_timeout = 7200;		// ÆÒ ÆÒÆÒ ÆÒÆÒ ï¿½Ã°ï¿½ 2ï¿½Ã°ï¿½
			flg_goal_temp_ok = 0;
			cycle_count = 0;
			flg_heater_runrate_first = 0;
			flg_coolling = 0;
			flg_coolled_amulating = 1;
			flg_cwd = 0;
			break;
		case 1:
			if(oper_timeout == 0)
			{
				if(COMM_EXT_TEMP < 50)		// ³»ºÎ¿Âµµ°¡ 50µµ ¹Ì¸¸ÀÌ¸é 
				{
					circula_fan_speed_level = 10;
					flg_cirfan_out = 1;				// ¼øÈ¯ ÆÒ
					flg_heater_out = 1;				// ÆÒÆÒ
					flg_duct_out = 0;				// ´öÆ® Open Close
					flg_ventilfan_out = 0;			// ¹è±â? ÆÒ
					flg_uvlamp_out = 1;
					oper_step = 2;
					flg_cwd = 1;
				}
				else
				{
					oper_step = 3;				// È¯¹è±â? ï¿½Ìµï¿½
				}
			}
			break;
		case 2:
			if(COMM_EXT_TEMP >= 50)									// ¹è±â½Î¿Âµï¿½ 50ÆÒÆÒ ÆÒÆÒ ï¿½Ï¸ï¿½
			{
				oper_step = 3;
			}
			else
			{
				if(oper_total_timeout == 0)
					oper_step = 5;
			}
			break;
		case 3:														// 50ÆÒ ÆÒÆÒ ÆÒÆÒ È¯ÆÒ
			tmp = Flashdatarec.e2p_auto_temp - 50;						// ÆÒ¹è±â½Âµï¿½ ÆÒÆÒ 50ÆÒ ÆÒÅ­ 1ÆÒÆÒÆÒ 1ÆÒ ï¿½ß°ï¿½
			if((humi_remain_time + tmp) > 99)
				tmp = 0;
			oper_remain_time = (humi_remain_time + tmp) * 60;
		
			flg_coolled_amulating = 0;
			flg_cwd = 0;
			circula_fan_speed_level = 10;
			flg_cirfan_out = 1;										// ¼øÈ¯ ÆÒ
			flg_heater_out = 1;										// ÆÒÆÒ
			flg_duct_out = 1;										// ´öÆ® Open Close
			ventilation_fan_speed_level = 5;
			flg_ventilfan_out = 1;									// ¹è±â? ÆÒ
			flg_uvlamp_out = 1;
		
			cycle_count = 0;
			flg_heater_runrate_first = 0;
			heater_running_time = 0;
			heater_run_total_time = 0;
		
			flg_goal_temp_ok = 0;
			flg_heater_onoff = 1;									// ÆÒÆÒ On Off ÆÒÆÒ ï¿½Ã·ï¿½ÆÒ
			flg_check_humi_first = 1;
			compare_check_humi = COMM_EXT_HUMI;	
			oper_step = 4;
			break;
		case 4:														// ÆÒ¹è±â½Âµï¿½ ¹è±â½Þ½ï¿½ ÆÒÆÒ ÆÒÆÒ
			if(flg_goal_temp_ok)
			{
				ventilation_fan_speed_level = 6;					// ÆÒÇ¥ï¿½Âµï¿½ ¹è±â½Þ½ï¿½ È¯ÆÒ 50%
				
				if((compare_check_humi - COMM_EXT_HUMI) > 2)		// ÆÒÆÒÆÒÆÒ 2% ï¿½Ì»ï¿½ï¿½Ì¶ï¿½ï¿?
				{
					compare_check_humi = COMM_EXT_HUMI;				// ÆÒÆÒ ï¿½ñ±³°ï¿½ÆÒ ÆÒ¹è±â½Ñ´ï¿½.
					humi_check_timeout = 600;						// 10ÆÒ
				}
				
				if(humi_check_timeout == 0)							// ÆÒÆÒÆÒÆÒ ÆÒÆÒ 10ÆÒ ï¿½Ì»ï¿½ ÆÒÆÒÆÒ
					oper_step = 5;									// ÆÒÆÒÆÒÆÒ ÆÒÆÒ
			}
			
//			if(flg_check_humi_first)
//			{
//				if(COMM_EXT_HUMI <= Flashdatarec.e2p_auto_humi)		// ÆÒÆÒÆÒ¹è±â? ÆÒÆÒÆÒÆÒ ÆÒ¹è±â½Ï°ï¿½ï¿?
//				{
//					flg_goal_temp_ok = 1;							// ÆÒÇ¥ ï¿½Âµï¿½ ÆÒÆÒ
//					flg_check_humi_first = 0;
//					humi_check_timeout = 600;						// 10ÆÒ
//					compare_check_humi = COMM_EXT_HUMI;				// ÆÒÆÒ ï¿½ñ±³°ï¿½ÆÒ ÆÒ¹è±â½Ñ´ï¿½.
//				}
//			}
			
			
			if(flg_heater_onoff)									// ÆÒÆÒ ÆÒÆÒ¹è±â½Ì¸ï¿½
			{
				if(Flashdatarec.e2p_auto_temp <= COMM_EXT_TEMP)		// ÆÒ¹è±â½Âµï¿½(ÆÒÇ¥ï¿½Âµï¿½) ï¿½Ì»ï¿½ÆÒ ¹è±â?
				{
					flg_goal_temp_ok = 1;							// ÆÒÇ¥ ï¿½Âµï¿½ ÆÒÆÒ
					flg_heater_onoff = 0;							// ÆÒÆÒ On Off ÆÒÆÒ ï¿½Ã·ï¿½ÆÒ
					flg_heater_out = 0;								// ÆÒÆÒ ¹è±â?
					if(flg_check_humi_first)
					{
						flg_check_humi_first = 0;
						humi_check_timeout = 600;					// 10ÆÒ
						compare_check_humi = COMM_EXT_HUMI;			// ÆÒÆÒ ï¿½ñ±³°ï¿½ÆÒ ÆÒ¹è±â½Ñ´ï¿½.
						//humi_check_timeout = 60;					// 10ÆÒ
					}
					
					if(flg_heater_runrate_first)
						cycle_count++;
				}
			}
			else
			{
				if(COMM_EXT_TEMP <= (Flashdatarec.e2p_auto_temp - 5))		// ÆÒ¹è±â½Âµï¿½(ÆÒÇ¥ï¿½Âµï¿½) - 5 ÆÒÆÒÆÒ ¹è±â?
				{
					flg_heater_onoff = 1;							// ÆÒÆÒ On Off ÆÒÆÒ ï¿½Ã·ï¿½ÆÒ
					flg_heater_out = 1;								// ÆÒÆÒ ¹è±â?
					if(flg_heater_runrate_first == 0)
					{
						flg_heater_runrate_first = 1;
						heater_running_time = 0;
						heater_run_total_time = 0;
					}
				}
			}
			
			if(oper_total_timeout == 0)
				oper_step = 5;
			
			if(flg_hsen_error)										// ÆÒÆÒ ÆÒÆÒ ÆÒÆÒÆÒ 30ÆÒ ÆÒ ÆÒÆÒ ï¿½Ï°ï¿½ ÆÒÆÒ
			{
				if(humi_err_timeout == 0)
					oper_step = 5;
			}
			break;
		case 5:														// ï¿½Ã°ï¿½ ÆÒÆÒ
			//mprintf("Cooling Process..\n");
			flg_coolled_amulating = 1;
			flg_cwd = 0;
			flg_coolling = 1;
			coolling_timeout = 1800;								// 30ºÐ 
			prev_cooling_temper = COMM_EXT_TEMP;
		
			circula_fan_speed_level = 10;
			flg_cirfan_out = 1;										// ¼øÈ¯ ÆÒ
			flg_heater_out = 0;										// ÆÒÆÒ
			flg_duct_out = 1;										// ´öÆ® Open Close
			ventilation_fan_speed_level = 6;						// 2021-07-06 ¹è±â½Î±×·ï¿½ ÆÒÆÒÆÒÃ»ÆÒÆÒ 10ÆÒÆÒ ÆÒÆÒ
			flg_ventilfan_out = 1;									// ¹è±â? ÆÒ
			flg_uvlamp_out = 1;
			oper_step = 6;
			flg_heater_runrate_first = 0;
		
			playend_count = 3;
			flg_dryend = 1;
			break;
		case 6:
			if(COMM_EXT_TEMP <= 50)									// ¹è±â½Î¿Âµï¿½ÆÒ 40ÆÒ ÆÒÆÒ
			{
				oper_step = 99;
			}
			else
			{
				if(oper_total_timeout == 0)
					oper_step = 99;
			}
			break;
		case 99:
			flg_cirfan_out = 0;										// ¼øÈ¯ ÆÒ
			flg_ventilfan_out = 0;									// ¹è±â? ÆÒ
			flg_heater_out = 0;										// ÆÒÆÒ
			flg_duct_out = 0;										// ´öÆ® Open Close
			flg_uvlamp_out = 0;
			oper_step = 100;
			smoke_step = 0;
			cool_step = 0;
			op_kind = IDLE_MODE;
			flg_coolled_amulating = 0;
			flg_cwd = 0;
			break;
		default:
			break;
	}
}
//-----------------------------------------------------------------------------------------------------------------
// »ì±Õ
void Ster_Proc(void)
{
	switch(oper_step)
	{
		case 0:
			flg_cirfan_out = 0;											// ¼øÈ¯ ÆÒ
			ventilation_fan_speed_level = 10;
			flg_ventilfan_out = 1;										// ¹è±â ÆÒ
			flg_heater_out = 0;											// È÷ÅÍ
			flg_smoke_heater_out = 0;									// ¿¬¹« È÷ÅÍ
			flg_smoke_pump_out = 0;										// ¿¬¹« ÆßÇÁ
			flg_duct_out = 1;											// ´öÆ® Open Close
			flg_uvlamp_out = 0;
			oper_timeout = 20;											// 20ÃÊ ÇÁ¸®ÆÛÁö ÁøÇà
			oper_step = 1;
		
			cycle_count = 0;
			flg_heater_runrate_first = 0;
			flg_coolling = 0;
			flg_coolled_amulating = 1;
			flg_cwd = 0;
			break;
		case 1:
			if(oper_timeout == 0)
			{
				flg_coolled_amulating = 0;
				flg_cwd = 0;
				circula_fan_speed_level = 10;
				flg_cirfan_out = 1;										// ¼øÈ¯ ÆÒ
				flg_heater_out = 1;										// È÷ÅÍ
				flg_duct_out = 0;										// ´öÆ® Close
				flg_ventilfan_out = 0;									// ¹è±â ÆÒ
				flg_uvlamp_out = 1;
			
				cycle_count = 0;
				flg_heater_runrate_first = 0;
				heater_running_time = 0;
				heater_run_total_time = 0;
				
				flg_goal_temp_ok = 0;
				flg_heater_onoff = 1;									// È÷ÅÍ On Off Á¦¾î ÇÃ·¡±×
				flg_check_ster_first = 1;
				oper_step = 2;
				oper_total_timeout = 7200;								// 2½Ã°£
				
				// ¼³Á¤¿Âµµ¿Í ÇöÀç¿ÂµµÀÇ Â÷ 1µµ ¸¶´Ù 1ºÐ¾¿
				oper_remain_time = (Flashdatarec.e2p_ster_temp - COMM_EXT_TEMP + 15) * 60;
			}
			break;
		case 2:
			if(flg_goal_temp_ok)
			{
				ventilation_fan_speed_level = 6;
				flg_ventilfan_out = 1;										// ¹è±â ÆÒ
				flg_duct_out = 1;											// ´öÆ® Open Close
				
				if(ster_run_timeout == 0)
					oper_step = 3;
			}
			
			if(flg_heater_onoff)									// È÷ÅÍ °¡µ¿ÁßÀÌ¸é
			{
				if(Flashdatarec.e2p_ster_temp <= COMM_EXT_TEMP)		// ¼³Á¤¿Âµµ(¸ñÇ¥¿Âµµ) ÀÌ»óÀÏ °æ¿ì
				{
					flg_goal_temp_ok = 1;							// ¸ñÇ¥ ¿Âµµ µµ´Þ
					flg_heater_onoff = 0;							// È÷ÅÍ On Off Á¦¾î ÇÃ·¡±×
					flg_heater_out = 0;								// È÷ÅÍ Ãâ·Â
					
					if(flg_check_ster_first)
					{
						flg_check_ster_first = 0;
						ster_run_timeout = Flashdatarec.e2p_ster_time * 60;	// 15ºÐ 
					}
					
					if(flg_heater_runrate_first)
						cycle_count++;
				}
			}
			else
			{
				if(COMM_EXT_TEMP <= (Flashdatarec.e2p_ster_temp - 5))		// ¼³Á¤¿Âµµ(¸ñÇ¥¿Âµµ) - 5 ÀÌÇÏÀÏ °æ¿ì
				{
					flg_heater_onoff = 1;							// È÷ÅÍ On Off Á¦¾î ÇÃ·¡±×
					flg_heater_out = 1;								// È÷ÅÍ Ãâ·Â
					
					if(flg_heater_runrate_first == 0)
					{
						flg_heater_runrate_first = 1;
						heater_running_time = 0;
						heater_run_total_time = 0;
					}
				}
			}
			
			if(oper_total_timeout == 0)
				oper_step = 3;
			break;
		case 3:														// ³Ã°¢ °úÁ¤
			flg_coolling = 1;
			flg_coolled_amulating = 1;
			flg_cwd = 0;
			coolling_timeout = 1800;								// 30ºÐ
			prev_cooling_temper = COMM_EXT_TEMP;
			
			circula_fan_speed_level = 10;
			flg_cirfan_out = 1;										// ¼øÈ¯ ÆÒ
			flg_heater_out = 0;										// È÷ÅÍ
			flg_duct_out = 1;										// ´öÆ® Open Close
			ventilation_fan_speed_level = 10;						// 2021-07-06 ÇÁ·Î±×·¥ ¼öÁ¤¿äÃ»À¸·Î 10À¸·Î º¯°æ
			flg_ventilfan_out = 1;									// ¹è±â ÆÒ
			flg_uvlamp_out = 1;
			flg_heater_runrate_first = 0;
			oper_step = 4;
		
			playend_count = 3;
			flg_dryend = 1;
			break;
		case 4:
			if((COMM_EXT_TEMP <= 50) && (COMM_HEATER_TEMP <= 60))	// ³»ºÎ¿Âµµ°¡ 50µµ ÀÌÇÏ
			{
				oper_step = 99;
			}
			else
			{
				if(oper_total_timeout == 0)
					oper_step = 99;
			}
			break;
		case 99:
			flg_cirfan_out = 0;										// ¼øÈ¯ ÆÒ
			flg_ventilfan_out = 0;									// ¹è±â? ÆÒ
			flg_heater_out = 0;										// ÆÒÆÒ
			flg_duct_out = 0;										// ´öÆ® Open Close
			flg_uvlamp_out = 0;
			oper_step = 100;
			smoke_step = 0;
			cool_step = 0;
			op_kind = IDLE_MODE;
			flg_coolled_amulating = 0;
			flg_cwd = 0;
			break;
		default:
			break;
	}
}
//-----------------------------------------------------------------------------------------------------------------
// ¿¬¹«µ¿ÀÛ
void Smoke_Proc(void)
{
	switch(smoke_step)
	{
		case 0:
			flg_coolled_amulating = 1;
			flg_cwd = 0;
			flg_smoke_heater_out = 1;					// smoke heater on
			flg_smoke_pump_out = 0;
			smoke_step = 1;
			oper_timeout = 5;							// 5ÆÒ
			break;
		case 1:
			if(COMM_SMOKE_HEATEROUT_STATE)				// Smoke Heater BimetalÆÒ ¹è±â? ÆÒÆÒ
			{
				oper_timeout = 180;						// 3ÆÒ
				smoke_step = 2;
			}
			else
			{
				if(oper_timeout == 0)
				{
					oper_timeout = 2400;				// 40 ÆÒ					
					smoke_step = 10;
				}
			}
			break;
		case 2:
			if(COMM_SMOKE_HEATEROUT_STATE == 0)				// Smoke Heater BimetalÆÒ ÆÒÆÒÆÒÆÒ
			{
				flg_smoke_heater_out = 0;					// smoke heater off
				flg_smoke_pump_out = 1;
				
				circula_fan_speed_level = 10;
				flg_cirfan_out = 1;							// ¼øÈ¯ ÆÒ
				
				smoke_run_timeout = 60;						// 6ÆÒ
				smoke_step = 3;
			}
			else
			{
				if(oper_timeout == 0)
				{
					flg_smoke_heater_out = 0;				// smoke heater off
					flg_smoke_pump_out = 0;
					// ÆÒÆÒÃ³ÆÒ
					error_code |= E6_ERR_SET;				// for cert
					smoke_step = 99;
				}
			}
			break;
		case 3:
			if(smoke_run_timeout == 0)
			{
				flg_smoke_heater_out = 0;					// smoke heater off
				flg_smoke_pump_out = 0;				
				
				circula_fan_speed_level = 6;
				flg_cirfan_out = 1;							// ¼øÈ¯ ÆÒ
				oper_timeout = 120;							// 2ÆÒ
				smoke_step = 4;
			}
			break;
		case 4:
			if(oper_timeout == 0)
			{
				flg_cirfan_out = 0;							// ¼øÈ¯ ÆÒ
				smoke_step = 99;
			}
			break;
		case 10:
			if(COMM_SMOKE_HEATEROUT_STATE)				// Smoke Heater BimetalÆÒ ¹è±â? ÆÒÆÒ
			{
				oper_timeout = 180;						// 3ÆÒ
				smoke_step = 2;
			}
			else
			{
				if(oper_timeout == 0)
				{
					flg_smoke_heater_out = 0;				// smoke heater off
					flg_smoke_pump_out = 0;
						// ¿¡·¯Ã³¸®
					error_code |= E6_ERR_SET;				// for cert
					smoke_step = 99;
				}
			}
			break;
		case 99:
			flg_smoke_select = 0;
			smoke_step = 100;
			flg_coolled_amulating = 0;
			flg_cwd = 0;
			break;
		default:
			break;
	}
}
//-----------------------------------------------------------------------------------------------------------------
// Àü¿ø OFF½Ã ¶Ç´Â ¿îÀü Á¾·á½Ã ³»ºÎ ¿Âµµ¸¦ ³Ã°¢ ½ÃÅ²´Ù.
void Cooling_Proc(void)
{
	switch(cool_step)
	{
		case 0:
			if((COMM_EXT_TEMP <= 50) && (COMM_HEATER_TEMP < 100))										// ³»ºÎ¿Âµµ°¡ 50µµ ÀÌÇÏ
				cool_step = 99;
			else
			{
				cool_step = 1;
				disp_mode = DISP_TEMPTIME;
			}
			break;
		case 1:
			flg_coolled_amulating = 1;
			flg_cwd = 0;
			circula_fan_speed_level = 10;
			flg_cirfan_out = 1;											// ¼øÈ¯ ÆÒ
			ventilation_fan_speed_level = 10;
			flg_ventilfan_out = 1;										// ¹è±â? ÆÒ
			flg_heater_out = 0;											// È÷ÅÍ
			flg_smoke_heater_out = 0;									// ¿¬¹« È÷ÅÍ
			flg_smoke_pump_out = 0;										// ¿¬¹« ÆßÇÁ
			flg_duct_out = 1;											// ´öÆ® Open Close
			flg_uvlamp_out = 1;

			if((COMM_EXT_TEMP <= 50) && (COMM_HEATER_TEMP < 100))		// ¹è±â½Î¿Âµï¿½ÆÒ 50ÆÒ ÆÒÆÒ
				cool_step = 99;
			break;
		case 99:
			flg_cirfan_out = 0;											// ¼øÈ¯ ÆÒ
			flg_ventilfan_out = 0;										// ¹è±â? ÆÒ
			flg_heater_out = 0;											// È÷ÅÍ
			flg_smoke_heater_out = 0;									// ¿¬¹« È÷ÅÍ
			flg_smoke_pump_out = 0;										// ¿¬¹« ÆßÇÁ
			flg_duct_out = 0;											// ´öÆ® Open Close
			flg_uvlamp_out = 0;
			cool_step = 100;
			flg_coolled_amulating = 0;
			flg_cwd = 0;
		
			if((disp_mode != DISP_RESERVE) && (disp_mode != DISP_OPTION) && (flg_smoke_select == 0))
			{
				disp_mode = DISP_WAIT;
				//Waitled_on();
			}
			break;
		case 100:
			if((COMM_EXT_TEMP > 50) || (COMM_HEATER_TEMP >= 100))										// ³»ºÎ¿Âµµ°¡ 50µµ ÃÊ°ú
				cool_step = 0;
		default:
			break;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void Stop_Proc(void)
{
	op_mode = STOP_MODE;
		
	if((Flashdatarec.e2p_operated_time / 3600) >= 1000)					// 1000½Ã°£ ÀÌ»óÀÏ °æ¿ì
		disp_mode = DISP_FILTER;
	else
		disp_mode = DISP_WAIT;
	
	oper_step = 0;
	smoke_step = 0;
	cool_step = 0;
	flg_smoke_select = 0;
	//error_code &= EB_ERR_SET;		// 485 ¿¡·¯¸¦ Á¦¿ÜÇÑ ³ª¸ÓÁö ¿¡·¯¸¦ Å¬¸®¾î ÇÑ´Ù.
	flg_coolled_amulating = 0;
	flg_cwd = 0;
	Waitled_on();
	
	if(error_code == 0)
	{
		Buzzer_Stop();
	}
}
//-----------------------------------------------------------------------------------------------------------------
// Å° ï¿½Ô·ï¿½ Ã³ÆÒ
void InputKey_Proc(void)
{
	// Up, Down Key ¹è±â½Ã¿ï¿½ ÆÒÆÒÆÒ¹è±â? ÆÒÆÒ ï¿½Âµï¿½ÆÒ ÆÒ¹è±â½Ø´ï¿½.
	if(key_continued[UP_KEY] && key_continued[DOWN_KEY]  && (flg_key_start == 0) && (error_code == 0))
	{
		if((disp_mode != DISP_HEATER_TEMP) && (disp_mode != DISP_HUMIDITY))
		{
			if(disp_mode == DISP_RESERVE)
			{
				reserve_timeout = 0;
				flg_reserve = 0;
			}
			else if(disp_mode == DISP_TEMPSET)
			{
				prev_dispmode = prev_dispmode;
			}
			else
				prev_dispmode = disp_mode;
			
			flg_key_start = 1;
			disp_mode = DISP_HEATER_TEMP;
			disp_timeout = 0;					// 5sec
			Play_Buzzer(1);						// Button
		}
		else if(((disp_mode == DISP_HEATER_TEMP) || (disp_mode == DISP_HUMIDITY)) && (flg_key_start == 0))
		{
			flg_key_start = 1;
			disp_timeout = 1;
			Play_Buzzer(1);	
		}
		
		goto KEY_CLEAR;
	}
	
	if((menu_key[UP_KEY] || menu_key[DOWN_KEY]) && ((disp_mode == DISP_HEATER_TEMP) || (disp_mode == DISP_HUMIDITY)))
	{
		if(disp_mode == DISP_HEATER_TEMP)
			disp_mode = DISP_HUMIDITY;
		else
			disp_mode = DISP_HEATER_TEMP;

		disp_timeout = 0;					// 5sec
		Play_Buzzer(1);						// Button
		
		goto KEY_CLEAR;
	}
	
	//--------------------------------------------------------------------------------------------------------------------
	// ÆÒÆÒ ÆÒÆÒÆÒ ¹è±â? 
	if(temper_open_error)	
	{
		if((menu_key[UP_KEY]) || (menu_key[DOWN_KEY]) || (menu_key[AUTO_KEY]) || 
			(menu_key[RESERVE_KEY]) || (menu_key[STER_KEY]) || (menu_key[SMOKE_KEY]) || (menu_key[RUNSTOP_KEY]))
		{
			Play_Buzzer(0);				
		}
		
		goto KEY_CLEAR;
	}
	
	if(disp_mode == DISP_RESERVE)											// ÆÒÆÒ ÆÒÆÒÆÒ
	{
		if(reserve_timeout == 0)
		{
			reserve_clock = (reserve_hour * 3600) + (reserve_minute *  60);
			disp_mode = prev_dispmode;
			if(disp_mode == DISP_TEMPTIME)
			{
				if(op_mode == RUN_MODE)
				{
					if(op_kind == AUTO_MODE)
						Autoled_on();
					else if(op_kind == STER_MODE)
						Sterled_on();
					else
						Smokeled_on();
				}
				else
					Waitled_on();
			}
			
			if(op_mode == STOP_MODE)
			{
				cool_step = 0;
				disp_mode = DISP_WAIT;
			}

			flg_reserve = 1;
		}
		
		if((menu_key[DOWN_KEY] || key_continued[DOWN_KEY]) && (key_pushed[UP_KEY] == 0))
		{
			if(key_continued[DOWN_KEY])
			{
				if(flg_key_start == 0)
				{
					flg_key_start = 1;
					if(key_timeout == 0)
						key_timeout = 5;		// 500 msec
				}
			}
			
			if(key_timeout == 0)
			{
				if(key_continued[DOWN_KEY])
				{
					key_timeout = 2;
					Play_Buzzer(1);						// Button
				}

				if(disp_subnum == 0)					// ï¿½Ã°ï¿½ÆÒ
				{
					if(reserve_hour > 0)
						reserve_hour--;
				}
				else
				{
					if(reserve_minute > 0)
						reserve_minute--;
				}

				if(!key_continued[DOWN_KEY])
				{
					Play_Buzzer(1);						// Button
				}
			}
			
			menu_key[DOWN_KEY] = 0;
			reserve_timeout = 50;			// 5sec
		}
		else if((menu_key[UP_KEY] || key_continued[UP_KEY]) && (key_pushed[DOWN_KEY] == 0)) // Up Å°
		{
			if(key_continued[UP_KEY])
			{
				if(flg_key_start == 0)
				{
					flg_key_start = 1;
					if(key_timeout == 0)
						key_timeout = 5;		// 500msec
				}
			}
			
			
			if(key_timeout == 0)
			{
				if(key_continued[UP_KEY])
				{
					key_timeout = 2;
					Play_Buzzer(1);						// Button
				}
				
				if(disp_subnum == 0)					// ï¿½Ã°ï¿½ÆÒ
				{
					if(reserve_hour < 23)
						reserve_hour++;
				}
				else
				{
					if(reserve_minute < 59)
						reserve_minute++;
				}

				if(!key_continued[UP_KEY])
				{
					Play_Buzzer(1);						// Button
				}
				
			}
			menu_key[UP_KEY] = 0;
			reserve_timeout = 50;			// 5sec
		}
		
	}
	
	if(menu_key[RESERVE_KEY] && (key_pushed[AUTO_KEY] == 0) && (disp_mode != DISP_OPTION) && (error_code == 0))		// Reserve Key
	{
		if(flg_reserve)
			flg_reserve = 0;
		else
		{
			if(disp_mode != DISP_RESERVE)
			{
				disp_subnum = 0;
				prev_dispmode = disp_mode;
				disp_mode = DISP_RESERVE;
				reserve_hour = reserve_clock / 3600;
				reserve_minute = (reserve_clock % 3600) / 60;
				Led_data_set(1, LEDC_TIME, 1);				// ï¿½Ã°ï¿½
				Led_data_set(3, LEDC_MINUTE, 1);			// ÆÒ
				
				Led_data_set(1, LEDC_TEMP, 0);				// ï¿½Âµï¿½
				Led_data_set(3, LEDC_TIMEREMAIN, 0);		// ÆÒ¹è±â½Ã°ï¿½
				
				if(op_mode == STOP_MODE)
				{
					Led_data_set(6, LEDT_UP, 1);				// ÆÒÄ¡ ï¿½Ã¸ï¿½
					Led_data_set(7, LEDT_DOWN, 1);				// ÆÒÄ¡ ÆÒÆÒ
					Led_data_set(8, LEDI_UP, 1);				// ÆÒÆÒÆÒ ï¿½Ã¸ï¿½
					Led_data_set(8, LEDL_DOWN, 1);				// ÆÒÆÒÆÒ ÆÒÆÒ
				}
			}
			else
			{
				disp_subnum++;
				if(disp_subnum >= 2)
				{
					reserve_clock = (reserve_hour * 3600) + (reserve_minute *  60);
					disp_mode = prev_dispmode;
					flg_reserve = 1;
					
					if(disp_mode == DISP_TEMPTIME)
					{
						if(op_mode == RUN_MODE)
						{
							if(op_kind == AUTO_MODE)
								Autoled_on();
							else if(op_kind == STER_MODE)
								Sterled_on();
							else
								Smokeled_on();
						}
						else
							Waitled_on();
					}

					if(op_mode == STOP_MODE)
					{
						cool_step = 0;
						disp_mode = DISP_WAIT;
					}
				}
			}
			reserve_timeout = 50;			// 5sec
		}
		Play_Buzzer(1);
	}
	
	//-----------------------------------------------------------------------------------------------	
	if(op_mode == STOP_MODE)			// ÆÒÆÒ¹è±â?
	{
		if(menu_key[RUNSTOP_KEY] && (error_code > 0)) 
		{
			error_code = 0;
			Play_Buzzer(1);
		}
		else if(((menu_key[AUTO_KEY] && (key_pushed[RUNSTOP_KEY] == 0)) || (menu_key[STER_KEY]  && (key_pushed[AUTO_KEY] == 0)) || 
			menu_key[SMOKE_KEY]) && (disp_mode != DISP_RESERVE)  && (disp_mode != DISP_OPTION) && (error_code == 0))
		{
			if(error_code == 0)
			{
				disp_timeout = 0;
				op_mode = RUN_MODE;
				if(menu_key[AUTO_KEY])
				{
					smoke_step = 0;
					op_kind = AUTO_MODE;
					Autoled_on();
				}
				else if(menu_key[STER_KEY])
				{
					smoke_step = 0;
					op_kind = STER_MODE;
					Sterled_on();
				}
				else
				{
					flg_smoke_select = 1;
					cool_step = 0;												// ï¿½Ã°ï¿½ Ã³ÆÒÆÒ ï¿½Ñ´ï¿½.
					smoke_step = 0;
					Smokeled_on();
					Led_data_set(3, LEDC_SMOKE, 1);				// +ÆÒ
					op_kind = IDLE_MODE;
				}
				
				oper_step =  0;
				disp_mode = DISP_TEMPTIME;
				Buzzer_Stop();
				Play_Buzzer(3);				// Power On
			}
			else
				Play_Buzzer(0);				
		}
		else if((menu_key[RUNSTOP_KEY]) && (disp_mode == DISP_OPTION))     // ÆÒÆÒ Key
		{
				disp_timeout = 0;
				disp_mode = DISP_WAIT;
				e2p_save_time = 10;											// 1ÆÒ ÆÒ ÆÒÆÒ
				//Waitled_on();
				Play_Buzzer(1);
		}
		else if(key_continued[AUTO_KEY] && key_continued[RUNSTOP_KEY] && (disp_mode == DISP_WAIT) && (error_code == 0))				// Option Mode
		{
			if(flg_option_start == 0)
			{
				flg_option_start = 1;
				key_timeout = 20;            								// 2 SEC
				flg_cont_proc = 0;
			}
			else
			{
				if((key_timeout == 0) && (flg_cont_proc == 0))
				{
					disp_timeout = 0;
					flg_cont_proc = 1;
					disp_mode = DISP_OPTION;

					opt_num = 1;
					optkey_timeout = 10;										// key timeout
					Optionled_on();
					Play_Buzzer(1);												// Button
				}
			}
		}
		else if((menu_key[AUTO_KEY]) && (key_pushed[RESERVE_KEY] == 0) && (disp_mode == DISP_OPTION) && (error_code == 0))		// ÆÒÆÒÆÒÆÒ ÆÒÆÒ
		{
			if(disp_mode == DISP_OPTION)
			{
				optkey_timeout = 10;
				opt_num++;
				if(opt_num > 7)			
					opt_num = 1;
				
				e2p_save_time = 10;											// 1ÆÒ ÆÒ ÆÒÆÒ
				Play_Buzzer(1);													// Button
			}
			else
				Play_Buzzer(0);						// Not
		}	//----------------------------------------------------------------------------------------------------------
		else if((menu_key[RESERVE_KEY]) && (key_pushed[AUTO_KEY] == 0) && (disp_mode == DISP_OPTION) && (error_code == 0))		// ÆÒÆÒÆÒÆÒ ï¿½Ã¸ï¿½
		{
			if(disp_mode == DISP_OPTION)
			{
				optkey_timeout = 10;
				
				if(opt_num > 1)			
					opt_num--;
				else
					opt_num = 7;
				
				e2p_save_time = 10;											// 1ÆÒ ÆÒ ÆÒÆÒ
				Play_Buzzer(1);													// Button
			}
			else
				Play_Buzzer(0);						// Not
		}	//----------------------------------------------------------------------------------------------------------
		else if((menu_key[DOWN_KEY] || key_continued[DOWN_KEY]) && (key_pushed[UP_KEY] == 0) && \
				(disp_mode == DISP_OPTION) && (error_code == 0))				// Down Å°
		{
			if(key_continued[DOWN_KEY])
			{
				if(flg_option_start == 0)
				{
					flg_option_start = 1;
					if(key_timeout == 0)
						key_timeout = 5;		// 500 msec
				}
			}
			
			if(key_timeout == 0)
			{
				if(key_continued[DOWN_KEY])
				{
					key_timeout = 2;
					Play_Buzzer(1);						// Button
				}
					
				if(disp_mode == DISP_OPTION)
				{
					optkey_timeout = 10;
					e2p_save_time = 30;							// 3ÆÒ ÆÒ ÆÒÆÒ
					switch(opt_num)
					{
						case 1:										// ¼øÈ¯ ï¿½Ò·ï¿½ Ã¼Å©ÆÒ ï¿½Ç³ï¿½ï¿½Âµï¿½ÆÒ ¹è±â½â±¸ ï¿½Âµï¿½ÆÒ ÆÒÆÒ ¹è±â½â±¸ ï¿½Âµï¿½ 10ÆÒ ï¿½Ì»ï¿½
							if(Flashdatarec.e2p_cirfan_diff_temp > 1)
								Flashdatarec.e2p_cirfan_diff_temp--;
							break;
						case 2:										// ÆÒÆÒ ï¿½Ò·ï¿½ Ã¼Å©ÆÒ ï¿½Ç³ï¿½ï¿½Âµï¿½ÆÒ ¹è±â½â±¸ ï¿½Âµï¿½ÆÒ ÆÒÆÒ 1ÆÒÆÒÆÒ(ÆÒÆÒ ï¿½Ûµï¿½ 3ÆÒÆÒ ÆÒÆÒ Ã¼Å©)
							if(Flashdatarec.e2p_heater_diff_temp > 1)
								Flashdatarec.e2p_heater_diff_temp--;
							break;
						case 3:										// ï¿½Ê´ï¿½ÆÒ ÆÒÆÒÆÒÆÒ ÆÒÆÒÆÒ 40ÆÒ ï¿½Ì³ï¿½ÆÒ ÆÒÆÒÆÒÆÒ ¹è±â½Â½ï¿½È£ÆÒ ÆÒÆÒÆÒÆÒ ÆÒÆÒÆÒ ÆÒÆÒÆÒ ÆÒÆÒÆÒÆÒ Ã³¹è±â½Ñ´ï¿½.
							if(Flashdatarec.e2p_smokbimetal_time > 10)
								Flashdatarec.e2p_smokbimetal_time--;	
							break;
						case 4:										// È¯ÆÒ ÆÒÆÒÆÒ ÆÒÆÒÆÒ ÆÒÆÒÆÒ N% ï¿½Ñ¹ï¿½ï¿½Ì¶ï¿½ ÆÒ¹è±â½Âµï¿½ ÆÒÆÒ ¹è±â½Äºï¿½ÆÒ 3ÆÒÆÒÅ¬ ï¿½Ýºï¿½ï¿½Ä¿ï¿½ ÆÒÆÒ Ã¼Å©
							if(Flashdatarec.e2p_heater_oper_percent > 1)
								Flashdatarec.e2p_heater_oper_percent--;	
							break;
						case 5:										// ÆÒÆÒÆÒÆÒ ÆÒÆÒ N% ÆÒÆÒ				
							if(Flashdatarec.e2p_under_err_humi > 1)
								Flashdatarec.e2p_under_err_humi--;	
							break;
						case 6:										// ÆÒÆÒÆÒÆÒ ÆÒÆÒ N% ï¿½Ì»ï¿½
							if(Flashdatarec.e2p_over_err_humi > 80)
								Flashdatarec.e2p_over_err_humi--;
							break;
						case 7:										// ¹è±â½Í±ï¿½È¯ ï¿½Ã±ï¿½ ÆÒÆÒ ï¿½Ã°ï¿½ (1-99) ÆÒÆÒ 10ï¿½Ã°ï¿½
							if(Flashdatarec.e2p_opertime_set > 1)
								Flashdatarec.e2p_opertime_set--;
							break;
						default:
							break;
					}
					
					if(!key_continued[DOWN_KEY])
					{
						Play_Buzzer(1);						// Button
					}
				}
				else
				{
					if(!key_continued[DOWN_KEY])
					{
						Play_Buzzer(0);						// Not
					}
				}	
			}
		}	//----------------------------------------------------------------------------------------------------------
		else if((menu_key[UP_KEY] || key_continued[UP_KEY]) && (key_pushed[DOWN_KEY] == 0) && \
				(disp_mode == DISP_OPTION) && (error_code == 0))				// Up Å°
		{
			if(key_continued[UP_KEY])
			{
				if(flg_option_start == 0)
				{
					flg_option_start = 1;
					if(key_timeout == 0)
						key_timeout = 5;		// 500msec
				}
			}
			
			if(key_timeout == 0)
			{
				if(key_continued[UP_KEY])
				{
					key_timeout = 2;
					Play_Buzzer(1);						// Button
				}
					
				if(disp_mode == DISP_OPTION)
				{
					optkey_timeout = 10;
					e2p_save_time = 30;							// 3ÆÒ ÆÒ ÆÒÆÒ
					switch(opt_num)
					{
						case 1:										// ¼øÈ¯ ï¿½Ò·ï¿½ Ã¼Å©ÆÒ ï¿½Ç³ï¿½ï¿½Âµï¿½ÆÒ ¹è±â½â±¸ ï¿½Âµï¿½ÆÒ ÆÒÆÒ ¹è±â½â±¸ ï¿½Âµï¿½ 10ÆÒ ï¿½Ì»ï¿½
							if(Flashdatarec.e2p_cirfan_diff_temp < 30)
								Flashdatarec.e2p_cirfan_diff_temp++;
							break;
						case 2:										// ÆÒÆÒ ï¿½Ò·ï¿½ Ã¼Å©ÆÒ ï¿½Ç³ï¿½ï¿½Âµï¿½ÆÒ ¹è±â½â±¸ ï¿½Âµï¿½ÆÒ ÆÒÆÒ 1ÆÒÆÒÆÒ(ÆÒÆÒ ï¿½Ûµï¿½ 3ÆÒÆÒ ÆÒÆÒ Ã¼Å©)
							if(Flashdatarec.e2p_heater_diff_temp < 10)
								Flashdatarec.e2p_heater_diff_temp++;
							break;
						case 3:										// ï¿½Ê´ï¿½ÆÒ ÆÒÆÒÆÒÆÒ ÆÒÆÒÆÒ 40ÆÒ ï¿½Ì³ï¿½ÆÒ ÆÒÆÒÆÒÆÒ ¹è±â½Â½ï¿½È£ÆÒ ÆÒÆÒÆÒÆÒ ÆÒÆÒÆÒ ÆÒÆÒÆÒ ÆÒÆÒÆÒÆÒ Ã³¹è±â½Ñ´ï¿½.
							if(Flashdatarec.e2p_smokbimetal_time < 90)
								Flashdatarec.e2p_smokbimetal_time++;	
							break;
						case 4:										// È¯ÆÒ ÆÒÆÒÆÒ ÆÒÆÒÆÒ ÆÒÆÒÆÒ N% ï¿½Ñ¹ï¿½ï¿½Ì¶ï¿½ ÆÒ¹è±â½Âµï¿½ ÆÒÆÒ ¹è±â½Äºï¿½ÆÒ 3ÆÒÆÒÅ¬ ï¿½Ýºï¿½ï¿½Ä¿ï¿½ ÆÒÆÒ Ã¼Å©
							if(Flashdatarec.e2p_heater_oper_percent < 90)
								Flashdatarec.e2p_heater_oper_percent++;	
							break;
						case 5:										// ÆÒÆÒÆÒÆÒ ÆÒÆÒ N% ÆÒÆÒ				
							if(Flashdatarec.e2p_under_err_humi < 10)
								Flashdatarec.e2p_under_err_humi++;	
							break;
						case 6:										// ÆÒÆÒÆÒÆÒ ÆÒÆÒ N% ï¿½Ì»ï¿½
							if(Flashdatarec.e2p_over_err_humi < 99)
								Flashdatarec.e2p_over_err_humi++;
							break;
						case 7:										// ¹è±â½Í±ï¿½È¯ ï¿½Ã±ï¿½ ÆÒÆÒ ï¿½Ã°ï¿½ (1-99) ÆÒÆÒ 10ï¿½Ã°ï¿½
							if(Flashdatarec.e2p_opertime_set < 99)
								Flashdatarec.e2p_opertime_set++;
							break;
						default:
							break;
					}
					
					if(!key_continued[UP_KEY])
					{
						Play_Buzzer(1);						// Button
					}
				}
				else
				{
					if(!key_continued[UP_KEY])
					{
						Play_Buzzer(0);						// Not
					}
				}	
			}
		}
	}
	//-----------------------------------------------------------------------------------------------
	else											// ÆÒÆÒ¹è±â?
	{
		if(menu_key[RUNSTOP_KEY])      			// ÆÒÆÒ
		{
			Stop_Proc();
			Play_Buzzer(2);						// Power Off
		}
		else if((menu_key[UP_KEY] || key_continued[UP_KEY]) && (key_pushed[DOWN_KEY] == 0) &&
				(disp_mode != DISP_HEATER_TEMP) && (disp_mode != DISP_HUMIDITY) && (error_code == 0))				// Up Å°
		{
			if(key_continued[UP_KEY])
			{
				if(flg_key_start == 0)
				{
					flg_key_start = 1;
					if(keycont_timeout == 0)
						keycont_timeout = 50;		// 500ms
				}
			}
			
			if(keycont_timeout == 0)
			{
				keycont_timeout = 20;
				Play_Buzzer(1);						// Button
				disp_timeout = 50;					// 5sec
				
				switch(op_kind)
				{
					case AUTO_MODE:					// ï¿½Úµï¿½¹è±â?
						if(disp_mode == DISP_TEMPSET)
						{
							if(Flashdatarec.e2p_auto_temp < 80)
							{
								Flashdatarec.e2p_auto_temp++;
								e2p_save_time = 20;		// 2sec
							}
						}
						else
						{
							prev_dispmode = disp_mode;
							disp_mode = DISP_TEMPSET;
						}
						break;
					case STER_MODE:					// ÆÒÕ¸ÆÒ
						if(disp_mode == DISP_TEMPSET)
						{
							if(Flashdatarec.e2p_ster_temp < 80)
							{
								Flashdatarec.e2p_ster_temp++;
								e2p_save_time = 20;		// 2sec
							}
						}
						else
						{
							prev_dispmode = disp_mode;
							disp_mode = DISP_TEMPSET;
						}
						break;
					default:
						break;
				}
			}
		}
		else if((menu_key[DOWN_KEY] || key_continued[DOWN_KEY]) && (key_pushed[UP_KEY] == 0) && 
				(disp_mode != DISP_HEATER_TEMP) && (disp_mode != DISP_HUMIDITY) && (error_code == 0))	// Down Å°
		{
			if(key_continued[DOWN_KEY])
			{
				if(flg_key_start == 0)
				{
					flg_key_start = 1;
					if(keycont_timeout == 0)
						keycont_timeout = 50;		// 500ms
				}
			}
			
			if(keycont_timeout == 0)
			{
				keycont_timeout = 20;
				Play_Buzzer(1);						// Button
				disp_timeout = 50;					// 5sec
				
				switch(op_kind)
				{
					case AUTO_MODE:					// ï¿½Úµï¿½¹è±â?
						if(disp_mode == DISP_TEMPSET)
						{
							if(Flashdatarec.e2p_auto_temp > 60)
							{
								Flashdatarec.e2p_auto_temp--;
								e2p_save_time = 20;		// 2sec
							}
						}
						else
						{
							prev_dispmode = disp_mode;
							disp_mode = DISP_TEMPSET;
						}
						break;
					case STER_MODE:					// ÆÒÕ¸ÆÒ
						if(disp_mode == DISP_TEMPSET)
						{
							if(Flashdatarec.e2p_ster_temp > 60)
							{
								Flashdatarec.e2p_ster_temp--;
								e2p_save_time = 20;		// 2sec
							}
						}
						else
						{
							prev_dispmode = disp_mode;
							disp_mode = DISP_TEMPSET;
						}
						break;
					default:
						break;
				}
			}
		}
		else if(menu_key[AUTO_KEY] && (disp_mode != DISP_RESERVE) && (error_code == 0))					// ï¿½Úµï¿½ Å°
		{
			if(op_kind != AUTO_MODE)
			{
				op_kind = AUTO_MODE;
				oper_step = 0;
				Autoled_on();
				Play_Buzzer(1);
			}
			else
				Play_Buzzer(0);
		}
		else if((menu_key[STER_KEY]) && (disp_mode != DISP_RESERVE) && (error_code == 0))					// ¹è±â?
		{
			if(op_kind != STER_MODE)
			{
				op_kind = STER_MODE;
				ster_run_timeout = Flashdatarec.e2p_ster_time * 60;	// ï¿½Ã°ï¿½
				oper_step = 0;
				Sterled_on();
				Play_Buzzer(1);
			}
			else
				Play_Buzzer(0);
		}
		else if((menu_key[SMOKE_KEY]) && (disp_mode != DISP_RESERVE) && (error_code == 0))					// ÆÒ ï¿½Ð»ï¿½
		{
			if((op_kind == AUTO_MODE) || (op_kind == STER_MODE))
			{
				flg_smoke_select = !flg_smoke_select;
				if(flg_smoke_select)
				{
					cool_step = 0;												// ï¿½Ã°ï¿½ Ã³ÆÒÆÒ ï¿½Ñ´ï¿½.
					smoke_step = 0;
					Led_data_set(3, LEDC_SMOKE, 1);				// +ÆÒ
				}
				else
					Led_data_set(3, LEDC_SMOKE, 0);				// +ÆÒ

				Play_Buzzer(1);						// Button
			}
			else
				Play_Buzzer(0);
		}
	}


KEY_CLEAR:
    if((!key_pushed[UP_KEY]) && (!key_pushed[DOWN_KEY]) && (!key_continued[AUTO_KEY]) && 
		(!key_continued[RESERVE_KEY]) && (!key_continued[STER_KEY]) && (!key_continued[SMOKE_KEY]) && (!key_continued[RUNSTOP_KEY]))
	{
		flg_key_start = 0;
	}

    if((!key_pushed[AUTO_KEY]) && (!key_pushed[RESERVE_KEY]))
	{
        flg_option_start = 0;
	}
	
    menu_key[RUNSTOP_KEY] = 0;
    menu_key[UP_KEY] = 0;
    menu_key[DOWN_KEY] = 0;
    menu_key[AUTO_KEY] = 0;
    menu_key[RESERVE_KEY] = 0;
    menu_key[STER_KEY] = 0;
    menu_key[SMOKE_KEY] = 0;
}
//--------------------------------------------------------------------------------------------------------------------------------
// ï¿½Ô·ï¿½ Check Ã³ÆÒ
void InputCheck_Proc(void)
{
	//----------------------------------------------------   TEMPER OPEN
	if(COMM_DOOR_SW)					
	{
		if(flg_temperchk == 0)
		{
			temper_timeout = 5;								// 500ms
			flg_temperchk = 1;
		}
		else
		{
			if(temper_timeout == 0)
				temper_open_error = 1;
		}
	}
	else
	{
		temper_timeout = 0;
		flg_temperchk = 0;
		temper_open_error = 0;
	}
	//----------------------------------------------------- // Ç³ÆÒ ÆÒÆÒÄ¡
	if(!COMM_AIRPRESURE_SW)						
	{
		if(flg_airpresurechk == 0)
		{
			airpresure_timeout = 5;							// 500ms
			flg_airpresurechk = 1;
		}
		else
		{
			if(airpresure_timeout == 0)
				flg_airpresure_in = 1;
		}
	}
	else
	{
		flg_airpresurechk = 0;
		flg_airpresure_in = 0;
	}
}

//------------------------------------------------------------------------------------
// Ãâ·Â Ã³¸®
void Output_Proc(void)
{
	if(flg_smoke_select == 0)
	{
		flg_smoke_heater_out = 0;
		flg_smoke_pump_out = 0;
	}
	
	if(temper_open_error == 0)
	{
		if(flg_circula_fan_backup)
		{
			flg_circula_fan_backup = 0;
			circula_fan_speed_level = circula_fan_speed_backup;
		}

		COMM_SPEED_CIRCULA = circula_fan_speed_level;
		COMM_SPEED_VENTILATION = ventilation_fan_speed_level;

		COMM_CIRFAN_ON = flg_cirfan_out;				// ¼øÈ¯ ÆÒ
		COMM_VENTILFAN_ON = flg_ventilfan_out;			// ¹è±â? ÆÒ

		COMM_HEATER_ON = flg_heater_out;				// ÆÒÆÒ

		COMM_DUCT = flg_duct_out;					// ´öÆ® Open Close
		COMM_UVLAMP_ON = flg_uvlamp_out;
		
		COMM_SMOKE_HEATER_ON = flg_smoke_heater_out;		// SMOKE HEATER
		COMM_SMOKE_PUMP_ON = flg_smoke_pump_out;			// SMOKE PUMP
		
		COMM_AC220_SPARE1 = 0;
		flg_first_door_open = 1;
	}
	else
	{
		if((op_mode == RUN_MODE) && ((op_kind == AUTO_MODE) || (op_kind == STER_MODE) || flg_smoke_select))
		{
			if(flg_first_door_open)
			{
				flg_first_door_open = 0; 
				circula_fan_off_timeout = 50;	// 5 ÃÊ
				flg_circula_fan_backup  = 1;
				circula_fan_speed_backup = circula_fan_speed_level;
				circula_fan_speed_level = 3;
				COMM_CIRFAN_ON = 1;				// ¼øÈ¯ ÆÒ
			}
		}
		
		COMM_VENTILFAN_ON = 0;			//¹è±â ÆÒ
		COMM_HEATER_ON = 0;				// È÷ÅÍ
		COMM_DUCT = 0;					// ´öÆ® Open Close
		COMM_UVLAMP_ON = 0;
		COMM_AC220_SPARE1 = 0;

		COMM_SMOKE_HEATER_ON = 0;		// SMOKE HEATER
		COMM_SMOKE_PUMP_ON = 0;			// SMOKE PUMP
		
		// ¿¬¹« ¿îÀüÁß ¹®¿­¸² ¹ß»ý½Ã ¿¬¹«µ¿ÀÛÀ» ¿Ï·áÇÑ°ÍÀ¸·Î ÆÇ´ÜÇÑ´Ù.
		if((op_mode == RUN_MODE) && (op_kind == IDLE_MODE) && flg_smoke_select)
		{
			flg_smoke_select = 0;
			flg_cirfan_out = 0;
			smoke_step = 100;
			flg_coolled_amulating = 0;
		}
		
		// ¿îÀüÁß µµ¾î Open ½Ã ½Ã°£ Clear
		if((op_mode == RUN_MODE) && ((op_kind == AUTO_MODE) || (op_kind == MANUAL_MODE) || (op_kind == STER_MODE)))
		{
			switch(op_kind)
			{
				case AUTO_MODE:
					// È÷ÅÍ °¡µ¿½Ã°£ Clear
					oper_total_timeout = 3600 * 2;	// ÃÑ °¡µ¿ Á¦ÇÑ ½Ã°£
					break;
				case STER_MODE:
						// ¼³Á¤½Ã°£ Clear
					if(oper_step == 2)
					{
						if(flg_check_ster_first == 0)
						{
							ster_run_timeout = Flashdatarec.e2p_ster_time * 60;	// 15ºÐ 
						}
					}
					break;
				default:
					break;
			}
		}
	}
	
	if(temper_open_error)				// ¹®ÀÌ ¿­¸®¸é LED LAMP¸¦ ÄÑÁØ´Ù
	{
		COMM_LEDLAMP_ON = 1;
	}
	else
	{
		COMM_LEDLAMP_ON = 0;
	}
}
//----------------------------------------------------------------------------------
void Disp_Coollingled(void)
{
	if((flg_coolled_amulating) && (disp_mode != DISP_RESERVE) && (temper_open_error == 0) 
		&& (disp_mode != DISP_HEATER_TEMP) && (disp_mode != DISP_HUMIDITY) && (error_code == 0))
	{
		if(flg_cwd == 0)						// CWD ÆÒÆÒ
		{
			if(coolled_timeout == 0)
			{
				coolled_timeout = 20;			// 300ms
				switch(coolled_step)
				{
					case 0:
						leddata[4] &= 0x80;
						leddata[4] |= 0x01;
						leddata[6] &= 0x80;
						leddata[6] |= 0x01;
						break;
					case 1:
						leddata[4] &= 0x80;
						leddata[4] |= 0x02;
						leddata[6] &= 0x80;
						leddata[6] |= 0x20;
						break;
					case 2:
						leddata[4] &= 0x80;
						leddata[4] |= 0x04;
						leddata[6] &= 0x80;
						leddata[6] |= 0x10;
						break;
					case 3:
						leddata[4] &= 0x80;
						leddata[4] |= 0x08;
						leddata[6] &= 0x80;
						leddata[6] |= 0x08;
						break;
					case 4:
						leddata[4] &= 0x80;
						leddata[4] |= 0x10;
						leddata[6] &= 0x80;
						leddata[6] |= 0x04;
						break;
					case 5:
						leddata[4] &= 0x80;
						leddata[4] |= 0x20;
						leddata[6] &= 0x80;
						leddata[6] |= 0x02;
						break;
					
					default:
						break;
				}
				
				coolled_step++;
				if(coolled_step > 5)
					coolled_step = 0;
			}
		}
		else											// CCWD				¼øÈ¯ÆÒ
		{
			if(coolled_timeout == 0)
			{
				coolled_timeout = 20;			// 300ms
				switch(coolled_step)
				{
					case 0:
						leddata[4] &= 0x80;
						leddata[4] |= 0x01;
						leddata[6] &= 0x80;
						leddata[6] |= 0x01;
						break;
					case 1:
						leddata[4] &= 0x80;
						leddata[4] |= 0x20;
						leddata[6] &= 0x80;
						leddata[6] |= 0x02;
						break;
					case 2:
						leddata[4] &= 0x80;
						leddata[4] |= 0x10;
						leddata[6] &= 0x80;
						leddata[6] |= 0x04;
						break;
					case 3:
						leddata[4] &= 0x80;
						leddata[4] |= 0x08;
						leddata[6] &= 0x80;
						leddata[6] |= 0x08;
						break;
					case 4:
						leddata[4] &= 0x80;
						leddata[4] |= 0x04;
						leddata[6] &= 0x80;
						leddata[6] |= 0x10;
						break;
					case 5:
						leddata[4] &= 0x80;
						leddata[4] |= 0x02;
						leddata[6] &= 0x80;
						leddata[6] |= 0x20;
						break;
					
					default:
						break;
				}
				
				coolled_step++;
				if(coolled_step > 5)
					coolled_step = 0;
			}
		}
	}
}
//------------------------------------------------------------------------------------
void Error_Recovery_Proc(void)
{
	if(error_code & EB_ERR_SET)				// RS485
	{
		if(flg_rs485_error == 0)
		{
			error_code &= ~EB_ERR_SET;
			Buzzer_Stop();
		}
	}
}
//--------------------------------------------------------------------------------------
// 7 Segment & LED Display
void Disp_Segment(void)
{
	uint8_t temp;
	
	if(disp_mode == DISP_OPTION)
	{
		Disp_Option();
		return;
	}
	
	if(error_code > 0)                             // Error Ç¥ÆÒ
	{
		Disp_ErrCode();
	}
	else
	{
		if(temper_open_error)						// ÆÒÆÒ ÆÒÆÒÆÒ
		{
			if(flg_toggle_500ms)
			{
				if(flg_toggle_500ms != disp_toggle)
				{
					disp_toggle = flg_toggle_500ms;
				
					Seg_data_set(0, 0, 0);					// '0'
					Seg_data_set(2, 26, 0);					// 'P'
					Seg_data_set(4, 14, 0);					// 'E'
					Seg_data_set(6, 17, 0);					// 'h'
				}
			}
			else
			{
				if(flg_toggle_500ms != disp_toggle)
				{
					disp_toggle = flg_toggle_500ms;
				
					Seg_data_set(0, 18, 0);
					Seg_data_set(2, 18, 0);
					Seg_data_set(4, 18, 0);
					Seg_data_set(6, 18, 0);
				}
			}

			return;
		}
		
		if(disp_mode != DISP_RESERVE)					// ÆÒÆÒ ÆÒÆÒÆÒÆÒ ï¿½Æ´Ï¶ï¿½ï¿?		
		{
			if(flg_reserve)								// ÆÒÆÒÆÒ ï¿½Ö´Ù¸ï¿½
				Led_data_set(1, LEDC_RESERVE, 1);		// ÆÒÆÒ ÆÒÆÒ
			else 
				Led_data_set(1, LEDC_RESERVE, 0);		// ÆÒÆÒ ï¿½Òµï¿½
		}
		
		switch(disp_mode)
		{
			case DISP_FILTER:								// ÇÊÅÍ±³È¯ ¸Þ½ÃÁö Ãâ·ÂDISP_FILTER
				if(flg_toggle_5sec)
				{
					if(flg_toggle_5sec != disp_toggle)
					{
						disp_toggle = flg_toggle_5sec;
					
						Seg_data_set(0, 15, 0);					// 'F'
						Seg_data_set(2, 22, 0);					// 'i'
						Seg_data_set(4, 12, 0);					// 'C'
						Seg_data_set(6, 19, 0);					// 'L'
					}
				}
				else
				{
					if(flg_toggle_5sec != disp_toggle)
					{
						disp_toggle = flg_toggle_5sec;
					
						Seg_data_set(0, 18, 0);
						Seg_data_set(2, 18, 0);
						Seg_data_set(4, 18, 0);
						Seg_data_set(6, 18, 0);
					}
				}
				break;
			case DISP_DOOROPEN:								// ÆÒÆÒÆÒ ÆÒÆÒ
				break;
			case DISP_TEMPTIME:								// ÆÒÆÒ ï¿½Âµï¿½ ÆÒ ÆÒ¹è±â½Ã°ï¿½ Ç¥ÆÒ
				Disp_Temp_Seg(COMM_EXT_TEMP);
				if((flg_coolled_amulating == 0) && (op_kind != IDLE_MODE))
				{
					if(oper_remain_time < 60)
						oper_remain_time = 60;
					
					Disp_Time_Seg(oper_remain_time / 60);
				}
				break;
			case DISP_TEMPSET:                       		// ÆÒÆÒ
				switch(op_kind)
				{
					case AUTO_MODE:							// ï¿½Úµï¿½ÆÒÆÒ
						if(flg_toggle_500ms)
						{
							if(flg_toggle_500ms != disp_toggle)
							{
								disp_toggle = flg_toggle_500ms;
							
								Disp_Temp_Seg(Flashdatarec.e2p_auto_temp);
							}
						}
						else
						{
							if(flg_toggle_500ms != disp_toggle)
							{
								disp_toggle = flg_toggle_500ms;
							
								Seg_data_set(0, 18, 0);
								Seg_data_set(2, 18, 0);
							}
						}
						break;
					case STER_MODE:							// ÆÒÕ¸ÆÒ
						if(flg_toggle_500ms)
						{
							if(flg_toggle_500ms != disp_toggle)
							{
								disp_toggle = flg_toggle_500ms;
							
								Disp_Temp_Seg(Flashdatarec.e2p_ster_temp);
							}
						}
						else
						{
							if(flg_toggle_500ms != disp_toggle)
							{
								disp_toggle = flg_toggle_500ms;
							
								Seg_data_set(0, 18, 0);
								Seg_data_set(2, 18, 0);
							}
						}
						break;
					default:
						break;
				}
				break;
			//-------------------------------------------------------------------
			case DISP_HEATER_TEMP:
				Disp_HighTemp_Seg();
				break;
			case DISP_HUMIDITY:
				Disp_Humi_Seg(COMM_EXT_HUMI);
				break;
			case DISP_RESERVE:								// ÆÒÆÒ ÆÒÆÒ ¹è±â?
				if(flg_toggle_500ms)
				{
					if(flg_toggle_500ms != disp_toggle)
					{
						disp_toggle = flg_toggle_500ms;
						
						Led_data_set(1, LEDC_RESERVE, 1);
						if(disp_subnum == 0)					// ï¿½Ã°ï¿½
						{
							temp = reserve_hour / 10;
							Seg_data_set(0, temp, 0);
							temp = reserve_hour % 10;
							Seg_data_set(2, temp, 0);
						}
						else
						{
							temp = reserve_minute / 10;
							Seg_data_set(4, temp, 0);
							temp = reserve_minute % 10;
							Seg_data_set(6, temp, 0);
						}
					}
				}
				else
				{
					if(flg_toggle_500ms != disp_toggle)
					{
						disp_toggle = flg_toggle_500ms;
						
						Led_data_set(1, LEDC_RESERVE, 0);
						if(disp_subnum == 0)					// ï¿½Ã°ï¿½
						{
							Seg_data_set(0, 18, 0);
							Seg_data_set(2, 18, 0);

							temp = reserve_minute / 10;
							Seg_data_set(4, temp, 0);
							temp = reserve_minute % 10;
							Seg_data_set(6, temp, 0);
						}
						else
						{
							Seg_data_set(4, 18, 0);
							Seg_data_set(6, 18, 0);

							temp = reserve_hour / 10;
							Seg_data_set(0, temp, 0);
							temp = reserve_hour % 10;
							Seg_data_set(2, temp, 0);
						}
					}
				}	
				break;
			case DISP_WAIT:
				Waitled_on();
				break;
			default:
				break;
		}
	}
}
//---------------------------------------------------------------------------------------------------------
void Waitled_on(void)
{
	Seg_data_set(0, 18, 0);
	Seg_data_set(2, 18, 0);
	Seg_data_set(4, 18, 0);
	Seg_data_set(6, 18, 0);

	Led_data_set(0, LEDC_AUTO, 0);				// ï¿½Úµï¿½
	Led_data_set(1, LEDC_DRY, 0);				// ÆÒÆÒ
	Led_data_set(1, LEDC_TEMP, 0);				// ï¿½Âµï¿½
	Led_data_set(1, LEDC_TIME, 0);				// ï¿½Ã°ï¿½

	Led_data_set(2, LEDC_STABIL, 0);			// ¹è±â?
	Led_data_set(3, LEDC_DEODRAN, 0);			// Å»ÆÒ
	Led_data_set(3, LEDC_SMOKE, 0);				// +ÆÒ
	Led_data_set(3, LEDC_MINUTE, 0);			// ÆÒ
	Led_data_set(3, LEDC_TIMEREMAIN, 0);		// ÆÒ¹è±â½Ã°ï¿½
	Led_data_set(7, LEDC_TIMEREMAIN2, 0);		// ÆÒ¹è±â½Ã°ï¿½

	Led_data_set(4, LEDT_POWER, 0);				// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_AUTO, 1);				// ÆÒÄ¡ ï¿½Úµï¿½
	Led_data_set(5, LEDT_RESERVE, 1);			// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_STABIL, 1);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(7, LEDT_STABIL2, 1);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(5, LEDT_SMOKE, 1);				// ÆÒÄ¡ +ÆÒ

	Led_data_set(6, LEDT_UP, 0);				// ÆÒÄ¡ ï¿½Ã¸ï¿½
	Led_data_set(7, LEDT_DOWN, 0);				// ÆÒÄ¡ ÆÒÆÒ
	
	Led_data_set(8, LEDI_POWER, 0);				// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_AUTO, 1);				// ÆÒÆÒÆÒ ï¿½Úµï¿½
	Led_data_set(8, LEDI_RESERVE, 1);			// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_STABIL, 1);			// ÆÒÆÒÆÒ ¹è±â?
	Led_data_set(8, LEDI_SMOKE, 1);				// ÆÒÆÒÆÒ +ÆÒ
	Led_data_set(8, LEDI_UP, 0);				// ÆÒÆÒÆÒ ï¿½Ã¸ï¿½
	Led_data_set(8, LEDL_DOWN, 0);				// ÆÒÆÒÆÒ ÆÒÆÒ
}
//---------------------------------------------------------------------------------------------------------
void Optionled_on(void)
{
	Led_data_set(0, LEDC_AUTO, 0);				// ï¿½Úµï¿½
	Led_data_set(1, LEDC_DRY, 0);				// ÆÒÆÒ
	Led_data_set(1, LEDC_TEMP, 0);				// ï¿½Âµï¿½
	Led_data_set(1, LEDC_TIME, 0);				// ï¿½Ã°ï¿½

	Led_data_set(2, LEDC_STABIL, 0);			// ¹è±â?
	Led_data_set(3, LEDC_DEODRAN, 0);			// Å»ÆÒ
	Led_data_set(3, LEDC_MINUTE, 0);			// ÆÒ
	Led_data_set(3, LEDC_TIMEREMAIN, 0);		// ÆÒ¹è±â½Ã°ï¿½
	Led_data_set(7, LEDC_TIMEREMAIN2, 0);		// ÆÒ¹è±â½Ã°ï¿½
	
	Led_data_set(4, LEDT_POWER, 1);				// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_AUTO, 1);				// ÆÒÄ¡ ï¿½Úµï¿½
	Led_data_set(5, LEDT_RESERVE, 1);			// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_STABIL, 0);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(7, LEDT_STABIL2, 0);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ

	Led_data_set(6, LEDT_UP, 1);				// ÆÒÄ¡ ï¿½Ã¸ï¿½
	Led_data_set(7, LEDT_DOWN, 1);				// ÆÒÄ¡ ÆÒÆÒ
	
	Led_data_set(8, LEDI_POWER, 1);				// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_AUTO, 1);				// ÆÒÆÒÆÒ ï¿½Úµï¿½
	Led_data_set(8, LEDI_RESERVE, 1);			// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_STABIL, 0);			// ÆÒÆÒÆÒ ¹è±â?
	Led_data_set(8, LEDI_SMOKE, 0);				// ÆÒÆÒÆÒ +ÆÒ
	Led_data_set(8, LEDI_UP, 1);				// ÆÒÆÒÆÒ ï¿½Ã¸ï¿½
	Led_data_set(8, LEDL_DOWN, 1);				// ÆÒÆÒÆÒ ÆÒÆÒ
}
//---------------------------------------------------------------------------------------------------------
void Autoled_on(void)
{
	Led_data_set(0, LEDC_AUTO, 1);				// ï¿½Úµï¿½
	Led_data_set(1, LEDC_DRY, 1);				// ÆÒÆÒ
	Led_data_set(1, LEDC_TEMP, 1);				// ï¿½Âµï¿½
	Led_data_set(1, LEDC_TIME, 0);				// ï¿½Ã°ï¿½

	Led_data_set(2, LEDC_STABIL, 1);			// ¹è±â?
	Led_data_set(3, LEDC_DEODRAN, 1);			// Å»ÆÒ
	Led_data_set(3, LEDC_MINUTE, 1);			// ÆÒ
	Led_data_set(3, LEDC_TIMEREMAIN, 1);		// ÆÒ¹è±â½Ã°ï¿½
	Led_data_set(7, LEDC_TIMEREMAIN2, 1);		// ÆÒ¹è±â½Ã°ï¿½

	Led_data_set(4, LEDT_POWER, 1);				// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_AUTO, 1);				// ÆÒÄ¡ ï¿½Úµï¿½
	Led_data_set(5, LEDT_RESERVE, 1);			// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_STABIL, 1);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(7, LEDT_STABIL2, 1);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(5, LEDT_SMOKE, 1);				// ÆÒÄ¡ +ÆÒ

	Led_data_set(6, LEDT_UP, 1);				// ÆÒÄ¡ ï¿½Ã¸ï¿½
	Led_data_set(7, LEDT_DOWN, 1);				// ÆÒÄ¡ ÆÒÆÒ
	
	Led_data_set(8, LEDI_POWER, 1);				// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_AUTO, 1);				// ÆÒÆÒÆÒ ï¿½Úµï¿½
	Led_data_set(8, LEDI_RESERVE, 1);			// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_STABIL, 1);			// ÆÒÆÒÆÒ ¹è±â?
	Led_data_set(8, LEDI_SMOKE, 1);				// ÆÒÆÒÆÒ +ÆÒ
	Led_data_set(8, LEDI_UP, 1);				// ÆÒÆÒÆÒ ï¿½Ã¸ï¿½
	Led_data_set(8, LEDL_DOWN, 1);				// ÆÒÆÒÆÒ ÆÒÆÒ
}
//---------------------------------------------------------------------------------------------------------
void Sterled_on(void)
{
	Led_data_set(0, LEDC_AUTO, 0);				// ï¿½Úµï¿½
	Led_data_set(1, LEDC_DRY, 1);				// ÆÒÆÒ
	Led_data_set(1, LEDC_TEMP, 1);				// ï¿½Âµï¿½
	Led_data_set(1, LEDC_TIME, 0);				// ï¿½Ã°ï¿½

	Led_data_set(2, LEDC_STABIL, 1);			// ¹è±â?
	Led_data_set(3, LEDC_DEODRAN, 1);			// Å»ÆÒ
	Led_data_set(3, LEDC_MINUTE, 1);			// ÆÒ
	Led_data_set(3, LEDC_TIMEREMAIN, 1);		// ÆÒ¹è±â½Ã°ï¿½
	Led_data_set(7, LEDC_TIMEREMAIN2, 1);		// ÆÒ¹è±â½Ã°ï¿½
	
	Led_data_set(4, LEDT_POWER, 1);				// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_AUTO, 1);				// ÆÒÄ¡ ï¿½Úµï¿½
	Led_data_set(5, LEDT_RESERVE, 1);			// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_STABIL, 1);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(7, LEDT_STABIL2, 1);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(5, LEDT_SMOKE, 1);				// ÆÒÄ¡ +ÆÒ
	
	Led_data_set(6, LEDT_UP, 1);				// ÆÒÄ¡ ï¿½Ã¸ï¿½
	Led_data_set(7, LEDT_DOWN, 1);				// ÆÒÄ¡ ÆÒÆÒ
	
	Led_data_set(8, LEDI_POWER, 1);				// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_AUTO, 1);				// ÆÒÆÒÆÒ ï¿½Úµï¿½
	Led_data_set(8, LEDI_RESERVE, 1);			// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_STABIL, 1);			// ÆÒÆÒÆÒ ¹è±â?
	Led_data_set(8, LEDI_SMOKE, 1);				// ÆÒÆÒÆÒ +ÆÒ
	Led_data_set(8, LEDI_UP, 1);				// ÆÒÆÒÆÒ ï¿½Ã¸ï¿½
	Led_data_set(8, LEDL_DOWN, 1);				// ÆÒÆÒÆÒ ÆÒÆÒ
}
//---------------------------------------------------------------------------------------------------------
void Smokeled_on(void)
{
	Led_data_set(0, LEDC_AUTO, 0);				// ï¿½Úµï¿½
	Led_data_set(1, LEDC_DRY, 0);				// ÆÒÆÒ
	Led_data_set(1, LEDC_TEMP, 1);				// ï¿½Âµï¿½
	Led_data_set(1, LEDC_TIME, 0);				// ï¿½Ã°ï¿½

	Led_data_set(2, LEDC_STABIL, 0);			// ¹è±â?
	Led_data_set(3, LEDC_DEODRAN, 0);			// Å»ÆÒ
	Led_data_set(3, LEDC_MINUTE, 0);			// ÆÒ
	Led_data_set(3, LEDC_TIMEREMAIN, 0);		// ÆÒ¹è±â½Ã°ï¿½
	Led_data_set(7, LEDC_TIMEREMAIN2, 1);		// ÆÒ¹è±â½Ã°ï¿½

	Led_data_set(4, LEDT_POWER, 1);				// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_AUTO, 1);				// ÆÒÄ¡ ï¿½Úµï¿½
	Led_data_set(5, LEDT_RESERVE, 1);			// ÆÒÄ¡ ÆÒÆÒ
	Led_data_set(5, LEDT_STABIL, 1);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(7, LEDT_STABIL2, 1);			// ÆÒÄ¡ ¹è±â?/Å»ÆÒ
	Led_data_set(5, LEDT_SMOKE, 1);				// ÆÒÄ¡ +ÆÒ

	Led_data_set(6, LEDT_UP, 0);				// ÆÒÄ¡ ï¿½Ã¸ï¿½
	Led_data_set(7, LEDT_DOWN, 0);				// ÆÒÄ¡ ÆÒÆÒ
	
	Led_data_set(8, LEDI_POWER, 1);				// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_AUTO, 1);				// ÆÒÆÒÆÒ ï¿½Úµï¿½
	Led_data_set(8, LEDI_RESERVE, 1);			// ÆÒÆÒÆÒ ÆÒÆÒ
	Led_data_set(8, LEDI_STABIL, 1);			// ÆÒÆÒÆÒ ¹è±â?
	Led_data_set(8, LEDI_SMOKE, 1);				// ÆÒÆÒÆÒ +ÆÒ
	Led_data_set(8, LEDI_UP, 0);				// ÆÒÆÒÆÒ ï¿½Ã¸ï¿½
	Led_data_set(8, LEDL_DOWN, 0);				// ÆÒÆÒÆÒ ÆÒÆÒ
}
//---------------------------------------------------------------------------------------------------------
void Disp_Option(void)
{
	switch(opt_num)
	{
		case 1:										// ¼øÈ¯ ï¿½Ò·ï¿½ Ã¼Å©
			Seg_data_set(0, 12, 0);           		// 'C' Ç¥ÆÒ
			Seg_data_set(2, 14, 0);           		// 'E' Ç¥ÆÒ
		
			Seg_data_set(4, (Flashdatarec.e2p_cirfan_diff_temp / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_cirfan_diff_temp % 10), 0);
			break;
		case 2:										// ÆÒÆÒ ï¿½Ò·ï¿½ Ã¼Å©
			Seg_data_set(0, 21, 0);           		// 'H' Ç¥ÆÒ
			Seg_data_set(2, 14, 0);           		// 'E' Ç¥ÆÒ
		
			Seg_data_set(4, (Flashdatarec.e2p_heater_diff_temp / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_heater_diff_temp % 10), 0);
			break;
		case 3:										// ÆÒÆÒÆÒ ÆÒÆÒ Ã¼Å©(ÆÒ¹è±â½Ò·ï¿½)
			Seg_data_set(0, 5, 0);           		// 'S' Ç¥ÆÒ
			Seg_data_set(2, 14, 0);           		// 'E' Ç¥ÆÒ
		
			Seg_data_set(4, (Flashdatarec.e2p_smokbimetal_time / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_smokbimetal_time % 10), 0);
			break;
		case 4:										// È¯ÆÒÒ·ï¿? Ã¼Å©
			Seg_data_set(0, 0, 0);           		// 'O' Ç¥ÆÒ
			Seg_data_set(2, 14, 0);           		// 'E' Ç¥ÆÒ
		
			Seg_data_set(4, (Flashdatarec.e2p_heater_oper_percent / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_heater_oper_percent % 10), 0);
			break;
		case 5:										// ÆÒÆÒÆÒÆÒ ï¿½Ò·ï¿½ N% ÆÒÆÒ
			Seg_data_set(0, 29, 0);           		// 'u' Ç¥ÆÒ
			Seg_data_set(2, 21, 0);           		// 'H' Ç¥ÆÒ
		
			Seg_data_set(4, (Flashdatarec.e2p_under_err_humi / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_under_err_humi % 10), 0);
			break;
		case 6:										// ÆÒÆÒÆÒÆÒ ï¿½Ò·ï¿½ N% ï¿½Ì»ï¿½
			Seg_data_set(0, 20, 0);           		// 'o' Ç¥ÆÒ
			Seg_data_set(2, 21, 0);           		// 'H' Ç¥ÆÒ
		
			Seg_data_set(4, (Flashdatarec.e2p_over_err_humi / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_over_err_humi % 10), 0);
			break;
		case 7:										// ÆÒÆÒ Ã»ÆÒ/¼øÈ¯
			Seg_data_set(0, 15, 0);           		// 'F' Ç¥ÆÒ
			Seg_data_set(2, 24, 0);           		// 't' Ç¥ÆÒ
		
			Seg_data_set(4, (Flashdatarec.e2p_opertime_set / 10), 0);
			Seg_data_set(6 , (Flashdatarec.e2p_opertime_set % 10), 0);
			break;
		default:
			break;
	}
}
//---------------------------------------------------------------------------------------------------------
void Disp_ErrCode(void)
{
	uint8_t i;
	
	if(flg_toggle_500ms)
	{
		if(before_status)
		{
			before_status = 0;
			Seg_data_set(0, 14, 0);                   // 'E'
			Seg_data_set(2, 25, 0);                   // 'r'
			
			if(flg_errscan)
			{
				for(i=0; i<16; i++)
				{
					if(error_code & err_mask)
						break;
					else
					{
						if(err_mask == 0x8000)
							err_mask = 1;
						else
							err_mask <<= 1;
					}
				}
			}

			switch(err_mask)
			{
				case 0x0001:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 1, 0);
						break;
				case 0x0002:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 2, 0);
						break;
				case 0x0004:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 3, 0);
						break;
				case 0x0008:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 4, 0);
						break;
				case 0x0010:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 5, 0);
						break;
				case 0x0020:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 6, 0);
						break;
				case 0x0040:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 7, 0);
						break;
				case 0x0080:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 8, 0);
						break;
				case 0x0100:
						Seg_data_set(4, 0, 0);
						Seg_data_set(6, 9, 0);
						break;
				case 0x0200:
						Seg_data_set(4, 1, 0);
						Seg_data_set(6, 0, 0);
						break;
				case 0x0400:
						Seg_data_set(4, 1, 0);
						Seg_data_set(6, 1, 0);
						break;
				default:
						break;
			}

			if(flg_errscan == 0)
			{
				if(err_mask == 0x8000)
					err_mask = 1;
				else
					err_mask <<= 1;
			}

			flg_errscan = !flg_errscan;    // 2È¸ ÆÒ¹è±â½Ì±ï¿½ ÆÒÆÒ
		}
	}
	else
	{
		before_status = 1;
		Seg_data_set(0, 18, 0);                   // ' '
		Seg_data_set(2, 18, 0);
		Seg_data_set(4, 18, 0);                   // ' '
		Seg_data_set(6, 18, 0);
	}

	if(error_timeout == 0)
	{
		if(op_mode == RUN_MODE)
		{
			error_timeout = 60;
			Play_Buzzer(5);        // ÆÒÆÒÆÒ
		}
	}
}
//---------------------------------------------------------------------------------------------------------
// ¼øÈ¯ºÒ·® Ã¼Å©
void Circula_Error_Check(void)
{
	if(flg_cirfan_out && flg_heater_out && (circula_fan_speed_level == 10) 
		&& (op_mode == RUN_MODE) && (temper_open_error == 0))
	{
		// ³»ºÎ¿Âµµ¿Í ÅäÃâ±¸ ¿Âµµ Â÷ÀÌ°¡ 20µµ ÀÌ»ó Â÷ÀÌ°¡ ³ª¸é
		if(COMM_EXT_TEMP <= (COMM_HEATER_TEMP - Flashdatarec.e2p_cirfan_diff_temp))
		{
			if(flg_cirtimeout_first == 0)
			{
				flg_cirtimeout_first = 1;
				cir_err_timeout = 600;					// 30ºÐ  1800 -> 10min 600 timeout //240904
			}
			else
			{
				if(cir_err_timeout == 0)
				{
					error_code |= E1_ERR_SET;
				}
			}
		}
		else
		{
			flg_cirtimeout_first = 0;
		}
	}
	else
	{
		flg_cirtimeout_first = 0;
	}
}
//---------------------------------------------------------------------------------------------------------
// È÷ÅÍºÒ·® Ã¼Å©
void Heater_Error_Check(void)
{
	if(flg_heater_out)
	{
		if(flg_heater_checktimeout_first == 0)
		{
			if((COMM_HEATER_TEMP - prev_heater_temper) < Flashdatarec.e2p_heater_diff_temp)
			{
				if(heater_err_timeout == 0)
					error_code |= E4_ERR_SET;
			}
			else
				flg_heater_checktimeout_first = 1;
		}
	}
	else
	{
		flg_heater_checktimeout_first = 0;
		heater_err_timeout = 1200;
		prev_heater_temper = COMM_HEATER_TEMP;
	}
}
//---------------------------------------------------------------------------------------------------------
// ¿¬¹«±âºÒ·® Ã¼Å©
void Smoke_Error_Check(void)
{
	if(flg_smoke_heater_out)
	{
		if(flg_smokeheater_timeout_first == 0)
		{
			flg_smokeheater_timeout_first = 1;
			smokeheater_err_timeout = 900;					//  5 mins 300 -> 15 mins 240904 
		}
		else
		{
			if(smokeheater_err_timeout == 0)
			{
				if(COMM_SMOKE_HEATEROUT_STATE)				// Smoke Heater BimetalÀÌ ºÙ¾î ÀÖ´Ù¸é
					error_code |= E5_ERR_SET;
			}
		}
	}
	else
		flg_smokeheater_timeout_first = 0;
}
//---------------------------------------------------------------------------------------------------------
// ÆÒÆÒÆÒ ÆÒÆÒ Ã¼Å©
//void Liquid_Error_Check(void)
//{
//	if(flg_smoke_pump_out)
//	{
//		if(flg_liquid_timeout_first == 0)
//		{
//			flg_liquid_timeout_first = 1;
//			liquidout_err_timeout = Flashdatarec.e2p_smokbimetal_time;
//		}
//		else
//		{
//			if(liquidout_err_timeout == 0)
//			{
//				if(COMM_SMOKE_HEATEROUT_STATE)				// Smoke Heater BimetalÆÒ ï¿½Ù¾ï¿½ ï¿½Ö´Ù¸ï¿½
//					error_code |= E4_ERR_SET;
//			}
//		}
//	}
//	else
//		flg_liquid_timeout_first = 0;
//}
//---------------------------------------------------------------------------------------------------------
// È¯±âºÒ·® Ã¼Å©
void Ventilation_Error_Check(void)
{
	if((op_mode == RUN_MODE) && (flg_coolling) && (temper_open_error == 0))
	{
		if((prev_cooling_temper - COMM_EXT_TEMP) < 10)
		{
			if(coolling_timeout == 0)
				error_code |= E7_ERR_SET;
		}
		else
			flg_coolling = 0;
	}
}
//---------------------------------------------------------------------------------------------------------
// ½Ç³»¿Âµµ ¼¾¼­ºÒ·® Ã¼Å©
void Temperature_Error_Check(void)
{
	if((COMM_EXT_TEMP <= -40) || (COMM_EXT_TEMP >= 100))
		error_code |= E8_ERR_SET;
	else
		error_code &= ~E8_ERR_SET;
}
//---------------------------------------------------------------------------------------------------------
// È÷ÅÍ ÅäÃâ¿Âµµ ¼¾¼­ºÒ·® Ã¼Å©
void HeaterTemper_Error_Check(void)
{
	if((COMM_HEATER_TEMP <= -40) || (COMM_HEATER_TEMP >= 160))    // 110 -> 160 º¯°æ 24³â 8¿ù 9ÀÏ 
		error_code |= E9_ERR_SET;
	else
		error_code &= ~E9_ERR_SET;
}
//---------------------------------------------------------------------------------------------------------
// È÷ÅÍ °ú¿­ ¼¾¼­ºÒ·® Ã¼Å©
void OverHeat_Error_Check(void)
{
	if((COMM_HEATER_TEMP >= 150) && (temper_open_error == 0))   // 
	{
		if(flg_heatertemp_check_first == 0)
		{
			flg_heatertemp_check_first = 1;
			heatertemp_check_timeout = 100;			// 3sec -> 10sec 
		}
		else
		{
			if(heatertemp_check_timeout == 0)
				error_code |= E3_ERR_SET;
		}
	}
	else
	{
		flg_heatertemp_check_first = 0;
		error_code &= ~E3_ERR_SET;
	}
}
//---------------------------------------------------------------------------------------------------------
// ½Àµµ ¼¾¼­ºÒ·® Ã¼Å©
void HumiSensor_Error_Check(void)
{
	if((COMM_EXT_HUMI <= 3) || (COMM_EXT_HUMI > 100))
	{
		if((op_mode == RUN_MODE) && (temper_open_error == 0))
		{
			if(((op_kind == AUTO_MODE) || (op_kind == STER_MODE)) && (flg_hsen_error == 0))
			{
				flg_hsen_error = 1;
				humi_err_timeout = 1800;			
			}
		}
		else
		{
			error_code |= EA_ERR_SET;
			humi_err_timeout = 0;
		}
	}
	else
	{
		error_code &= ~EA_ERR_SET;
		humi_err_timeout = 0;
	}
}
//---------------------------------------------------------------------------------------------------------
// Ç³¾Ð ½ºÀ§Ä¡ºÒ·® Ã¼Å©
void AirPresure_Error_Check(void)   // E2 no user 
{
	if((flg_cirfan_out) && (circula_fan_speed_level == 10))
	{
		if(airpresure_chk_timeout == 0)
		{
			if(flg_airpresure_in == 0)
			{
				if(flg_airpresure_timeout_first == 0)
				{
					flg_airpresure_timeout_first = 1;
					airpresure_err_timeout = 3;
				}
				else
				{
					if(airpresure_err_timeout == 0)
						error_code |= E2_ERR_SET;
				}
			}
			else
				flg_airpresure_timeout_first = 0;
		}
	}
	else
	{
		flg_airpresure_timeout_first = 0;
		airpresure_chk_timeout = 30;			// 3ÆÒ ÆÒ ÆÒÆÒ Ã¼Å©
	}
}
//---------------------------------------------------------------------------------------------------------
void Error_Check_Proc(void)
{
	if(sensor_check_timeout == 0)
	{
		Circula_Error_Check();		// 1. ¼øÈ¯ ºÒ·® Ã¼Å© : Er 01
		//AirPresure_Error_Check();	// 2. Ç³¾Ð½ºÀ§Ä¡ °¨Áö ºÒ·® : Er 02
		OverHeat_Error_Check();		// 3. È÷ÅÍ °ú¿­ Ã¼Å© : Er 03
		// Heater_Error_Check();		// 4. È÷ÅÍ ºÒ·® Ã¼Å© : Er 04
		Smoke_Error_Check();			// 5. ¿¬¹«±âºÒ·® : Er 05
		
		Ventilation_Error_Check();		// 7. È¯±â(¹è±â)ºÒ·® Ã¼Å© : Er 07
		Temperature_Error_Check();		// 8. ½Ç³»¿Âµµ¼¾¼­ ºÒ·® : Er 08
		HeaterTemper_Error_Check();		// 9. ÅäÃâ¿Âµµ¼¾¼­ ºÒ·® : Er 09
		HumiSensor_Error_Check();		// 10. ½Àµµ¼¾¼­ ºÒ·® : Er 10
	}
}
//---------------------------------------------------------------------------------------------------------
// ÆÒÆÒ Ã»ÆÒ ï¿½Ã°ï¿½ Ã¼Å©
void Operated_Time_Check(void)
{
	
}

//---------------------------------------------------------------------------------------------------------
void Disp_HighTemp_Seg(void)
{
	uint16_t	disp_temper;
	
	Seg_data_set(0, 21, 0);				// 'H'
	
	if(COMM_HEATER_TEMP < 0)
	{
		disp_temper = COMM_HEATER_TEMP * -1;
		Seg_data_set(2, 16, 0);				// '-'
		
		if(disp_temper >= 10)
			Seg_data_set(4, disp_temper / 10, 0);				// -1
		else
			Seg_data_set(4, 18, 0);				// ' '
		
		Seg_data_set(6, disp_temper % 10, 0);
	}
	else
	{
		disp_temper = COMM_HEATER_TEMP;
		
		if(disp_temper >= 100)
		{
			Seg_data_set(2, disp_temper / 100, 0);
			Seg_data_set(4, (disp_temper % 100) / 10, 0);
			Seg_data_set(6, disp_temper % 10, 0);
		}
		else
		{
			Seg_data_set(2, 18, 0);				// ' '
			if(disp_temper >= 10)
				Seg_data_set(4, (disp_temper % 100) / 10, 0);
			else
				Seg_data_set(4, 18, 0);				// ' '

			Seg_data_set(6, disp_temper % 10, 0);
		}
	}
}

//---------------------------------------------------------------------------------------------------------
void Disp_Temp_Seg(int8_t current_temp)
{
	if(current_temp < -19)
		current_temp = -19;
	
	if(current_temp < 0)
	{
		current_temp *= -1;
		
		if(current_temp >= 10)
			Seg_data_set(0, 28, 0);				// -1
		else
			Seg_data_set(0, 16, 0);				// '-'
		
		current_temp %= 10;
		Seg_data_set(2, current_temp, 0);
	}
	else
	{
		if(current_temp >= 10)
			Seg_data_set(0, current_temp / 10, 0);				// -1
		else
			Seg_data_set(0, 18, 0);				// ' '
		
		current_temp %= 10;
		Seg_data_set(2, current_temp, 0);
	}
}

void Disp_Humi_Seg(uint8_t current_humi)
{
	Seg_data_set(0, 21, 0);				// 'H'
	Seg_data_set(2, 28, 0);				// 'u'
	
	if(current_humi >= 10)
		Seg_data_set(4, current_humi / 10, 0);				// -1
	else
		Seg_data_set(4, 18, 0);				// ' '
	
	current_humi %= 10;
	Seg_data_set(6, current_humi, 0);
}


void Disp_Time_Seg(uint8_t current_time)
{
	Seg_data_set(4, current_time / 10, 0);
	Seg_data_set(6, current_time % 10, 0);
}

void Disp_Version(void)
{
	Seg_data_set(0, 13, 0);		// 'D'
	Seg_data_set(2, 25, 0);		// 'r'
	Seg_data_set(4, 4, 0);
	Seg_data_set(6, 9, 0);
}

