/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32g0xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */
// Copilot/Entwickler-Hinweis:
// - Diese Datei wird von STM32CubeMX generiert und enthält die Initialisierung und De-Initialisierung der Low-Level-Peripherie (MSP).
// - Hier werden z.B. Clock-Enable, NVIC-Interrupts und spezielle Hardware-Initialisierungen vorgenommen.
// - Eigene Anpassungen (z.B. spezielle Interrupt-Prioritäten, zusätzliche Peripherie) können in den USER CODE-Bereichen ergänzt werden.
// - Änderungen außerhalb der USER CODE-Bereiche werden beim nächsten CubeMX-Export überschrieben!
/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */
void HAL_MspInit(void)
{

  /* USER CODE BEGIN MspInit 0 */

  /* USER CODE END MspInit 0 */

  __HAL_RCC_SYSCFG_CLK_ENABLE();
  __HAL_RCC_PWR_CLK_ENABLE();

  /* System interrupt init*/

  /* USER CODE BEGIN MspInit 1 */
  // Eigene globale Initialisierungen, z.B. NVIC-Prioritäten, können hier ergänzt werden.
  /* USER CODE END MspInit 1 */
}

/* USER CODE BEGIN 1 */
// Eigene MSP-Initialisierungen für spezielle Peripherie können hier ergänzt werden.
// Beispiel: Anpassung von Interrupt-Prioritäten für DMA, Timer, etc.
/* USER CODE END 1 */
