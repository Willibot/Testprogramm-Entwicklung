/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.c
  * @brief   This file provides code for the configuration
  *          of all the requested memory to memory DMA transfers.
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
#include "dma.h"

/* USER CODE BEGIN 0 */
// Copilot/Entwickler-Hinweis:
// - Diese Datei wird von STM32CubeMX generiert und enthält die Initialisierung der DMA-Peripherie.
// - Für LED-Effekte: DMA1_Channel1 wird für die Übertragung der PWM-Daten an den LED-Ring (SK6812/WS2812) genutzt.
// - Änderungen an der DMA-Konfiguration sollten bevorzugt in CubeMX erfolgen, damit sie nicht überschrieben werden.
// - Eigene DMA-Interrupt-Handler oder Callback-Funktionen können in den USER CODE-Bereichen ergänzt werden.
// - Bei Problemen mit LED-Flackern oder Timing: Prüfe DMA-Konfiguration und Interrupt-Prioritäten!
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure DMA                                                              */
/*----------------------------------------------------------------------------*/

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
  * Enable DMA controller clock
  */
void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

}

/* USER CODE BEGIN 2 */
// Eigene DMA-Interrupt-Handler oder Callback-Funktionen können hier ergänzt werden.
// Beispiel: void DMA1_Channel1_IRQHandler(void) { ... }
/* USER CODE END 2 */

