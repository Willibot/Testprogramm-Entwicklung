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
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "config.h"
#include "effects/led_effect_engine.h"
#include "sounds/sound_engine.h"
#include "sounds/sound_beep.h"
#include "sounds/piezo_driver.h"
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
// Status der Ausgänge (z.B. Relais oder LEDs)
bool output_state[3] = {0};
// Zähler für Zeitmessungen (z.B. für Effekte)
uint32_t timer_tick = 0;
// Wird vom Interrupt gesetzt, wenn ein Touch-Event erkannt wurde
volatile bool touch_event_pending = false;
// Zeitstempel für jede Taste (CS0-CS7), um die Haltedauer zu messen
uint32_t button_press_timestamp[8] = {0};
// Status der zuletzt gelesenen Latch-Register (optional)
volatile uint8_t latched_status = 0;
// Zeit, wann der aktuelle Effekt enden soll
volatile uint32_t effect_end_time = 0;
// Ist gerade ein Effekt aktiv?
volatile bool effect_active = false;
// Ganz oben (bei den anderen globalen Variablen)
static bool any_button_pressed = false;
// Zähler für die Anzahl der Touch-Events
volatile uint8_t touch_event_count = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void set_leds_solid_green(void);
void handle_touch_events(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Effektsteuerung für LED-Blinken bei Interrupt

// Setzt alle LEDs auf ein dauerhaftes Grün mit reduzierter Helligkeit
void set_leds_solid_green(void) {
    effect_params.hue = 85; // Farbwert für Grün
    effect_params.brightness = 50; // Geringe Helligkeit
    led_effect_engine_set(LED_EFFECT_SOLID);
    effect_active = false;
}

// Diese Funktion wird automatisch aufgerufen, wenn am INT-Pin (PA1) ein
// fallender Flankenwechsel erkannt wird (Interrupt vom CY8CMBR3108).
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_1)
    {
        touch_event_count++;
    }
}

// Diese Funktion übernimmt die komplette Tasten- und Haltedauer-Logik.
// Sie wird im Mainloop regelmäßig aufgerufen.
void handle_touch_events(void)
{
    while (touch_event_count > 0) {
        touch_event_count--;

        // Warte, bis der CY8CMBR3108 auf I2C-Anfragen antwortet (ACK gibt)
        uint32_t start = HAL_GetTick();
        while (HAL_I2C_IsDeviceReady(&hi2c1, CY8CMBR3108_I2C_ADDR, 1, 2) != HAL_OK) {
            if ((HAL_GetTick() - start) > 50) return;
        }

        // Status-Register (0xAA) lesen: zeigt, welche Tasten aktuell gedrückt sind
        uint8_t status = cy8cmbr3108_read_button_stat();

        // Prüfen, ob aktuell mindestens eine Taste gedrückt ist
        bool now_pressed = (status & BUTTON_MASK) != 0;

        // Effekt nur auslösen, wenn vorher keine Taste gedrückt war und jetzt mindestens eine
        if (!any_button_pressed && now_pressed) {
            // Effekt für alle aktuell gedrückten Tasten auslösen
            for (int i = 0; i < 8; ++i) {
                uint8_t mask = (1 << i);
                if ((BUTTON_MASK & mask) && (status & mask)) {
                    button_press_timestamp[i] = HAL_GetTick();
                    sound_engine_play(SOUND_BEEP);

                    switch(i) {
                        case 0: effect_params.hue = 0; break;      // Rot
                        case 1: effect_params.hue = 170; break;    // Blau
                        case 5: effect_params.hue = 213; break;    // Magenta
                        case 6: effect_params.hue = 42; break;     // Gelb
                        default: effect_params.hue = 85; break;    // Grün
                    }

                    effect_params.brightness = 255;
                    led_effect_engine_set(LED_EFFECT_BLINK);
                    effect_active = true;
                    effect_end_time = HAL_GetTick() + 500;
                }
            }
        }

        // any_button_pressed für die nächste Runde merken
        any_button_pressed = now_pressed;

        // Optional: Zeitmessung für Long-Press oder Loslassen zurücksetzen
        for (int i = 0; i < 8; ++i) {
            uint8_t mask = (1 << i);
            if ((BUTTON_MASK & mask) && !(status & mask)) {
                button_press_timestamp[i] = 0;
            }
        }
    }
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM3_Init();
    MX_TIM14_Init();
    MX_I2C1_Init();

    HAL_Delay(200); // Warten nach Power-On

    if (HAL_I2C_IsDeviceReady(&hi2c1, 0x6E, 2, 100) != HAL_OK) {
        Error_Handler();
    }

    // Schreibe komplette Konfiguration (alle gültigen Register)
    #if USE_I2C_CY8CMBR3108_WRITE
    if (cy8cmbr3108_write_config() != HAL_OK) {
        Error_Handler();
    }
    #endif

    sound_engine_init();
    led_effect_engine_init();

    set_leds_solid_green();

    while (1)
    {
        sound_engine_tick();
        sound_beep_update();
        led_effect_engine_update(HAL_GetTick());

        if (effect_active && HAL_GetTick() > effect_end_time)
        {
            set_leds_solid_green();
        }

        handle_touch_events();
    }
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
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
