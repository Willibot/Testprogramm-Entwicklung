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
#include "sounds/sound_double_beep.h"
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"
#include "Driver/drv8904q1.h"

// === Globale Statusvariablen ===
uint32_t button_press_timestamp[8] = {0};
bool hold_effect_active[8] = {0};
volatile bool hold_chase_effect_active = false;
static uint32_t chase_start_timestamp = 0;
volatile bool effect_active = false;
volatile uint8_t touch_event_count = 0;
bool any_button_pressed = false;
bool double_beep_played = false;

// === Tastenbelegung ===
static const uint8_t valid_buttons[4] = {0, 1, 5, 6};
static const uint8_t button_hue[8] = {
    [0] = 0,     // Rot
    [1] = 170,   // Blau
    [5] = 213,   // Lila
    [6] = 14     // Orange
};

static const uint8_t drv_out_states[8][2] = {
    [0] = {1, 0},
    [1] = {0, 1},
    [5] = {1, 1},
    [6] = {0, 0},
};

void set_leds_solid_green(void) {
    effect_params.hue = 85; // grün
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

        if (HAL_I2C_IsDeviceReady(&hi2c1, CY8CMBR3108_I2C_ADDR, 1, 2) != HAL_OK) return;

        uint8_t status = cy8cmbr3108_read_button_stat();
        bool now_pressed = (status & BUTTON_MASK) != 0;

        // Neuer Tastendruck
        if (!any_button_pressed && now_pressed) {
            for (int j = 0; j < 4; ++j) {
                int i = valid_buttons[j];
                if (status & (1 << i)) {
                    button_press_timestamp[i] = HAL_GetTick();

                    // Outputs setzen
                    drv8904q1_set_outputs(drv_out_states[i][0], drv_out_states[i][1]);

                    // Beep + Blink + LED-Farbe
                    sound_beep_start();
                    effect_params.hue = button_hue[i];
                    effect_params.brightness = 255;
                    led_effect_multibutton_double_blink_start(effect_params.hue, effect_params.brightness);
                    effect_active = true;
                    for (int k = 0; k < 8; ++k) hold_effect_active[k] = false;
                }
            }
        }

        any_button_pressed = now_pressed;

        // Taste losgelassen → Zeitstempel löschen
        for (int i = 0; i < 8; ++i) {
            if (!(status & (1 << i))) {
                button_press_timestamp[i] = 0;
            }
        }
    }

    // Chase starten wenn Effekt vorbei und Taste noch gehalten wird
    if (!effect_active && !hold_chase_effect_active && any_button_pressed) {
        uint8_t status = cy8cmbr3108_read_button_stat();
        for (int j = 0; j < 4; ++j) {
            int i = valid_buttons[j];
            if ((status & (1 << i)) && !hold_effect_active[i]) {
                led_effect_hold_multibutton_chase_left_start(button_hue[i], 255);
                hold_chase_effect_active = true;
                chase_start_timestamp = HAL_GetTick();
                double_beep_played = false;
                for (int k = 0; k < 8; ++k) hold_effect_active[k] = false;
                hold_effect_active[i] = true;
                break;
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
    MX_SPI1_Init();

    HAL_Delay(200);
    if (HAL_I2C_IsDeviceReady(&hi2c1, 0x6E, 2, 100) != HAL_OK) Error_Handler();
#if USE_I2C_CY8CMBR3108_WRITE
    if (cy8cmbr3108_write_config() != HAL_OK) Error_Handler();
#endif

    sound_engine_init();
    led_effect_engine_init();
    drv8904q1_init();
    set_leds_solid_green();

    while (1) {
        sound_engine_tick();
        sound_beep_update();
        sound_double_beep_update();

        if (!hold_chase_effect_active) led_effect_engine_update(HAL_GetTick());
        if (effect_active) led_effect_multibutton_double_blink_update(HAL_GetTick());

        led_effect_hold_multibutton_chase_left_update(HAL_GetTick());
        handle_touch_events();

        // Rückfall auf grün
        bool hold_active = false;
        for (int i = 0; i < 8; ++i) if (hold_effect_active[i]) hold_active = true;

        if (!effect_active && !any_button_pressed && !hold_active && !hold_chase_effect_active) {
            drv8904q1_set_outputs(0, 0);
            set_leds_solid_green();
        }

        // Stoppe Chase nach 2s und gib Rückmeldung
        if (hold_chase_effect_active && !double_beep_played) {
            if (HAL_GetTick() - chase_start_timestamp >= 2000) {
                sound_double_beep_start(4000, 80, 50);
                led_effect_multibutton_double_blink_start(effect_params.hue, effect_params.brightness);
                drv8904q1_set_outputs(0, 0);
                led_effect_hold_multibutton_chase_left_stop();
                for (int i = 0; i < 8; ++i) hold_effect_active[i] = false;
                hold_chase_effect_active = false;
                double_beep_played = true;
            }
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
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) Error_Handler();

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) Error_Handler();
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {}
}
