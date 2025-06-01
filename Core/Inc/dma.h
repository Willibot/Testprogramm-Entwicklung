/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dma.h
  * @brief   This file contains all the function prototypes for
  *          the dma.c file
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
#ifndef __DMA_H__
#define __DMA_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* DMA memory to memory transfer handles -------------------------------------*/

/* USER CODE BEGIN Includes */
// Copilot/Entwickler-Hinweis:
// - Diese Header-Datei wird von STM32CubeMX generiert und enthält die Prototypen für die DMA-Initialisierung.
// - Eigene DMA-Callback-Prototypen oder spezielle Handler können in den USER CODE-Bereichen ergänzt werden.
// - Für LED-Effekte: DMA1_Channel1 wird für die Übertragung der PWM-Daten an den LED-Ring genutzt.
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_DMA_Init(void);

/* USER CODE BEGIN Prototypes */
// Eigene DMA-Callback-Prototypen oder spezielle Handler können hier ergänzt werden.
// Beispiel: void DMA1_Channel1_IRQHandler(void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __DMA_H__ */

