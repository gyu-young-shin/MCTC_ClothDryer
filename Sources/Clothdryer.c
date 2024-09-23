#include "main.h"
#include "debug.h"
#include "clothdryer.h"
#include "buzzer.h"
#include "segment.h"
#include "flashrom.h"

_Bool		COMM_CIRFAN_ON = 0;				// ��ȯ ��
_Bool		COMM_VENTILFAN_ON = 0;			// ���? ��
_Bool		COMM_HEATER_ON = 0;				// ����
_Bool		COMM_DUCT = 0;					// ��Ʈ Open Close
_Bool		COMM_AC220_SPARE1 = 0;			// AC SPARE
_Bool		COMM_UVLAMP_ON = 0;				// UV LAMP
_Bool		COMM_LEDLAMP_ON = 0;			// LED LAMP
_Bool		COMM_SMOKE_HEATER_ON = 0;		// SMOKE HEATER
_Bool		COMM_SMOKE_PUMP_ON = 0;			// SMOKE PUMP

uint8_t 	COMM_SPEED_CIRCULA = 0;			// ��ȯ�� �ӵ�
uint8_t 	COMM_SPEED_VENTILATION = 0;		// �ҹ��? �ӵ�

_Bool		COMM_DOOR_SW = 0;
_Bool		COMM_AIRPRESURE_SW = 0;			// ǳ�� ����ġ
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
_Bool		flg_first_error = 0;			// ù��° ����


_Bool		flg_cirfan_out = 0;				// ��ȯ ��
_Bool		flg_ventilfan_out = 0;			// ���? ��
_Bool		flg_heater_out = 0;				// ����
_Bool		flg_duct_out = 0;				// ��Ʈ Open Close
_Bool		flg_uvlamp_out = 0;
_Bool		flg_smoke_heater_out = 0;
_Bool		flg_smoke_pump_out = 0;
_Bool		flg_dry_running = 0;
_Bool		flg_smoke_select = 0;		// ��л�? ����
_Bool		flg_goal_temp_ok = 0;		// ��ǥ�µ� ���� ����
_Bool		flg_heater_onoff = 0;		// ���� On Off ����
_Bool		flg_airpresure_in = 0;		// ǳ�н���ġ �Է�
_Bool		flg_hsen_error = 0;			// �������� ����


_Bool		flg_check_humi_start = 0;		// ���� ���� ���� �÷���
_Bool		flg_check_humi_first = 0;		// ���� ���� ���� ����
_Bool		flg_check_ster_first = 0;		// ���? �ҹ��ð� ī��Ʈ �÷���
_Bool		flg_first_door_open = 0;		// ó�� ������ üũ
_Bool		flg_circula_fan_backup = 0;		// ��ȯ�� ���ǵ� ���? �÷���

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
uint16_t	humi_check_timeout = 0;		// ������ ������ 10�� �̻� ���� �ð�
uint8_t		compare_check_humi = 0;		// ���ϱ� ���� ������ ����
uint8_t		humi_remain_time = 0;		// ������ ���� �ð�

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

uint16_t 	cir_err_timeout = 0;		// ��ȯ�ҷ� Timeout
uint16_t 	heater_err_timeout = 0;		// ���ͺҷ� Timeout
uint16_t 	smokeheater_err_timeout = 0;// ������ҷ� Timeout
uint16_t 	liquidout_err_timeout = 0;	// ������ ���� Timeout
uint16_t 	airpresure_err_timeout = 0;	// ǳ�н���ġ �ҷ� Timeout
uint16_t 	airpresure_chk_timeout = 0;	// ǳ�н���ġ Check Timeout
uint16_t 	heater_running_time = 0;	// ���� �ҹ��ð�
uint16_t 	heater_run_total_time = 0;	// ���� �⵿���� ������ ���? �ѽð�
uint16_t 	heatertemp_check_timeout = 0;

