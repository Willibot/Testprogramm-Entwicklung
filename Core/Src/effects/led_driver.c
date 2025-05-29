#include "led_driver.h"
#include "tim.h"         // CubeMX generierte Timer-Konfiguration
#include "dma.h"         // DMA Handle
#include <string.h>

#define WS2812_BITS_PER_LED 24
#define LED_RESET_SLOTS     48  // ca. 50µs Reset-Zeit

#define TIMER_PWM_HIGH  65  // ca. 0.7 µs bei 800kHz
#define TIMER_PWM_LOW   30  // ca. 0.35 µs

// DMA Buffer
static uint16_t pwm_buffer[(LED_COUNT * WS2812_BITS_PER_LED) + LED_RESET_SLOTS];

RGB_t led_state[LED_COUNT];

static void encode_leds_to_pwm(void) {
    uint16_t *p = pwm_buffer;

    for (uint8_t i = 0; i < LED_COUNT; ++i) {
        uint8_t colors[3] = { led_state[i].g, led_state[i].r, led_state[i].b };
        for (int c = 0; c < 3; ++c) {
            for (int bit = 7; bit >= 0; --bit) {
                *p++ = (colors[c] & (1 << bit)) ? TIMER_PWM_HIGH : TIMER_PWM_LOW;
            }
        }
    }

    // Reset-Slots füllen
    for (int i = 0; i < LED_RESET_SLOTS; ++i) {
        *p++ = 0;
    }
}

void led_driver_init(void) {
    led_driver_clear();
    encode_leds_to_pwm();
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2, (uint32_t *)pwm_buffer, sizeof(pwm_buffer) / sizeof(uint16_t));
}

void led_driver_update(void) {
    encode_leds_to_pwm();
}

void led_driver_set_led(uint8_t index, RGB_t color) {
    if (index < LED_COUNT) {
        led_state[index] = color;
    }
}

void led_driver_set_all(RGB_t color) {
    for (uint8_t i = 0; i < LED_COUNT; ++i) {
        led_state[i] = color;
    }
}

void led_driver_clear(void) {
    RGB_t black = {0, 0, 0};
    led_driver_set_all(black);
}

void led_driver_refresh(void) {
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2, (uint32_t *)pwm_buffer, sizeof(pwm_buffer) / sizeof(uint16_t));
}
