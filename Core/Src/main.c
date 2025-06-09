/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (Sensortaster-Projekt)
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"

/* USER CODE BEGIN Includes */
#include "config.h"
#include "led_effect_engine.h"   // Effekt-Engine für LED-Effekte
#include "sound_engine.h"      // Noch nicht benötigt für ersten Test
//#include "logic_engine.h"      // Noch nicht benötigt für ersten Test
#include "piezo_driver.h"      // Noch nicht benötigt für ersten Test
/* USER CODE END Includes */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

/* USER CODE BEGIN 0 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    // PA1 = GPIO_PIN_1 = EXTI1: Interrupt vom CY8CMBR3108 (Touch-Sensor)
    if(GPIO_Pin == GPIO_PIN_1)
    {
        sound_engine_play(SOUND_BEEP); // Kurzer Piezo-Beep
    }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  // Ziel: Nur den ersten LED-Effekt (solid) testen, um DMA, Timer und LED-Treiber zu prüfen.
  /* USER CODE END 1 */

  HAL_Init();                // Reset aller Peripherie, Flash-Interface und Systick

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  SystemClock_Config();      // Systemtakt konfigurieren (CubeMX-Standard)

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  MX_GPIO_Init();            // GPIOs initialisieren
  MX_DMA_Init();             // DMA initialisieren (wichtig für SK6812)
  MX_TIM3_Init();            // Timer 3 für PWM (SK6812) initialisieren

  // --- Flanke für EXTI1 (PA1) auf "Falling Edge" setzen ---
  HAL_NVIC_DisableIRQ(EXTI0_1_IRQn); // Sicherheitshalber Interrupt kurz deaktivieren
  EXTI->FTSR1 |= EXTI_FTSR1_FT1;     // Falling Trigger für Line 1 aktivieren
  EXTI->RTSR1 &= ~EXTI_RTSR1_RT1;    // Rising Trigger für Line 1 deaktivieren
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);  // Interrupt wieder aktivieren

  /* USER CODE BEGIN 2 */
  led_effect_engine_init();  // Initialisiert die Effekt-Engine und ggf. den LED-Treiber

  // Starte den ersten Effekt (z.B. solid)
  led_effect_engine_set(LED_EFFECT_SOLID);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    // Effekt-Update aufrufen (führt den aktuellen Effekt aus)
    led_effect_engine_update();

    // Optional: kleine Pause, damit die CPU nicht 100% läuft
    HAL_Delay(1);

    // Noch keine Logik, Sound oder weitere Features – Fokus: LED-Test!
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 8;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Error Handler
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
    // Fehleranzeige z.B. per LED oder Piezo (hier Endlosschleife)
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  // Optional: Fehlerprotokollierung
}
#endif

/*
 * Hinweise:
 * - Diese main.c ist für den allerersten LED-Effekt-Test ausgelegt.
 * - Es werden nur die für den LED-Test nötigen Initialisierungen und Funktionen aufgerufen.
 * - Wenn der Effekt funktioniert, können Sound, Logik und weitere Features schrittweise ergänzt werden.
 * - Die Effekt-Engine muss so implementiert sein, dass sie mit led_effect_engine_set(LED_EFFECT_SOLID) den gewünschten Effekt startet.
 */
