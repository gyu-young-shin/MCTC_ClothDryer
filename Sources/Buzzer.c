#include "main.h"
#include "buzzer.h"

uint16_t  	on_delay = 0;
uint16_t  	off_delay = 0;
uint8_t		bs_num = 99;
uint8_t  	buzzer_kind = 0;
uint16_t   	beep_timeout;
uint8_t  	beep_num = 0;
uint16_t  	*psound_data;
uint16_t   	sound_size;


const uint8_t  TempoValue[] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 48, 96, 144, 192, 240};

const uint16_t  STBL_PwrConnect[][2] = {
	{O6_MI, 0x21}, 
	{O6_FA, 0x21},
	{O6_SO, 0x21},
	{O6_RA, 0x21},
	{O6_CI, 0x21},
	{O7_DO, 0x4f}
};

const uint16_t  STBL_PwrOn[][2] = {
	{O6_DO, 0x22},
	{O6_MI, 0x22},
	{O6_SO, 0x1f}
};

const uint16_t  STBL_PwrOff[][2] = {
	{O6_SO, 0x46},
	{O6_MI, 0x46},
	{O6_DO, 0x2f}
};

const uint16_t  STBL_UnjunChange[][2]= {
	{O7_DO, 0x23},
	{O7_FA, 0x1f}
};

const uint16_t  STBL_SwitchOn[][2]= {
	{O7_DO, 0x21},
	{O7_RE, 0x21},
	{O7_MI, 0x22},
	{O7_DO, 0x21},
	{O7_RE, 0x21},
	{O7_MI, 0x22}
};

const uint16_t  STBL_SwitchOff[][2]= {
	{O7_DO, 0x22},
	{O7_RE, 0x22},
	{O7_MI, 0x23},
	{O7_DO, 0x22},
	{O7_RE, 0x22},
	{O7_MI, 0x23}
};

const uint16_t  STBL_Not[][2] = {
	{O7_RA, 0x12},
	{O7_RA, 0x15},
	{O7_RA, 0x1f}
};

const uint16_t  STBL_End[][2] = {
	{O7_RA, 0x12},
	{O7_RA, 0x12},
	{O7_RA, 0x12},
	{O7_RA, 0x13}
};

const uint16_t  STBL_Button[1][2] = {
	{O7_FA, 0x1f}
};



const uint16_t  STBL_LockEn[][2] = {
	{O7_SO, 0x22},
	{O7_CI, 0x1f}
};

const uint16_t  STBL_LockDis[][2]= {
	{O7_CI, 0x12},
	{O7_SO, 0x1f}
};

const uint16_t  STBL_Melody[49][2]= {
      {O6_MI, 0x22},
      {O6_SO, 0x22},
      {O6_SO, 0x26},
      {O6_MI, 0x22},
      {O6_SO, 0x22},
      {O6_SO, 0x26},
      {O6_RA, 0x22},
      {O6_RA, 0x22},
      {O6_RA, 0x22},
      {O6_RA, 0x22},
      {O6_RA, 0x29},
      {O6_SO, 0x22},
      {O6_SO, 0x22},
      {O6_SO, 0x22},
      {O6_SO, 0x22},
      {O6_FA, 0x22},
      {O6_FA, 0x22},
      {O6_FA, 0x22},
      {O6_FA, 0x22},
      {O6_MI, 0x22},
      {O6_MI, 0x22},
      {O6_MI, 0x22},
      {O6_MI, 0x22},
      {O6_MI, 0x29},

      {O6_MI, 0x22},
      {O6_SO, 0x22},
      {O6_SO, 0x40},
      {O6_SO, 0x22},

      {O6_MI, 0x22},
      {O6_SO, 0x22},
      {O6_SO, 0x26},

      {O6_RA, 0x24},
      {O6_RA, 0x02},
      {O6_MI, 0x22},
      {O6_MI, 0x22},
      {O6_SO, 0x29},

      {O6_FA, 0x22},
      {O6_FA, 0x22},
      {O6_FA, 0x22},
      {O6_FA, 0x22},
      {O6_MI, 0x22},
      {O6_MI, 0x22},
      {O6_MI, 0x22},
      {O6_MI, 0x22},
      {O6_RE, 0x22},
      {O6_RE, 0x22},
      {O6_SO, 0x22},
      {O6_SO, 0x22},
      {O6_DO, 0x2F}
};

