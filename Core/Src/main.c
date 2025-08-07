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
#include "Driver/drv8904q1.h"
#include "spi.h"

uint32_t button_press_timestamp[8] = {0};
volatile bool effect_active = false;
static bool any_button_pressed = false;
volatile uint8_t touch_event_count = 0;
bool hold_effect_active[8] = {0};
volatile bool hold_chase_effect_active = false;
static uint32_t chase_start_timestamp = 0;
static bool double_beep_played = false;

void SystemClock_Config(void);
void set_leds_solid_green(void);
void handle_touch_events(void);

void set_leds_solid_green(void) {
    effect_params.hue = 85;
    effect_params.brightness = 50;
    led_effect_engine_set(LED_EFFECT_SOLID);
    effect_active = false;
}

void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin) {
    if (GPIO_Pin == GPIO_PIN_1) {
        touch_event_count++;
    }
}

void handle_touch_events(void) {
    while (touch_event_count > 0) {
        touch_event_count--;
        uint32_t start = HAL_GetTick();
        while (HAL_I2C_IsDeviceReady(&hi2c1, CY8CMBR3108_I2C_ADDR, 1, 2) != HAL_OK) {
            if ((HAL_GetTick() - start) > 10) return;
        }

        uint8_t status = cy8cmbr3108_read_button_stat();
        bool now_pressed = (status & BUTTON_MASK) != 0;

        // Effekt nur auslösen, wenn vorher keine Taste gedrückt war und jetzt mindestens eine.
        if (!any_button_pressed && now_pressed) {
            for (int i = 0; i < 8; ++i) {
                uint8_t mask = (1 << i);
                if ((BUTTON_MASK & mask) && (status & mask)) {
                    button_press_timestamp[i] = HAL_GetTick();
                    sound_single_sweep_1_start();

                    switch(i) {
                        case 0: effect_params.hue = 0; break;
                        case 1: effect_params.hue = 170; break;
                        case 5: effect_params.hue = 213; break;
                        case 6: effect_params.hue = 14; break;
                        default: effect_params.hue = 85; break;
                    }

                    effect_params.brightness = 255;
                    led_effect_multibutton_double_blink_start(effect_params.hue, effect_params.brightness);
                    effect_active = true;
                    hold_effect_active[i] = false;
                }
            }
        }

        // --- PATCH: Effekt-Flag nach Doppelblink zurücksetzen ---
        // Prüfe, ob der Doppelblink vorbei ist und setze effect_active zurück
        if (effect_active && !led_effect_multibutton_double_blink_is_active()) {
            effect_active = false;
        }
        // --------------------------------------------------------

        // Nach dem Doppelblink, prüfe ob Taste gehalten wird und Hold-Effekt starten
        for (int i = 0; i < 8; ++i) {
            uint8_t mask = (1 << i);
            if (!effect_active && !hold_effect_active[i] && (status & mask)) {
                uint8_t hue;
                switch(i) {
                    case 0: hue = 0; break;
                    case 1: hue = 170; break;
                    case 5: hue = 213; break;
                    case 6: hue = 14; break;
                    default: hue = 85; break;
                }
                uint8_t brightness = 255;
                effect_params.speed = 5;
                led_effect_hold_multibutton_chase_left_start(hue, brightness);
                chase_start_timestamp = HAL_GetTick();
                double_beep_played = false;
                hold_effect_active[i] = true;
                // hold_chase_effect_active = true; // --- PATCH: Flag explizit setzen
            }
        }

        any_button_pressed = now_pressed;

        for (int i = 0; i < 8; ++i) {
            uint8_t mask = (1 << i);
            if ((BUTTON_MASK & mask) && !(status & mask)) {
                button_press_timestamp[i] = 0;
            }
        }
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_TIM3_Init();
    MX_TIM14_Init();
    MX_I2C1_Init();

    cy8cmbr3108_init(); // <-- Initialisierung des Touchcontrollers

    sound_engine_init();
    led_effect_engine_init();
    drv8904q1_init();

    set_leds_solid_green();

    while (1) {
        handle_touch_events();  // <- unbedingt nötig!

        sound_engine_tick();
        sound_beep_update();
        sound_single_sweep_1_update();

        if (!led_effect_hold_multibutton_chase_left_is_active()) {
            led_effect_engine_update(HAL_GetTick());
        }

        if (led_effect_hold_multibutton_chase_left_is_active() && !double_beep_played) {
            if (HAL_GetTick() - chase_start_timestamp >= 2000) {
                sound_double_beep_start(4000, 80, 50);
                led_effect_multibutton_double_blink_start(effect_params.hue, effect_params.brightness);
                led_effect_hold_multibutton_chase_left_stop();
                for (int i = 0; i < 8; ++i) hold_effect_active[i] = false;
                double_beep_played = true;
                drv8904q1_set_outputs(0, 0);
            }
        }

        bool hold_active = false;
        for (int i = 0; i < 8; ++i) {
            if (hold_effect_active[i]) {
                hold_active = true;
                break;
            }
        }

        // Rückfall auf grün:
        if (!effect_active && !any_button_pressed && !hold_active && !led_effect_hold_multibutton_chase_left_is_active()) {
            set_leds_solid_green();
            double_beep_played = false;
        }

        if (sound_double_beep_is_active()) {
            // Doppelbeep läuft noch
        }

        if (sound_beep_is_active()) {
            // Einfacher Beep läuft noch
        }
    }
}

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

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1;
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
