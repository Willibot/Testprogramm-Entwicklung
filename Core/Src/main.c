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
#include "effects/led_effect_multibutton_double_blink.h"
#include "effects/led_effect_hold_multibutton_chase_left.h"
#include "sounds/sound_engine.h"
#include "sounds/sound_beep.h"
#include "sounds/piezo_driver.h"
#include "sounds/sound_single_sweep_1.h"
#include "sounds/sound_double_beep.h"
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"

// Nur Deklaration, keine Definition mehr hier!
extern effect_params_t effect_params;

/* Private variables ---------------------------------------------------------*/
// Zeitstempel für jede Taste (CS0-CS7), um die Haltedauer zu messen
uint32_t button_press_timestamp[8] = {0};
// Ist gerade ein Effekt aktiv?
volatile bool effect_active = false;
// Merkt, ob aktuell mindestens eine Taste gedrückt ist
static bool any_button_pressed = false;
// Zähler für die Anzahl der Touch-Events
volatile uint8_t touch_event_count = 0;
// Merkt, ob Hold-Effekt pro Taste läuft
bool hold_effect_active[8] = {0};
volatile bool hold_chase_effect_active = false; // Globale Variable für Hold-Chase-Effekt
static uint32_t chase_start_timestamp = 0;
static bool double_beep_played = false;

// Annahmen: 
// - touch_active: true, solange Finger auf Sensor
// - touch_start_time: Zeitstempel bei Touch-Down
// - chase_started: true, wenn Chase-Effekt läuft
// - long_press_handled: true, wenn langer Tastendruck bereits behandelt wurde
bool touch_active = false;
uint32_t touch_start_time = 0;
bool chase_started = false;
bool long_press_handled = false;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void set_leds_solid_green(void);
void handle_touch_events(void);

/* Setzt alle LEDs auf ein dauerhaftes Grün mit reduzierter Helligkeit */
void set_leds_solid_green(void) {
    effect_params.hue = 85; // Grün
    effect_params.brightness = 50; // Reduzierte Helligkeit
    led_effect_engine_set(LED_EFFECT_SOLID);
    effect_active = false;
}

/* Interrupt-Callback für Touch-Events */
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_1)
    {
        touch_event_count++;
    }
}