//==============================================================================
extern	TIM_HandleTypeDef htim17;
//==============================================================================
void Buzzer_Sequence(void)
{
     uint16_t cur_period;

     switch(bs_num)
     {
          case 0:
              if(sound_size > 0)
              {
                  cur_period = *(psound_data + beep_num);
                  PWM_Config(cur_period);
                  beep_num++;
                  cur_period = *(psound_data + beep_num);
                  on_delay = TempoValue[cur_period >> 4];
                  off_delay = TempoValue[cur_period & 0x0F];

                  BUZER_ON;
                  bs_num = 1;
                  sound_size--;
              }
              else
              {
                  Buzzer_Stop();
                  bs_num = 99;
              }
              break;
          case 1:
              if(on_delay > 0)
                  on_delay--;
              else
              {
                  BUZER_OFF;
                  bs_num = 2;
              }
              break;
          case 2:                                      // Echo
              if(off_delay > 0)
                  off_delay--;
              else
              {
                  beep_num++;
                  bs_num = 0;
              }
              break;
          default:
              break;
     }
}

void Play_Buzzer(uint8_t play_num)
{
	BUZER_OFF;

    switch(play_num)
    {
        case 0:
            psound_data = (uint16_t *)(STBL_Not);
            sound_size = sizeof(STBL_Not) / 4;
            break;
        case 1:
            psound_data = (uint16_t *)(STBL_Button);
            sound_size = sizeof(STBL_Button) / 4;
            break;
        case 2:
            psound_data = (uint16_t *)(STBL_PwrOff);
            sound_size = sizeof(STBL_PwrOff) / 4;
            break;
        case 3:
            psound_data = (uint16_t *)(STBL_PwrOn);
            sound_size = sizeof(STBL_PwrOn) / 4;
            break;
        case 4:
            psound_data = (uint16_t *)(STBL_UnjunChange);
            sound_size = sizeof(STBL_UnjunChange) / 4;
            break;
        case 5:
            psound_data = (uint16_t *)(STBL_Melody);
            sound_size = sizeof(STBL_Melody) / 4;
            break;
        case 6:
            psound_data = (uint16_t *)(STBL_LockEn);
            sound_size = sizeof(STBL_LockEn) / 4;
            break;
        case 7:
            psound_data = (uint16_t *)(STBL_PwrConnect);
            sound_size = sizeof(STBL_PwrConnect) / 4;
            break;
        case 8:
            psound_data = (uint16_t *)(STBL_SwitchOff);
            sound_size = sizeof(STBL_SwitchOn) / 4;
            break;
        case 9:
            psound_data = (uint16_t *)(STBL_End);
            sound_size = sizeof(STBL_End) / 4;
            break;
        default:
            break;
    }
    beep_num = 0;
    bs_num = 0; 
}

void Buzzer_Stop(void)
{
    BUZER_OFF;
	HAL_TIM_PWM_Stop(&htim17, TIM_CHANNEL_1);
}

void PWM_Config(uint16_t cur_period)
{
	uint16_t  Channel1Pulse, PrescalerValue = 0;
	TIM_OC_InitTypeDef sConfigOC = {0};

	PrescalerValue = (SystemCoreClock / 1000000) - 1;					// 1MHZ
	Channel1Pulse = (uint16_t) (1000000 / cur_period);

	htim17.Instance = TIM17;
	htim17.Init.Prescaler = PrescalerValue;
	htim17.Init.CounterMode = TIM_COUNTERMODE_DOWN;
	htim17.Init.Period = Channel1Pulse - 1;
	htim17.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim17.Init.RepetitionCounter = 0;
	htim17.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim17) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim17) != HAL_OK)
	{
		Error_Handler();
	}
	
	sConfigOC.OCMode = TIM_OCMODE_PWM2;
	sConfigOC.Pulse = (Channel1Pulse / 2) - 1;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	if (HAL_TIM_PWM_ConfigChannel(&htim17, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	
	if (HAL_TIM_PWM_Start(&htim17, TIM_CHANNEL_1) != HAL_OK)
	{
		/* PWM Generation Error */
		Error_Handler();
	}
	
}
