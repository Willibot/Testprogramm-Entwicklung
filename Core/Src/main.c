#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"
#include "config.h"
#include "effects/led_effect_engine.h"
#include "effects/led_effect_multibutton_double_blink.h"
#include "sounds/sound_engine.h"
#include "sounds/sound_beep.h"
#include "sounds/sound_double_beep.h"
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"
#include "Driver/drv8904q1.h"

// ------------------- Konfiguration -------------------
#define HOLD_THRESHOLD_MS 2000

// Mapping Taste → OUT1, OUT2
static const uint8_t out_states[4][2] = {
    {1, 0}, // T0
    {0, 1}, // T1
    {1, 1}, // T2
    {0, 0}  // T3
};

// Farben je Taste (Hue)
static const uint8_t hue_map[4] = {0, 170, 213, 85};

// ------------------- Globale Variablen -------------------
uint32_t button_press_timestamp[8] = {0};
volatile bool any_button_pressed = false;
volatile uint8_t touch_event_count = 0;

// ------------------- Funktionsprototypen -------------------
void SystemClock_Config(void);
void set_leds_solid_green(void);
void handle_touch_events(void);

// ------------------- Hilfsfunktionen -------------------
void set_leds_solid_green(void) {
    effect_params.hue = 85; // Grün
    effect_params.brightness = 50;
    led_effect_engine_set(LED_EFFECT_SOLID);
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_1) {
        touch_event_count++;
    }
}

void handle_touch_events(void) {
    while (touch_event_count > 0) {
        touch_event_count--;

        // Warten auf CY8CMBR3108
        uint32_t start = HAL_GetTick();
        while (HAL_I2C_IsDeviceReady(&hi2c1, CY8CMBR3108_I2C_ADDR, 1, 2) != HAL_OK) {
            if ((HAL_GetTick() - start) > 10) return;
        }

        uint8_t status = cy8cmbr3108_read_button_stat();
        bool now_pressed = (status & BUTTON_MASK) != 0;

        // Neuer Tastendruck
        if (!any_button_pressed && now_pressed) {
            for (int i = 0; i < 4; ++i) { // nur T0..T3
                uint8_t mask = (1 << i);
                if ((BUTTON_MASK & mask) && (status & mask)) {
                    button_press_timestamp[i] = HAL_GetTick();

                    // Ausgänge setzen
                    drv8904q1_set_output(1, out_states[i][0]);
                    drv8904q1_set_output(2, out_states[i][1]);

                    // LED in Tastenfarbe
                    effect_params.hue = hue_map[i];
                    effect_params.brightness = 255;
                    led_effect_engine_set(LED_EFFECT_SOLID);

                    // Kurzer Beep
                    sound_beep_start();
                }
            }
        }

        // Prüfen auf langes Halten
        for (int i = 0; i < 4; ++i) {
            uint8_t mask = (1 << i);
            if (button_press_timestamp[i] &&
                (status & mask) &&
                (HAL_GetTick() - button_press_timestamp[i] >= HOLD_THRESHOLD_MS)) {

                // Doppelbeep + Doppelblink
                sound_double_beep_start(4000, 80, 50);
                led_effect_multibutton_double_blink_start(hue_map[i], 255);

                // Ausgänge deaktivieren
                drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, 0x0000);

                // LEDs zurück auf grün
                HAL_Delay(200);
                set_leds_solid_green();

                button_press_timestamp[i] = 0;
            }
        }

        any_button_pressed = now_pressed;

        // Loslassen → Timestamp zurücksetzen
        for (int i = 0; i < 4; ++i) {
            uint8_t mask = (1 << i);
            if ((BUTTON_MASK & mask) && !(status & mask)) {
                button_press_timestamp[i] = 0;
            }
        }
    }
}

// ------------------- Main -------------------
int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM3_Init();
    MX_TIM14_Init();
    MX_I2C1_Init();
    MX_SPI1_Init(); // für DRV8904Q1

    HAL_Delay(200);

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
    drv8904q1_init();

    set_leds_solid_green();

    while (1) {
        sound_engine_tick();
        sound_beep_update();
        sound_double_beep_update();

        led_effect_engine_update(HAL_GetTick());

        handle_touch_events();
    }
}

// ------------------- System Clock -------------------
void SystemClock_Config(void) {
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
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                |RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {}
}
