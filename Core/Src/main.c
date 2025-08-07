/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body (Sensortaster-Projekt)
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "spi.h"
#include "gpio.h"
#include "config.h"

#include "effects/led_effect_engine.h"
#include "effects/led_effect_multibutton_double_blink.h"
#include "effects/led_effect_hold_multibutton_chase_left.h"
#include "sounds/sound_engine.h"
#include "sounds/sound_beep.h"
#include "sounds/sound_double_beep.h"
#include "sounds/sound_single_sweep_1.h"
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"
#include "Driver/drv8904q1.h"

uint32_t button_press_timestamp[8] = {0};
volatile bool effect_active = false;
bool hold_effect_active[8] = {0};
volatile bool hold_chase_effect_active = false;
static uint32_t chase_start_timestamp = 0;
static bool double_beep_played = false;
static bool any_button_pressed = false;
static uint8_t last_button_status = 0;

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
        extern volatile uint8_t touch_event_count;
        touch_event_count++;
    }
}

void handle_touch_events(void) {
    extern volatile uint8_t touch_event_count;
    while (touch_event_count > 0) {
        touch_event_count--;

        if (HAL_I2C_IsDeviceReady(&hi2c1, CY8CMBR3108_I2C_ADDR, 2, 10) != HAL_OK)
            return;

        uint8_t status = cy8cmbr3108_read_button_stat();
        bool now_pressed = (status & BUTTON_MASK);

        // Neue Taste gedrückt erkannt (Flanke)
        if (!any_button_pressed && now_pressed) {
            for (int i = 0; i < 8; ++i) {
                uint8_t mask = (1 << i);
                if ((BUTTON_MASK & mask) && (status & mask)) {
                    button_press_timestamp[i] = HAL_GetTick();
                    sound_single_sweep_1_start();

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

        // Taste losgelassen
        if (any_button_pressed && !now_pressed) {
            hold_chase_effect_active = false;
            drv8904q1_set_outputs(0, 0);
            set_leds_solid_green();
            effect_active = false;
        }

        any_button_pressed = now_pressed;
        last_button_status = status;
    }
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_I2C1_Init();
    MX_TIM3_Init();
    MX_TIM14_Init();
    MX_SPI1_Init();

    HAL_Delay(200);

    if (HAL_I2C_IsDeviceReady(&hi2c1, CY8CMBR3108_I2C_ADDR, 2, 100) != HAL_OK) {
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

        if (!hold_chase_effect_active)
            led_effect_engine_update(HAL_GetTick());

        led_effect_multibutton_double_blink_update(HAL_GetTick());
        led_effect_hold_multibutton_chase_left_update(HAL_GetTick());

        // Starte Chase nur wenn Taste lang genug gehalten wurde
        if (effect_active && !led_effect_multibutton_double_blink_is_active()) {
            effect_active = false;
            uint8_t status = cy8cmbr3108_read_button_stat();
            for (int i = 0; i < 8; ++i) {
                uint8_t mask = (1 << i);
                if ((BUTTON_MASK & mask) && (status & mask)) {
                    if (HAL_GetTick() - button_press_timestamp[i] > 500) {
                        led_effect_hold_multibutton_chase_left_start(effect_params.hue, 255);
                        hold_chase_effect_active = true;
                        chase_start_timestamp = HAL_GetTick();
                        double_beep_played = false;
                        hold_effect_active[i] = true;
                    } else {
                        // Tipp erkannt → DRV setzen
                        drv8904q1_set_outputs(0x01, 0x01);
                    }
                    break;
                }
            }
        }

        handle_touch_events();

        bool hold_active = false;
        for (int i = 0; i < 8; ++i) {
            if (hold_effect_active[i]) hold_active = true;
        }

        if (!effect_active && !any_button_pressed && !hold_active && !hold_chase_effect_active) {
            set_leds_solid_green();
            drv8904q1_set_outputs(0, 0);
        }

        if (hold_chase_effect_active && !double_beep_played) {
            if (HAL_GetTick() - chase_start_timestamp >= 2000) {
                sound_double_beep_start(4000, 80, 50);
                double_beep_played = true;
                led_effect_multibutton_double_blink_start(effect_params.hue, effect_params.brightness);
                drv8904q1_set_outputs(0, 0);
            }
        }
    }
}