/* Tasten- und Effekt-Logik */
void handle_touch_events(void)
{
    while (touch_event_count > 0) {
        touch_event_count--;

        // Warte, bis der CY8CMBR3108 auf I2C-Anfragen antwortet (ACK gibt)
        uint32_t start = HAL_GetTick();
        while (HAL_I2C_IsDeviceReady(&hi2c1, CY8CMBR3108_I2C_ADDR, 1, 2) != HAL_OK) {
            if ((HAL_GetTick() - start) > 10) {
                return;
            }
        }

        uint8_t status = cy8cmbr3108_read_button_stat();
        bool now_pressed = (status & BUTTON_MASK) != 0;

        // ----------- NEUER TASTENSTART -----------
        if (!any_button_pressed && now_pressed) {
            for (int i = 0; i < 8; ++i) {
                uint8_t mask = (1 << i);
                if ((BUTTON_MASK & mask) && (status & mask)) {
                    button_press_timestamp[i] = HAL_GetTick();
                    sound_beep_start();

                    // Farbwahl je Taste
                    switch (i) {
                        case 0: effect_params.hue = 0; break;
                        case 1: effect_params.hue = 170; break;
                        case 5: effect_params.hue = 213; break;
                        case 6: effect_params.hue = 14; break;
                        default: effect_params.hue = 85; break;
                    }
                    effect_params.brightness = 255;

                    led_effect_multibutton_double_blink_start(effect_params.hue, effect_params.brightness);
                    effect_active = true;
                    for (int j = 0; j < 8; ++j) hold_effect_active[j] = false;
                }
            }
        }

        // ----------- CHASE-START NACH DOPPELBLINK (normale Variante) -----------
        if (!effect_active && !hold_chase_effect_active && now_pressed) {
            for (int i = 0; i < 8; ++i) {
                uint8_t mask = (1 << i);
                if ((status & mask) && !hold_effect_active[i]) {
                    uint8_t hue;
                    switch (i) {
                        case 0: hue = 0; break;
                        case 1: hue = 170; break;
                        case 5: hue = 213; break;
                        case 6: hue = 14; break;
                        default: hue = 85; break;
                    }
                    uint8_t brightness = 255;
                    effect_params.speed = 5; // Standardgeschwindigkeit

                    led_effect_hold_multibutton_chase_left_start(hue, brightness);
                    chase_start_timestamp = HAL_GetTick();
                    double_beep_played = false;
                    for (int j = 0; j < 8; ++j) hold_effect_active[j] = false;
                    hold_effect_active[i] = true;
                    break;
                }
            }
        }

        // ----------- CHASE-STOPP-BEDINGUNGEN (robust) -----------
        if (hold_chase_effect_active) {
            int active_index = -1;
            for (int i = 0; i < 8; ++i) {
                if (hold_effect_active[i]) {
                    active_index = i;
                    break;
                }
            }

            bool any_held = false;
            int held_count = 0;
            for (int i = 0; i < 8; ++i) {
                uint8_t mask = (1 << i);
                if ((BUTTON_MASK & mask) && (status & mask)) {
                    any_held = true;
                    held_count++;
                }
            }

            // Sofortiger Stopp bei Mehrfachtouch
            if (held_count > 1) {
                led_effect_hold_multibutton_chase_left_stop();
                for (int i = 0; i < 8; ++i) hold_effect_active[i] = false;
            }
            // Stopp wenn keine Taste mehr gehalten wird
            else if (!any_held) {
                led_effect_hold_multibutton_chase_left_stop();
                for (int i = 0; i < 8; ++i) hold_effect_active[i] = false;
            }
            // Stopp wenn andere Taste als die ursprüngliche aktiv
            else if (active_index >= 0) {
                uint8_t mask = (1 << active_index);
                if (!(status & mask)) {
                    led_effect_hold_multibutton_chase_left_stop();
                    for (int i = 0; i < 8; ++i) hold_effect_active[i] = false;
                }
            }
        }

        any_button_pressed = now_pressed;

        // Zeitmessung für Loslassen zurücksetzen
        for (int i = 0; i < 8; ++i) {
            uint8_t mask = (1 << i);
            if ((BUTTON_MASK & mask) && !(status & mask)) {
                button_press_timestamp[i] = 0;
            }
        }
    }

    // ----------- ZUSATZ: Chase-Start ohne neues Event -----------
    // Nur wenn Doppelblink definitiv vorbei ist (!effect_active)
    if (!effect_active && !hold_chase_effect_active && any_button_pressed) {
        uint8_t status = cy8cmbr3108_read_button_stat();
        for (int i = 0; i < 8; ++i) {
            uint8_t mask = (1 << i);
            if ((BUTTON_MASK & mask) && (status & mask)) {
                uint8_t hue;
                switch (i) {
                    case 0: hue = 0; break;
                    case 1: hue = 170; break;
                    case 5: hue = 213; break;
                    case 6: hue = 14; break;
                    default: hue = 85; break;
                }
                uint8_t brightness = 255;
                effect_params.speed = 5; // Standardgeschwindigkeit

                led_effect_hold_multibutton_chase_left_start(hue, brightness);
                chase_start_timestamp = HAL_GetTick();
                double_beep_played = false;
                for (int j = 0; j < 8; ++j) hold_effect_active[j] = false;
                hold_effect_active[i] = true;
                break;
            }
        }
    }

    if (touch_active) {
        uint32_t duration = HAL_GetTick() - touch_start_time;

        // Nach 500ms: Chase-Effekt starten
        if (!chase_started && duration >= 500) {
            led_effect_hold_multibutton_chase_left_start(effect_params.hue, effect_params.brightness);
            chase_started = true;
        }

        // Nach 2500ms: Schaltvorgang, Rückmeldung, Rückfall
        if (!long_press_handled && duration >= 2500) {
            sound_double_beep_start(4000, 80, 50);
            led_effect_multibutton_double_blink_start(effect_params.hue, effect_params.brightness);
            // setDRVOutputsToHighZ(); // (später einfügen)
            led_effect_hold_multibutton_chase_left_stop();
            set_leds_solid_green();
            chase_started = false;
            long_press_handled = true;
            touch_active = false; // Rückfall in Eingabeerwartung
            // ggf. weitere Flags zurücksetzen
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

    #if USE_I2C_CY8CMBR3108_WRITE
    if (cy8cmbr3108_write_config() != HAL_OK) {
        Error_Handler();
    }
    #endif

    sound_engine_init();
    led_effect_engine_init();

    // LEDs beim Start auf grün mit reduzierter Helligkeit setzen
    set_leds_solid_green();

    while (1)
    {
        sound_engine_tick();
        sound_beep_update();
        sound_double_beep_update();

        // Effekt-Engine nur updaten, wenn kein Hold-Chase-Effekt läuft!
        if (!hold_chase_effect_active) {
            led_effect_engine_update(HAL_GetTick());
        }

        if (effect_active) {
            led_effect_multibutton_double_blink_update(HAL_GetTick());
        }

        led_effect_hold_multibutton_chase_left_update(HAL_GetTick());
        handle_touch_events();

        // Rückfall auf grün, wenn kein Effekt und keine Taste aktiv
        bool hold_active = false;
        for (int i = 0; i < 8; ++i) {
            if (hold_effect_active[i]) {
                hold_active = true;
                break;
            }
        }
        if (!effect_active && !any_button_pressed && !hold_active && !hold_chase_effect_active) {
            set_leds_solid_green();
        }

        if (hold_chase_effect_active && !double_beep_played) {
            uint32_t chase_duration = HAL_GetTick() - chase_start_timestamp;

            // Nach 2000ms: Doppelbeep, Doppelblink, Effekt stoppen, Rückfall
            if (chase_duration >= 2000) {
                sound_double_beep_start(4000, 80, 50);
                led_effect_multibutton_double_blink_start(effect_params.hue, effect_params.brightness);

                led_effect_hold_multibutton_chase_left_stop();
                for (int i = 0; i < 8; ++i) hold_effect_active[i] = false;
                hold_chase_effect_active = false;
                double_beep_played = false; // ggf. zurücksetzen, falls du das Flag noch brauchst

                set_leds_solid_green();

                // Rückfall in Eingabeerwartung erzwingen:
                any_button_pressed = false;
                touch_active = false;
                chase_started = false;
                long_press_handled = false;
                // ggf. weitere Flags zurücksetzen
            }
            // Effektabbruch, falls Finger vorher entfernt wird (wie gehabt)
            else {
                // ...deine bestehende Logik für Loslassen...
            }
        }
    }
}

/**
  * @brief System Clock Configuration (STM32G0, CubeMX-Standard)
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

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  // Für STM32G0: FLASH_LATENCY_2 verwenden!
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
  __disable_irq();
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  /* Optional: Fehlerausgabe für Debug */
}
#endif /* USE_FULL_ASSERT */
