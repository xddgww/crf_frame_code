/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32g4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CURTAIN_LPUART1_TX_Pin GPIO_PIN_2
#define CURTAIN_LPUART1_TX_GPIO_Port GPIOA
#define CURTAIN_LPUART1_RX_Pin GPIO_PIN_3
#define CURTAIN_LPUART1_RX_GPIO_Port GPIOA
#define CURTAIN_LPUART1_DE_Pin GPIO_PIN_1
#define CURTAIN_LPUART1_DE_GPIO_Port GPIOB
#define PC_USART_TX_Pin GPIO_PIN_10
#define PC_USART_TX_GPIO_Port GPIOB
#define PC_USART_RX_Pin GPIO_PIN_11
#define PC_USART_RX_GPIO_Port GPIOB
#define CURTAIN_UART4_DE_Pin GPIO_PIN_15
#define CURTAIN_UART4_DE_GPIO_Port GPIOA
#define CURTAIN_UART4_TX_Pin GPIO_PIN_10
#define CURTAIN_UART4_TX_GPIO_Port GPIOC
#define CURTAIN_UART4_RX_Pin GPIO_PIN_11
#define CURTAIN_UART4_RX_GPIO_Port GPIOC
#define BUZZER_TIM8_CH2_Pin GPIO_PIN_8
#define BUZZER_TIM8_CH2_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