int16_t		prev_heater_temper = 0;
uint16_t 	coolling_timeout = 0;
int16_t		prev_cooling_temper = 0;
uint32_t	reserve_clock = 0;			// 24�ð� ���� ����
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
		//----------------------------------------------------------------  ����
		if(error_code == 0)
		{
			flg_first_error = 0;								// ù��° ����
			
			switch(op_kind)										// ���� ���ø��
			{
				case AUTO_MODE:									// �ڵ�����
					Auto_Proc();

					break;
				case STER_MODE:									// ���/Ż����
					Ster_Proc();
					break;
				case IDLE_MODE:									// IDLE ��忡���� +�� ��ư�� �����ִ°� �����Ѵ�.
					Cooling_Proc();								// �ð�
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
			// flg_cirfan_out = 0;				// ��ȯ ��
			// flg_ventilfan_out = 0;			// ���? ��
			// flg_heater_out = 0;				// ����
			// flg_smoke_heater_out = 0;		// ���� ����
			// flg_smoke_pump_out = 0;			// ���� ����
			// flg_duct_out = 0;				// ��Ʈ Close
			// flg_uvlamp_out = 0;
			// Stop_Proc();
			if(flg_first_error == 0)
			{
				flg_first_error = 1;
				cool_step = 0;
			}
			
			Cooling_Proc();					// �ð�
			
			if(coolled_step == 100)
			{
				flg_cirfan_out = 0;				// ��ȯ ��
				flg_ventilfan_out = 0;			// ��� ��
				flg_heater_out = 0;				// ����
				flg_smoke_heater_out = 0;		// ���� ����
				flg_smoke_pump_out = 0;			// ���� ����
				flg_duct_out = 0;				// ��Ʈ Close
				flg_uvlamp_out = 0;
				Stop_Proc();
			}
		}
	}
	else								// 2021-08-25 �ڱ��� ���� ��û ������ ������ �ٷ� ����
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
// �ڵ����� 
void Auto_Proc(void)
{
	uint8_t tmp;
	
	switch(oper_step)
	{
		case 0:
			flg_cirfan_out = 0;				// ��ȯ ��
			ventilation_fan_speed_level = 10;			// 2021-08-30 �ڱ��� ���� ��û���� ���� 10���� ����
			flg_ventilfan_out = 1;			// ��� ��
			flg_heater_out = 0;				// ����
			flg_smoke_heater_out = 0;		// ���� ����
			flg_smoke_pump_out = 0;			// ���� ����
			flg_duct_out = 1;				// ��Ʈ Open Close
			flg_uvlamp_out = 0;
			oper_timeout = 20;				// 20�� �������� ����
			oper_step = 1;
			oper_total_timeout = 7200;		// �� ���� ���� �ð� 2�ð�
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
				if(COMM_EXT_TEMP < 50)		// ���οµ��� 50�� �̸��̸� 
				{
					circula_fan_speed_level = 10;
					flg_cirfan_out = 1;				// ��ȯ ��
					flg_heater_out = 1;				// ����
					flg_duct_out = 0;				// ��Ʈ Open Close
					flg_ventilfan_out = 0;			// ���? ��
					flg_uvlamp_out = 1;
					oper_step = 2;
					flg_cwd = 1;
				}
				else
				{
					oper_step = 3;				// ȯ���? �̵�
				}
			}
			break;
		case 2:
			if(COMM_EXT_TEMP >= 50)									// ���οµ� 50���� ���� �ϸ�
			{
				oper_step = 3;
			}
			else
			{
				if(oper_total_timeout == 0)
					oper_step = 5;
			}
			break;
		case 3:														// 50�� ���� ���� ȯ��
			tmp = Flashdatarec.e2p_auto_temp - 50;						// �ҹ��µ� ���� 50�� ��ŭ 1������ 1�� �߰�
			if((humi_remain_time + tmp) > 99)
				tmp = 0;
			oper_remain_time = (humi_remain_time + tmp) * 60;
		
			flg_coolled_amulating = 0;
			flg_cwd = 0;
			circula_fan_speed_level = 10;
			flg_cirfan_out = 1;										// ��ȯ ��
			flg_heater_out = 1;										// ����
			flg_duct_out = 1;										// ��Ʈ Open Close
			ventilation_fan_speed_level = 5;
			flg_ventilfan_out = 1;									// ���? ��
			flg_uvlamp_out = 1;
		
			cycle_count = 0;
			flg_heater_runrate_first = 0;
			heater_running_time = 0;
			heater_run_total_time = 0;
		
			flg_goal_temp_ok = 0;
			flg_heater_onoff = 1;									// ���� On Off ���� �÷���
			flg_check_humi_first = 1;
			compare_check_humi = COMM_EXT_HUMI;	
			oper_step = 4;
			break;
		case 4:														// �ҹ��µ� ���޽� ���� ����
			if(flg_goal_temp_ok)
			{
				ventilation_fan_speed_level = 6;					// ��ǥ�µ� ���޽� ȯ�� 50%
				
				if((compare_check_humi - COMM_EXT_HUMI) > 2)		// �������� 2% �̻��̶��?
				{
					compare_check_humi = COMM_EXT_HUMI;				// ���� �񱳰��� �ҹ��Ѵ�.
					humi_check_timeout = 600;						// 10��
				}
				
				if(humi_check_timeout == 0)							// �������� ���� 10�� �̻� ������
					oper_step = 5;									// �������� ����
			}
			
//			if(flg_check_humi_first)
//			{
//				if(COMM_EXT_HUMI <= Flashdatarec.e2p_auto_humi)		// �����ҹ��? �������� �ҹ��ϰ��?
//				{
//					flg_goal_temp_ok = 1;							// ��ǥ �µ� ����
//					flg_check_humi_first = 0;
//					humi_check_timeout = 600;						// 10��
//					compare_check_humi = COMM_EXT_HUMI;				// ���� �񱳰��� �ҹ��Ѵ�.
//				}
//			}
			
			
			if(flg_heater_onoff)									// ���� ���ҹ��̸�
			{
				if(Flashdatarec.e2p_auto_temp <= COMM_EXT_TEMP)		// �ҹ��µ�(��ǥ�µ�) �̻��� ���?
				{
					flg_goal_temp_ok = 1;							// ��ǥ �µ� ����
					flg_heater_onoff = 0;							// ���� On Off ���� �÷���
					flg_heater_out = 0;								// ���� ���?
					if(flg_check_humi_first)
					{
						flg_check_humi_first = 0;
						humi_check_timeout = 600;					// 10��
						compare_check_humi = COMM_EXT_HUMI;			// ���� �񱳰��� �ҹ��Ѵ�.
						//humi_check_timeout = 60;					// 10��
					}
					
					if(flg_heater_runrate_first)
						cycle_count++;
				}
			}
			else
			{
				if(COMM_EXT_TEMP <= (Flashdatarec.e2p_auto_temp - 5))		// �ҹ��µ�(��ǥ�µ�) - 5 ������ ���?
				{
					flg_heater_onoff = 1;							// ���� On Off ���� �÷���
					flg_heater_out = 1;								// ���� ���?
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
			
			if(flg_hsen_error)										// ���� ���� ������ 30�� �� ���� �ϰ� ����
			{
				if(humi_err_timeout == 0)
					oper_step = 5;
			}
			break;
		case 5:														// �ð� ����
			//mprintf("Cooling Process..\n");
			flg_coolled_amulating = 1;
			flg_cwd = 0;
			flg_coolling = 1;
			coolling_timeout = 1800;								// 30�� 
			prev_cooling_temper = COMM_EXT_TEMP;
		
			circula_fan_speed_level = 10;
			flg_cirfan_out = 1;										// ��ȯ ��
			flg_heater_out = 0;										// ����
			flg_duct_out = 1;										// ��Ʈ Open Close
			ventilation_fan_speed_level = 6;						// 2021-07-06 ���α׷� ������û���� 10���� ����
			flg_ventilfan_out = 1;									// ���? ��
			flg_uvlamp_out = 1;
			oper_step = 6;
			flg_heater_runrate_first = 0;
		
			playend_count = 3;
			flg_dryend = 1;
			break;
		case 6:
			if(COMM_EXT_TEMP <= 50)									// ���οµ��� 40�� ����
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
			flg_cirfan_out = 0;										// ��ȯ ��
			flg_ventilfan_out = 0;									// ���? ��
			flg_heater_out = 0;										// ����
			flg_duct_out = 0;										// ��Ʈ Open Close
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
// ���
void Ster_Proc(void)
{
	switch(oper_step)
	{
		case 0:
			flg_cirfan_out = 0;											// ��ȯ ��
			ventilation_fan_speed_level = 10;
			flg_ventilfan_out = 1;										// ��� ��
			flg_heater_out = 0;											// ����
			flg_smoke_heater_out = 0;									// ���� ����
			flg_smoke_pump_out = 0;										// ���� ����
			flg_duct_out = 1;											// ��Ʈ Open Close
			flg_uvlamp_out = 0;
			oper_timeout = 20;											// 20�� �������� ����
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
				flg_cirfan_out = 1;										// ��ȯ ��
				flg_heater_out = 1;										// ����
				flg_duct_out = 0;										// ��Ʈ Close
				flg_ventilfan_out = 0;									// ��� ��
				flg_uvlamp_out = 1;
			
				cycle_count = 0;
				flg_heater_runrate_first = 0;
				heater_running_time = 0;
				heater_run_total_time = 0;
				
				flg_goal_temp_ok = 0;
				flg_heater_onoff = 1;									// ���� On Off ���� �÷���
				flg_check_ster_first = 1;
				oper_step = 2;
				oper_total_timeout = 7200;								// 2�ð�
				
				// �����µ��� ����µ��� �� 1�� ���� 1�о�
				oper_remain_time = (Flashdatarec.e2p_ster_temp - COMM_EXT_TEMP + 15) * 60;
			}
			break;
		case 2:
			if(flg_goal_temp_ok)
			{
				ventilation_fan_speed_level = 6;
				flg_ventilfan_out = 1;										// ��� ��
				flg_duct_out = 1;											// ��Ʈ Open Close
				
				if(ster_run_timeout == 0)
					oper_step = 3;
			}
			
			if(flg_heater_onoff)									// ���� �������̸�
			{
				if(Flashdatarec.e2p_ster_temp <= COMM_EXT_TEMP)		// �����µ�(��ǥ�µ�) �̻��� ���
				{
					flg_goal_temp_ok = 1;							// ��ǥ �µ� ����
					flg_heater_onoff = 0;							// ���� On Off ���� �÷���
					flg_heater_out = 0;								// ���� ���
					
					if(flg_check_ster_first)
					{
						flg_check_ster_first = 0;
						ster_run_timeout = Flashdatarec.e2p_ster_time * 60;	// 15�� 
					}
					
					if(flg_heater_runrate_first)
						cycle_count++;
				}
			}
			else
			{
				if(COMM_EXT_TEMP <= (Flashdatarec.e2p_ster_temp - 5))		// �����µ�(��ǥ�µ�) - 5 ������ ���
				{
					flg_heater_onoff = 1;							// ���� On Off ���� �÷���
					flg_heater_out = 1;								// ���� ���
					
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
		case 3:														// �ð� ����
			flg_coolling = 1;
			flg_coolled_amulating = 1;
			flg_cwd = 0;
			coolling_timeout = 1800;								// 30��
			prev_cooling_temper = COMM_EXT_TEMP;
			
			circula_fan_speed_level = 10;
			flg_cirfan_out = 1;										// ��ȯ ��
			flg_heater_out = 0;										// ����
			flg_duct_out = 1;										// ��Ʈ Open Close
			ventilation_fan_speed_level = 10;						// 2021-07-06 ���α׷� ������û���� 10���� ����
			flg_ventilfan_out = 1;									// ��� ��
			flg_uvlamp_out = 1;
			flg_heater_runrate_first = 0;
			oper_step = 4;
		
			playend_count = 3;
			flg_dryend = 1;
			break;
		case 4:
			if((COMM_EXT_TEMP <= 50) && (COMM_HEATER_TEMP <= 60))	// ���οµ��� 50�� ����
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
			flg_cirfan_out = 0;										// ��ȯ ��
			flg_ventilfan_out = 0;									// ���? ��
			flg_heater_out = 0;										// ����
			flg_duct_out = 0;										// ��Ʈ Open Close
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
// ��������
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
			oper_timeout = 5;							// 5��
			break;
		case 1:
			if(COMM_SMOKE_HEATEROUT_STATE)				// Smoke Heater Bimetal�� ���? ����
			{
				oper_timeout = 180;						// 3��
				smoke_step = 2;
			}
			else
			{
				if(oper_timeout == 0)
				{
					oper_timeout = 2400;				// 40 ��					
					smoke_step = 10;
				}
			}
			break;
		case 2:
			if(COMM_SMOKE_HEATEROUT_STATE == 0)				// Smoke Heater Bimetal�� ��������
			{
				flg_smoke_heater_out = 0;					// smoke heater off
				flg_smoke_pump_out = 1;
				
				circula_fan_speed_level = 10;
				flg_cirfan_out = 1;							// ��ȯ ��
				
				smoke_run_timeout = 60;						// 6��
				smoke_step = 3;
			}
			else
			{
				if(oper_timeout == 0)
				{
					flg_smoke_heater_out = 0;				// smoke heater off
					flg_smoke_pump_out = 0;
					// ����ó��
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
				flg_cirfan_out = 1;							// ��ȯ ��
				oper_timeout = 120;							// 2��
				smoke_step = 4;
			}
			break;
		case 4:
			if(oper_timeout == 0)
			{
				flg_cirfan_out = 0;							// ��ȯ ��
				smoke_step = 99;
			}
			break;
		case 10:
			if(COMM_SMOKE_HEATEROUT_STATE)				// Smoke Heater Bimetal�� ���? ����
			{
				oper_timeout = 180;						// 3��
				smoke_step = 2;
			}
			else
			{
				if(oper_timeout == 0)
				{
					flg_smoke_heater_out = 0;				// smoke heater off
					flg_smoke_pump_out = 0;
						// ����ó��
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
// ���� OFF�� �Ǵ� ���� ����� ���� �µ��� �ð� ��Ų��.
void Cooling_Proc(void)
{
	switch(cool_step)
	{
		case 0:
			if((COMM_EXT_TEMP <= 50) && (COMM_HEATER_TEMP < 100))										// ���οµ��� 50�� ����
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
			flg_cirfan_out = 1;											// ��ȯ ��
			ventilation_fan_speed_level = 10;
			flg_ventilfan_out = 1;										// ���? ��
			flg_heater_out = 0;											// ����
			flg_smoke_heater_out = 0;									// ���� ����
			flg_smoke_pump_out = 0;										// ���� ����
			flg_duct_out = 1;											// ��Ʈ Open Close
			flg_uvlamp_out = 1;

			if((COMM_EXT_TEMP <= 50) && (COMM_HEATER_TEMP < 100))		// ���οµ��� 50�� ����
				cool_step = 99;
			break;
		case 99:
			flg_cirfan_out = 0;											// ��ȯ ��
			flg_ventilfan_out = 0;										// ���? ��
			flg_heater_out = 0;											// ����
			flg_smoke_heater_out = 0;									// ���� ����
			flg_smoke_pump_out = 0;										// ���� ����
			flg_duct_out = 0;											// ��Ʈ Open Close
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
			if((COMM_EXT_TEMP > 50) || (COMM_HEATER_TEMP >= 100))										// ���οµ��� 50�� �ʰ�
				cool_step = 0;
		default:
			break;
	}
}
//-----------------------------------------------------------------------------------------------------------------
void Stop_Proc(void)
{
	op_mode = STOP_MODE;
		
	if((Flashdatarec.e2p_operated_time / 3600) >= 1000)					// 1000�ð� �̻��� ���
		disp_mode = DISP_FILTER;
	else
		disp_mode = DISP_WAIT;
	
	oper_step = 0;
	smoke_step = 0;
	cool_step = 0;
	flg_smoke_select = 0;
	//error_code &= EB_ERR_SET;		// 485 ������ ������ ������ ������ Ŭ���� �Ѵ�.
	flg_coolled_amulating = 0;
	flg_cwd = 0;
	Waitled_on();
	
	if(error_code == 0)
	{
		Buzzer_Stop();
	}
}
//-----------------------------------------------------------------------------------------------------------------
// Ű �Է� ó��
void InputKey_Proc(void)
{
	// Up, Down Key ���ÿ� �����ҹ��? ���� �µ��� �ҹ��ش�.
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
	// ���� ������ ���? 
	if(temper_open_error)	
	{
		if((menu_key[UP_KEY]) || (menu_key[DOWN_KEY]) || (menu_key[AUTO_KEY]) || 
			(menu_key[RESERVE_KEY]) || (menu_key[STER_KEY]) || (menu_key[SMOKE_KEY]) || (menu_key[RUNSTOP_KEY]))
		{
			Play_Buzzer(0);				
		}
		
		goto KEY_CLEAR;
	}
	
	if(disp_mode == DISP_RESERVE)											// ���� ������
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

				if(disp_subnum == 0)					// �ð���
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
		else if((menu_key[UP_KEY] || key_continued[UP_KEY]) && (key_pushed[DOWN_KEY] == 0)) // Up Ű
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
				
				if(disp_subnum == 0)					// �ð���
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
				Led_data_set(1, LEDC_TIME, 1);				// �ð�
				Led_data_set(3, LEDC_MINUTE, 1);			// ��
				
				Led_data_set(1, LEDC_TEMP, 0);				// �µ�
				Led_data_set(3, LEDC_TIMEREMAIN, 0);		// �ҹ��ð�
				
				if(op_mode == STOP_MODE)
				{
					Led_data_set(6, LEDT_UP, 1);				// ��ġ �ø�
					Led_data_set(7, LEDT_DOWN, 1);				// ��ġ ����
					Led_data_set(8, LEDI_UP, 1);				// ������ �ø�
					Led_data_set(8, LEDL_DOWN, 1);				// ������ ����
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
	if(op_mode == STOP_MODE)			// ���ҹ��?
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
					cool_step = 0;												// �ð� ó���� �Ѵ�.
					smoke_step = 0;
					Smokeled_on();
					Led_data_set(3, LEDC_SMOKE, 1);				// +��
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
		else if((menu_key[RUNSTOP_KEY]) && (disp_mode == DISP_OPTION))     // ���� Key
		{
				disp_timeout = 0;
				disp_mode = DISP_WAIT;
				e2p_save_time = 10;											// 1�� �� ����
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
		else if((menu_key[AUTO_KEY]) && (key_pushed[RESERVE_KEY] == 0) && (disp_mode == DISP_OPTION) && (error_code == 0))		// �������� ����
		{
			if(disp_mode == DISP_OPTION)
			{
				optkey_timeout = 10;
				opt_num++;
				if(opt_num > 7)			
					opt_num = 1;
				
				e2p_save_time = 10;											// 1�� �� ����
				Play_Buzzer(1);													// Button
			}
			else
				Play_Buzzer(0);						// Not
		}	//----------------------------------------------------------------------------------------------------------
		else if((menu_key[RESERVE_KEY]) && (key_pushed[AUTO_KEY] == 0) && (disp_mode == DISP_OPTION) && (error_code == 0))		// �������� �ø�
		{
			if(disp_mode == DISP_OPTION)
			{
				optkey_timeout = 10;
				
				if(opt_num > 1)			
					opt_num--;
				else
					opt_num = 7;
				
				e2p_save_time = 10;											// 1�� �� ����
				Play_Buzzer(1);													// Button
			}
			else
				Play_Buzzer(0);						// Not
		}	//----------------------------------------------------------------------------------------------------------
		else if((menu_key[DOWN_KEY] || key_continued[DOWN_KEY]) && (key_pushed[UP_KEY] == 0) && \
				(disp_mode == DISP_OPTION) && (error_code == 0))				// Down Ű
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
					e2p_save_time = 30;							// 3�� �� ����
					switch(opt_num)
					{
						case 1:										// ��ȯ �ҷ� üũ�� �ǳ��µ��� ���ⱸ �µ��� ���� ���ⱸ �µ� 10�� �̻�
							if(Flashdatarec.e2p_cirfan_diff_temp > 1)
								Flashdatarec.e2p_cirfan_diff_temp--;
							break;
						case 2:										// ���� �ҷ� üũ�� �ǳ��µ��� ���ⱸ �µ��� ���� 1������(���� �۵� 3���� ���� üũ)
							if(Flashdatarec.e2p_heater_diff_temp > 1)
								Flashdatarec.e2p_heater_diff_temp--;
							break;
						case 3:										// �ʴ��� �������� ������ 40�� �̳��� �������� ���½�ȣ�� �������� ������ ������ �������� ó���Ѵ�.
							if(Flashdatarec.e2p_smokbimetal_time > 10)
								Flashdatarec.e2p_smokbimetal_time--;	
							break;
						case 4:										// ȯ�� ������ ������ ������ N% �ѹ��̶� �ҹ��µ� ���� ���ĺ��� 3����Ŭ �ݺ��Ŀ� ���� üũ
							if(Flashdatarec.e2p_heater_oper_percent > 1)
								Flashdatarec.e2p_heater_oper_percent--;	
							break;
						case 5:										// �������� ���� N% ����				
							if(Flashdatarec.e2p_under_err_humi > 1)
								Flashdatarec.e2p_under_err_humi--;	
							break;
						case 6:										// �������� ���� N% �̻�
							if(Flashdatarec.e2p_over_err_humi > 80)
								Flashdatarec.e2p_over_err_humi--;
							break;
						case 7:										// ���ͱ�ȯ �ñ� ���� �ð� (1-99) ���� 10�ð�
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
				(disp_mode == DISP_OPTION) && (error_code == 0))				// Up Ű
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
					e2p_save_time = 30;							// 3�� �� ����
					switch(opt_num)
					{
						case 1:										// ��ȯ �ҷ� üũ�� �ǳ��µ��� ���ⱸ �µ��� ���� ���ⱸ �µ� 10�� �̻�
							if(Flashdatarec.e2p_cirfan_diff_temp < 30)
								Flashdatarec.e2p_cirfan_diff_temp++;
							break;
						case 2:										// ���� �ҷ� üũ�� �ǳ��µ��� ���ⱸ �µ��� ���� 1������(���� �۵� 3���� ���� üũ)
							if(Flashdatarec.e2p_heater_diff_temp < 10)
								Flashdatarec.e2p_heater_diff_temp++;
							break;
						case 3:										// �ʴ��� �������� ������ 40�� �̳��� �������� ���½�ȣ�� �������� ������ ������ �������� ó���Ѵ�.
							if(Flashdatarec.e2p_smokbimetal_time < 90)
								Flashdatarec.e2p_smokbimetal_time++;	
							break;
						case 4:										// ȯ�� ������ ������ ������ N% �ѹ��̶� �ҹ��µ� ���� ���ĺ��� 3����Ŭ �ݺ��Ŀ� ���� üũ
							if(Flashdatarec.e2p_heater_oper_percent < 90)
								Flashdatarec.e2p_heater_oper_percent++;	
							break;
						case 5:										// �������� ���� N% ����				
							if(Flashdatarec.e2p_under_err_humi < 10)
								Flashdatarec.e2p_under_err_humi++;	
							break;
						case 6:										// �������� ���� N% �̻�
							if(Flashdatarec.e2p_over_err_humi < 99)
								Flashdatarec.e2p_over_err_humi++;
							break;
						case 7:										// ���ͱ�ȯ �ñ� ���� �ð� (1-99) ���� 10�ð�
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
	else											// ���ҹ��?
	{
		if(menu_key[RUNSTOP_KEY])      			// ����
		{
			Stop_Proc();
			Play_Buzzer(2);						// Power Off
		}
		else if((menu_key[UP_KEY] || key_continued[UP_KEY]) && (key_pushed[DOWN_KEY] == 0) &&
				(disp_mode != DISP_HEATER_TEMP) && (disp_mode != DISP_HUMIDITY) && (error_code == 0))				// Up Ű
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
					case AUTO_MODE:					// �ڵ����?
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
					case STER_MODE:					// ��ո��
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
				(disp_mode != DISP_HEATER_TEMP) && (disp_mode != DISP_HUMIDITY) && (error_code == 0))	// Down Ű
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
					case AUTO_MODE:					// �ڵ����?
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
					case STER_MODE:					// ��ո��
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
		else if(menu_key[AUTO_KEY] && (disp_mode != DISP_RESERVE) && (error_code == 0))					// �ڵ� Ű
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
		else if((menu_key[STER_KEY]) && (disp_mode != DISP_RESERVE) && (error_code == 0))					// ���?
		{
			if(op_kind != STER_MODE)
			{
				op_kind = STER_MODE;
				ster_run_timeout = Flashdatarec.e2p_ster_time * 60;	// �ð�
				oper_step = 0;
				Sterled_on();
				Play_Buzzer(1);
			}
			else
				Play_Buzzer(0);
		}
		else if((menu_key[SMOKE_KEY]) && (disp_mode != DISP_RESERVE) && (error_code == 0))					// �� �л�
		{
			if((op_kind == AUTO_MODE) || (op_kind == STER_MODE))
			{
				flg_smoke_select = !flg_smoke_select;
				if(flg_smoke_select)
				{
					cool_step = 0;												// �ð� ó���� �Ѵ�.
					smoke_step = 0;
					Led_data_set(3, LEDC_SMOKE, 1);				// +��
				}
				else
					Led_data_set(3, LEDC_SMOKE, 0);				// +��

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
// �Է� Check ó��
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
	//----------------------------------------------------- // ǳ�� ����ġ
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
// ��� ó��
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

		COMM_CIRFAN_ON = flg_cirfan_out;				// ��ȯ ��
		COMM_VENTILFAN_ON = flg_ventilfan_out;			// ���? ��

		COMM_HEATER_ON = flg_heater_out;				// ����

		COMM_DUCT = flg_duct_out;					// ��Ʈ Open Close
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
				circula_fan_off_timeout = 50;	// 5 ��
				flg_circula_fan_backup  = 1;
				circula_fan_speed_backup = circula_fan_speed_level;
				circula_fan_speed_level = 3;
				COMM_CIRFAN_ON = 1;				// ��ȯ ��
			}
		}
		
		COMM_VENTILFAN_ON = 0;			//��� ��
		COMM_HEATER_ON = 0;				// ����
		COMM_DUCT = 0;					// ��Ʈ Open Close
		COMM_UVLAMP_ON = 0;
		COMM_AC220_SPARE1 = 0;

		COMM_SMOKE_HEATER_ON = 0;		// SMOKE HEATER
		COMM_SMOKE_PUMP_ON = 0;			// SMOKE PUMP
		
		// ���� ������ ������ �߻��� ���������� �Ϸ��Ѱ����� �Ǵ��Ѵ�.
		if((op_mode == RUN_MODE) && (op_kind == IDLE_MODE) && flg_smoke_select)
		{
			flg_smoke_select = 0;
			flg_cirfan_out = 0;
			smoke_step = 100;
			flg_coolled_amulating = 0;
		}
		
		// ������ ���� Open �� �ð� Clear
		if((op_mode == RUN_MODE) && ((op_kind == AUTO_MODE) || (op_kind == MANUAL_MODE) || (op_kind == STER_MODE)))
		{
			switch(op_kind)
			{
				case AUTO_MODE:
					// ���� �����ð� Clear
					oper_total_timeout = 3600 * 2;	// �� ���� ���� �ð�
					break;
				case STER_MODE:
						// �����ð� Clear
					if(oper_step == 2)
					{
						if(flg_check_ster_first == 0)
						{
							ster_run_timeout = Flashdatarec.e2p_ster_time * 60;	// 15�� 
						}
					}
					break;
				default:
					break;
			}
		}
	}
	
	if(temper_open_error)				// ���� ������ LED LAMP�� ���ش�
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
		if(flg_cwd == 0)						// CWD ����
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
		else											// CCWD				��ȯ��
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
	
	if(error_code > 0)                             // Error ǥ��
	{
		Disp_ErrCode();
	}
	else
	{
		if(temper_open_error)						// ���� ������
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
		
		if(disp_mode != DISP_RESERVE)					// ���� �������� �ƴ϶��?		
		{
			if(flg_reserve)								// ������ �ִٸ�
				Led_data_set(1, LEDC_RESERVE, 1);		// ���� ����
			else 
				Led_data_set(1, LEDC_RESERVE, 0);		// ���� �ҵ�
		}
		
		switch(disp_mode)
		{
			case DISP_FILTER:								// ���ͱ�ȯ �޽��� ���DISP_FILTER
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
			case DISP_DOOROPEN:								// ������ ����
				break;
			case DISP_TEMPTIME:								// ���� �µ� �� �ҹ��ð� ǥ��
				Disp_Temp_Seg(COMM_EXT_TEMP);
				if((flg_coolled_amulating == 0) && (op_kind != IDLE_MODE))
				{
					if(oper_remain_time < 60)
						oper_remain_time = 60;
					
					Disp_Time_Seg(oper_remain_time / 60);
				}
				break;
			case DISP_TEMPSET:                       		// ����
				switch(op_kind)
				{
					case AUTO_MODE:							// �ڵ�����
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
					case STER_MODE:							// ��ո��
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
			case DISP_RESERVE:								// ���� ���� ���?
				if(flg_toggle_500ms)
				{
					if(flg_toggle_500ms != disp_toggle)
					{
						disp_toggle = flg_toggle_500ms;
						
						Led_data_set(1, LEDC_RESERVE, 1);
						if(disp_subnum == 0)					// �ð�
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
						if(disp_subnum == 0)					// �ð�
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

	Led_data_set(0, LEDC_AUTO, 0);				// �ڵ�
	Led_data_set(1, LEDC_DRY, 0);				// ����
	Led_data_set(1, LEDC_TEMP, 0);				// �µ�
	Led_data_set(1, LEDC_TIME, 0);				// �ð�

	Led_data_set(2, LEDC_STABIL, 0);			// ���?
	Led_data_set(3, LEDC_DEODRAN, 0);			// Ż��
	Led_data_set(3, LEDC_SMOKE, 0);				// +��
	Led_data_set(3, LEDC_MINUTE, 0);			// ��
	Led_data_set(3, LEDC_TIMEREMAIN, 0);		// �ҹ��ð�
	Led_data_set(7, LEDC_TIMEREMAIN2, 0);		// �ҹ��ð�

	Led_data_set(4, LEDT_POWER, 0);				// ��ġ ����
	Led_data_set(5, LEDT_AUTO, 1);				// ��ġ �ڵ�
	Led_data_set(5, LEDT_RESERVE, 1);			// ��ġ ����
	Led_data_set(5, LEDT_STABIL, 1);			// ��ġ ���?/Ż��
	Led_data_set(7, LEDT_STABIL2, 1);			// ��ġ ���?/Ż��
	Led_data_set(5, LEDT_SMOKE, 1);				// ��ġ +��

	Led_data_set(6, LEDT_UP, 0);				// ��ġ �ø�
	Led_data_set(7, LEDT_DOWN, 0);				// ��ġ ����
	
	Led_data_set(8, LEDI_POWER, 0);				// ������ ����
	Led_data_set(8, LEDI_AUTO, 1);				// ������ �ڵ�
	Led_data_set(8, LEDI_RESERVE, 1);			// ������ ����
	Led_data_set(8, LEDI_STABIL, 1);			// ������ ���?
	Led_data_set(8, LEDI_SMOKE, 1);				// ������ +��
	Led_data_set(8, LEDI_UP, 0);				// ������ �ø�
	Led_data_set(8, LEDL_DOWN, 0);				// ������ ����
}
//---------------------------------------------------------------------------------------------------------
void Optionled_on(void)
{
	Led_data_set(0, LEDC_AUTO, 0);				// �ڵ�
	Led_data_set(1, LEDC_DRY, 0);				// ����
	Led_data_set(1, LEDC_TEMP, 0);				// �µ�
	Led_data_set(1, LEDC_TIME, 0);				// �ð�

	Led_data_set(2, LEDC_STABIL, 0);			// ���?
	Led_data_set(3, LEDC_DEODRAN, 0);			// Ż��
	Led_data_set(3, LEDC_MINUTE, 0);			// ��
	Led_data_set(3, LEDC_TIMEREMAIN, 0);		// �ҹ��ð�
	Led_data_set(7, LEDC_TIMEREMAIN2, 0);		// �ҹ��ð�
	
	Led_data_set(4, LEDT_POWER, 1);				// ��ġ ����
	Led_data_set(5, LEDT_AUTO, 1);				// ��ġ �ڵ�
	Led_data_set(5, LEDT_RESERVE, 1);			// ��ġ ����
	Led_data_set(5, LEDT_STABIL, 0);			// ��ġ ���?/Ż��
	Led_data_set(7, LEDT_STABIL2, 0);			// ��ġ ���?/Ż��

	Led_data_set(6, LEDT_UP, 1);				// ��ġ �ø�
	Led_data_set(7, LEDT_DOWN, 1);				// ��ġ ����
	
	Led_data_set(8, LEDI_POWER, 1);				// ������ ����
	Led_data_set(8, LEDI_AUTO, 1);				// ������ �ڵ�
	Led_data_set(8, LEDI_RESERVE, 1);			// ������ ����
	Led_data_set(8, LEDI_STABIL, 0);			// ������ ���?
	Led_data_set(8, LEDI_SMOKE, 0);				// ������ +��
	Led_data_set(8, LEDI_UP, 1);				// ������ �ø�
	Led_data_set(8, LEDL_DOWN, 1);				// ������ ����
}
//---------------------------------------------------------------------------------------------------------
void Autoled_on(void)
{
	Led_data_set(0, LEDC_AUTO, 1);				// �ڵ�
	Led_data_set(1, LEDC_DRY, 1);				// ����
	Led_data_set(1, LEDC_TEMP, 1);				// �µ�
	Led_data_set(1, LEDC_TIME, 0);				// �ð�

	Led_data_set(2, LEDC_STABIL, 1);			// ���?
	Led_data_set(3, LEDC_DEODRAN, 1);			// Ż��
	Led_data_set(3, LEDC_MINUTE, 1);			// ��
	Led_data_set(3, LEDC_TIMEREMAIN, 1);		// �ҹ��ð�
	Led_data_set(7, LEDC_TIMEREMAIN2, 1);		// �ҹ��ð�

	Led_data_set(4, LEDT_POWER, 1);				// ��ġ ����
	Led_data_set(5, LEDT_AUTO, 1);				// ��ġ �ڵ�
	Led_data_set(5, LEDT_RESERVE, 1);			// ��ġ ����
	Led_data_set(5, LEDT_STABIL, 1);			// ��ġ ���?/Ż��
	Led_data_set(7, LEDT_STABIL2, 1);			// ��ġ ���?/Ż��
	Led_data_set(5, LEDT_SMOKE, 1);				// ��ġ +��

	Led_data_set(6, LEDT_UP, 1);				// ��ġ �ø�
	Led_data_set(7, LEDT_DOWN, 1);				// ��ġ ����
	
	Led_data_set(8, LEDI_POWER, 1);				// ������ ����
	Led_data_set(8, LEDI_AUTO, 1);				// ������ �ڵ�
	Led_data_set(8, LEDI_RESERVE, 1);			// ������ ����
	Led_data_set(8, LEDI_STABIL, 1);			// ������ ���?
	Led_data_set(8, LEDI_SMOKE, 1);				// ������ +��
	Led_data_set(8, LEDI_UP, 1);				// ������ �ø�
	Led_data_set(8, LEDL_DOWN, 1);				// ������ ����
}
//---------------------------------------------------------------------------------------------------------
void Sterled_on(void)
{
	Led_data_set(0, LEDC_AUTO, 0);				// �ڵ�
	Led_data_set(1, LEDC_DRY, 1);				// ����
	Led_data_set(1, LEDC_TEMP, 1);				// �µ�
	Led_data_set(1, LEDC_TIME, 0);				// �ð�

	Led_data_set(2, LEDC_STABIL, 1);			// ���?
	Led_data_set(3, LEDC_DEODRAN, 1);			// Ż��
	Led_data_set(3, LEDC_MINUTE, 1);			// ��
	Led_data_set(3, LEDC_TIMEREMAIN, 1);		// �ҹ��ð�
	Led_data_set(7, LEDC_TIMEREMAIN2, 1);		// �ҹ��ð�
	
	Led_data_set(4, LEDT_POWER, 1);				// ��ġ ����
	Led_data_set(5, LEDT_AUTO, 1);				// ��ġ �ڵ�
	Led_data_set(5, LEDT_RESERVE, 1);			// ��ġ ����
	Led_data_set(5, LEDT_STABIL, 1);			// ��ġ ���?/Ż��
	Led_data_set(7, LEDT_STABIL2, 1);			// ��ġ ���?/Ż��
	Led_data_set(5, LEDT_SMOKE, 1);				// ��ġ +��
	
	Led_data_set(6, LEDT_UP, 1);				// ��ġ �ø�
	Led_data_set(7, LEDT_DOWN, 1);				// ��ġ ����
	
	Led_data_set(8, LEDI_POWER, 1);				// ������ ����
	Led_data_set(8, LEDI_AUTO, 1);				// ������ �ڵ�
	Led_data_set(8, LEDI_RESERVE, 1);			// ������ ����
	Led_data_set(8, LEDI_STABIL, 1);			// ������ ���?
	Led_data_set(8, LEDI_SMOKE, 1);				// ������ +��
	Led_data_set(8, LEDI_UP, 1);				// ������ �ø�
	Led_data_set(8, LEDL_DOWN, 1);				// ������ ����
}
//---------------------------------------------------------------------------------------------------------
void Smokeled_on(void)
{
	Led_data_set(0, LEDC_AUTO, 0);				// �ڵ�
	Led_data_set(1, LEDC_DRY, 0);				// ����
	Led_data_set(1, LEDC_TEMP, 1);				// �µ�
	Led_data_set(1, LEDC_TIME, 0);				// �ð�

	Led_data_set(2, LEDC_STABIL, 0);			// ���?
	Led_data_set(3, LEDC_DEODRAN, 0);			// Ż��
	Led_data_set(3, LEDC_MINUTE, 0);			// ��
	Led_data_set(3, LEDC_TIMEREMAIN, 0);		// �ҹ��ð�
	Led_data_set(7, LEDC_TIMEREMAIN2, 1);		// �ҹ��ð�

	Led_data_set(4, LEDT_POWER, 1);				// ��ġ ����
	Led_data_set(5, LEDT_AUTO, 1);				// ��ġ �ڵ�
	Led_data_set(5, LEDT_RESERVE, 1);			// ��ġ ����
	Led_data_set(5, LEDT_STABIL, 1);			// ��ġ ���?/Ż��
	Led_data_set(7, LEDT_STABIL2, 1);			// ��ġ ���?/Ż��
	Led_data_set(5, LEDT_SMOKE, 1);				// ��ġ +��

	Led_data_set(6, LEDT_UP, 0);				// ��ġ �ø�
	Led_data_set(7, LEDT_DOWN, 0);				// ��ġ ����
	
	Led_data_set(8, LEDI_POWER, 1);				// ������ ����
	Led_data_set(8, LEDI_AUTO, 1);				// ������ �ڵ�
	Led_data_set(8, LEDI_RESERVE, 1);			// ������ ����
	Led_data_set(8, LEDI_STABIL, 1);			// ������ ���?
	Led_data_set(8, LEDI_SMOKE, 1);				// ������ +��
	Led_data_set(8, LEDI_UP, 0);				// ������ �ø�
	Led_data_set(8, LEDL_DOWN, 0);				// ������ ����
}
//---------------------------------------------------------------------------------------------------------
void Disp_Option(void)
{
	switch(opt_num)
	{
		case 1:										// ��ȯ �ҷ� üũ
			Seg_data_set(0, 12, 0);           		// 'C' ǥ��
			Seg_data_set(2, 14, 0);           		// 'E' ǥ��
		
			Seg_data_set(4, (Flashdatarec.e2p_cirfan_diff_temp / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_cirfan_diff_temp % 10), 0);
			break;
		case 2:										// ���� �ҷ� üũ
			Seg_data_set(0, 21, 0);           		// 'H' ǥ��
			Seg_data_set(2, 14, 0);           		// 'E' ǥ��
		
			Seg_data_set(4, (Flashdatarec.e2p_heater_diff_temp / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_heater_diff_temp % 10), 0);
			break;
		case 3:										// ������ ���� üũ(�ҹ��ҷ�)
			Seg_data_set(0, 5, 0);           		// 'S' ǥ��
			Seg_data_set(2, 14, 0);           		// 'E' ǥ��
		
			Seg_data_set(4, (Flashdatarec.e2p_smokbimetal_time / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_smokbimetal_time % 10), 0);
			break;
		case 4:										// ȯ��ҷ�? üũ
			Seg_data_set(0, 0, 0);           		// 'O' ǥ��
			Seg_data_set(2, 14, 0);           		// 'E' ǥ��
		
			Seg_data_set(4, (Flashdatarec.e2p_heater_oper_percent / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_heater_oper_percent % 10), 0);
			break;
		case 5:										// �������� �ҷ� N% ����
			Seg_data_set(0, 29, 0);           		// 'u' ǥ��
			Seg_data_set(2, 21, 0);           		// 'H' ǥ��
		
			Seg_data_set(4, (Flashdatarec.e2p_under_err_humi / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_under_err_humi % 10), 0);
			break;
		case 6:										// �������� �ҷ� N% �̻�
			Seg_data_set(0, 20, 0);           		// 'o' ǥ��
			Seg_data_set(2, 21, 0);           		// 'H' ǥ��
		
			Seg_data_set(4, (Flashdatarec.e2p_over_err_humi / 10), 0);
			Seg_data_set(6, (Flashdatarec.e2p_over_err_humi % 10), 0);
			break;
		case 7:										// ���� û��/��ȯ
			Seg_data_set(0, 15, 0);           		// 'F' ǥ��
			Seg_data_set(2, 24, 0);           		// 't' ǥ��
		
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

			flg_errscan = !flg_errscan;    // 2ȸ �ҹ��̱� ����
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
			Play_Buzzer(5);        // ������
		}
	}
}
//---------------------------------------------------------------------------------------------------------
// ��ȯ�ҷ� üũ
void Circula_Error_Check(void)
{
	if(flg_cirfan_out && flg_heater_out && (circula_fan_speed_level == 10) 
		&& (op_mode == RUN_MODE) && (temper_open_error == 0))
	{
		// ���οµ��� ���ⱸ �µ� ���̰� 20�� �̻� ���̰� ����
		if(COMM_EXT_TEMP <= (COMM_HEATER_TEMP - Flashdatarec.e2p_cirfan_diff_temp))
		{
			if(flg_cirtimeout_first == 0)
			{
				flg_cirtimeout_first = 1;
				cir_err_timeout = 600;					// 30��  1800 -> 10min 600 timeout //240904
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
// ���ͺҷ� üũ
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
// ������ҷ� üũ
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
				if(COMM_SMOKE_HEATEROUT_STATE)				// Smoke Heater Bimetal�� �پ� �ִٸ�
					error_code |= E5_ERR_SET;
			}
		}
	}
	else
		flg_smokeheater_timeout_first = 0;
}
//---------------------------------------------------------------------------------------------------------
// ������ ���� üũ
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
//				if(COMM_SMOKE_HEATEROUT_STATE)				// Smoke Heater Bimetal�� �پ� �ִٸ�
//					error_code |= E4_ERR_SET;
//			}
//		}
//	}
//	else
//		flg_liquid_timeout_first = 0;
//}
//---------------------------------------------------------------------------------------------------------
// ȯ��ҷ� üũ
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
// �ǳ��µ� �����ҷ� üũ
void Temperature_Error_Check(void)
{
	if((COMM_EXT_TEMP <= -40) || (COMM_EXT_TEMP >= 100))
		error_code |= E8_ERR_SET;
	else
		error_code &= ~E8_ERR_SET;
}
//---------------------------------------------------------------------------------------------------------
// ���� ����µ� �����ҷ� üũ
void HeaterTemper_Error_Check(void)
{
	if((COMM_HEATER_TEMP <= -40) || (COMM_HEATER_TEMP >= 160))    // 110 -> 160 ���� 24�� 8�� 9�� 
		error_code |= E9_ERR_SET;
	else
		error_code &= ~E9_ERR_SET;
}
//---------------------------------------------------------------------------------------------------------
// ���� ���� �����ҷ� üũ
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
// ���� �����ҷ� üũ
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
// ǳ�� ����ġ�ҷ� üũ
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
		airpresure_chk_timeout = 30;			// 3�� �� ���� üũ
	}
}
//---------------------------------------------------------------------------------------------------------
void Error_Check_Proc(void)
{
	if(sensor_check_timeout == 0)
	{
		Circula_Error_Check();		// 1. ��ȯ �ҷ� üũ : Er 01
		//AirPresure_Error_Check();	// 2. ǳ�н���ġ ���� �ҷ� : Er 02
		OverHeat_Error_Check();		// 3. ���� ���� üũ : Er 03
		// Heater_Error_Check();		// 4. ���� �ҷ� üũ : Er 04
		Smoke_Error_Check();			// 5. ������ҷ� : Er 05
		
		Ventilation_Error_Check();		// 7. ȯ��(���)�ҷ� üũ : Er 07
		Temperature_Error_Check();		// 8. �ǳ��µ����� �ҷ� : Er 08
		HeaterTemper_Error_Check();		// 9. ����µ����� �ҷ� : Er 09
		HumiSensor_Error_Check();		// 10. �������� �ҷ� : Er 10
	}
}
//---------------------------------------------------------------------------------------------------------
// ���� û�� �ð� üũ
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

