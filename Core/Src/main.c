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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "config.h"
#include "effects/led_effect_engine.h"
#include "sounds/sound_engine.h"
#include "sounds/sound_beep.h"
#include "sounds/piezo_driver.h"
#include "Driver/cy8cmbr3108.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
bool output_state[3] = {0};
uint32_t timer_tick = 0;
volatile bool touch_event_pending = false; // Flag für neuen Tastendruck
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Effektsteuerung für LED-Blinken bei Interrupt
volatile uint32_t effect_end_time = 0;
volatile bool effect_active = false;

// Zentrale Funktion für Grundzustand (solid green, reduzierte Helligkeit)
void set_leds_solid_green(void) {
    effect_params.hue = 85; // Grün
    effect_params.brightness = 50; // 1/5 Helligkeit, anpassbar
    led_effect_engine_set(LED_EFFECT_SOLID);
    effect_active = false;
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_1)
    {
        touch_event_pending = true; // Nur das Flag setzen!
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

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  // Test: EXTI1 explizit auf Port A routen (nur zum Test)
  // SYSCFG->EXTICR[0] &= ~(0xF << 4); // Entfernt, CubeMX übernimmt das Routing
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_TIM14_Init();
  led_driver_init();
  /* USER CODE BEGIN 2 */
  sound_engine_init();
  led_effect_engine_init();

  // Start mit solid green
  set_leds_solid_green();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    sound_engine_tick();
    sound_beep_update();
    led_effect_engine_update(HAL_GetTick());

    // Nach 0,5s zurück zu grün
    if (effect_active && HAL_GetTick() > effect_end_time)
    {
        set_leds_solid_green();
    }

    // I2C-Status nur außerhalb des Interrupts abfragen!
    if (touch_event_pending && !effect_active)
    {
        touch_event_pending = false;
        uint8_t status = cy8cmbr3108_read_sensor_status();

        // Taste 1: Rot, 2: Blau, 3: Magenta, 4: Orange
        if (status & 0x01) { // Taste 1
            effect_params.hue = 0;    // Rot
        } else if (status & 0x02) { // Taste 2
            effect_params.hue = 170;  // Blau
        } else if (status & 0x04) { // Taste 3
            effect_params.hue = 213;  // Magenta
        } else if (status & 0x08) { // Taste 4
            effect_params.hue = 25;   // Orange
        } else {
            continue; // Keine Taste gedrückt
        }

        sound_engine_play(SOUND_BEEP);
        led_effect_engine_set(LED_EFFECT_BLINK);
        effect_params.brightness = 255;
        effect_params.speed = 137;
        effect_active = true;
        effect_end_time = HAL_GetTick() + 500;
    }
    // ... weitere zyklische Funktionen ...
}
/* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
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

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/* KEINE led_effect_solid_start hier! */
