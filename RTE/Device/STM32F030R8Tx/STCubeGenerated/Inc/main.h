/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define OPER_LED1_Pin GPIO_PIN_13
#define OPER_LED1_GPIO_Port GPIOC
#define RS485_DE_Pin GPIO_PIN_4
#define RS485_DE_GPIO_Port GPIOF
#define TC_SW1_Pin GPIO_PIN_7
#define TC_SW1_GPIO_Port GPIOA
#define TC_SW6_Pin GPIO_PIN_4
#define TC_SW6_GPIO_Port GPIOC
#define TC_SW7_Pin GPIO_PIN_5
#define TC_SW7_GPIO_Port GPIOC
#define TC_SW2_Pin GPIO_PIN_0
#define TC_SW2_GPIO_Port GPIOB
#define TC_SW3_Pin GPIO_PIN_1
#define TC_SW3_GPIO_Port GPIOB
#define TC_SW4_Pin GPIO_PIN_2
#define TC_SW4_GPIO_Port GPIOB
#define TC_SW5_Pin GPIO_PIN_10
#define TC_SW5_GPIO_Port GPIOB
#define SPI_SCK_Pin GPIO_PIN_3
#define SPI_SCK_GPIO_Port GPIOB
#define SPI_DOUT_Pin GPIO_PIN_4
#define SPI_DOUT_GPIO_Port GPIOB
#define SPI_DIN_Pin GPIO_PIN_5
#define SPI_DIN_GPIO_Port GPIOB
#define SP1_NSS_Pin GPIO_PIN_6
#define SP1_NSS_GPIO_Port GPIOB
#define BUZZER_ON_Pin GPIO_PIN_8
#define BUZZER_ON_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
#define  KEY_COUNT  7

#define	BUZER_ON		HAL_GPIO_WritePin(BUZZER_ON_GPIO_Port, BUZZER_ON_Pin, GPIO_PIN_SET)
#define	BUZER_OFF		HAL_GPIO_WritePin(BUZZER_ON_GPIO_Port, BUZZER_ON_Pin, GPIO_PIN_RESET)

#define POWER_BTN		HAL_GPIO_ReadPin(TC_SW2_GPIO_Port, TC_SW2_Pin)
#define UP_BTN			HAL_GPIO_ReadPin(TC_SW6_GPIO_Port, TC_SW6_Pin)
#define DOWN_BTN		HAL_GPIO_ReadPin(TC_SW7_GPIO_Port, TC_SW7_Pin)
#define AUTO_BTN		HAL_GPIO_ReadPin(TC_SW5_GPIO_Port, TC_SW5_Pin)
#define RESERV_BTN		HAL_GPIO_ReadPin(TC_SW3_GPIO_Port, TC_SW3_Pin)
#define STER_BTN		HAL_GPIO_ReadPin(TC_SW1_GPIO_Port, TC_SW1_Pin)
#define SMOKE_BTN		HAL_GPIO_ReadPin(TC_SW4_GPIO_Port, TC_SW4_Pin)

#define	RS485_DE_HIGH	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_SET)
#define	RS485_DE_LOW	HAL_GPIO_WritePin(RS485_DE_GPIO_Port, RS485_DE_Pin, GPIO_PIN_RESET)

#define SCK_HIGH		HAL_GPIO_WritePin(SPI_SCK_GPIO_Port, SPI_SCK_Pin, GPIO_PIN_SET)
#define SCK_LOW			HAL_GPIO_WritePin(SPI_SCK_GPIO_Port, SPI_SCK_Pin, GPIO_PIN_RESET)
#define MOSI_HIGH		HAL_GPIO_WritePin(SPI_DIN_GPIO_Port, SPI_DIN_Pin, GPIO_PIN_SET)
#define MOSI_LOW		HAL_GPIO_WritePin(SPI_DIN_GPIO_Port, SPI_DIN_Pin, GPIO_PIN_RESET)
#define MISO_PIN		HAL_GPIO_ReadPin(SPI_DOUT_GPIO_Port, SPI_DOUT_Pin)

#define	NSS_HIGH		HAL_GPIO_WritePin(SP1_NSS_GPIO_Port, SP1_NSS_Pin, GPIO_PIN_SET)
#define	NSS_LOW			HAL_GPIO_WritePin(SP1_NSS_GPIO_Port, SP1_NSS_Pin, GPIO_PIN_RESET)

enum {RUNSTOP_KEY = 0, AUTO_KEY, RESERVE_KEY, STER_KEY, SMOKE_KEY, UP_KEY, DOWN_KEY};
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
