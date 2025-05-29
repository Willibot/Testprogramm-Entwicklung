#include "led_effect_blink.h"
#include "led_driver.h"
#include "color_utils.h"
#include "config.h"
#include "main.h" // für timer_tick
#include "led_effect_engine.h"

static bool state = false;
static uint32_t last_toggle = 0;

void led_effect_blink_start(void) {
    state = false;
    last_toggle = timer_tick;
    led_driver_clear();
}

void led_effect_blink_update(uint32_t tick) {
    uint32_t interval = 200 + (1000 - (uint32_t)effect_params.speed * 8); // speed 0–255 → 1000–200 ms

    if (tick - last_toggle >= interval) {
        last_toggle = tick;
        state = !state;

        if (state) {
            RGB_t color = hsv2rgb(effect_params.hue, 255, effect_params.brightness);
            for (int i = 0; i < LED_COUNT; i++) {
                led_state[i] = color;
            }
        } else {
            led_driver_clear();
        }

        led_driver_update();
    }
}
