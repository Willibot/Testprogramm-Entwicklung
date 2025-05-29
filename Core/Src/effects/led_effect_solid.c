#include "led_effect_solid.h"
#include "led_driver.h"
#include "config.h"

extern RGB_t led_state[12];
extern effect_params_t effect_params;

static uint8_t prev_hue = 255;
static uint8_t prev_brightness = 255;

void led_effect_solid_start(void) {
    prev_hue = 255;         // Erzwingt Update beim ersten Tick
    prev_brightness = 255;
}

void led_effect_solid_update(uint32_t tick) {
    (void)tick;

    if (effect_params.hue != prev_hue || effect_params.brightness != prev_brightness) {
        RGB_t color = hsv_to_rgb(effect_params.hue, 255, effect_params.brightness);
        for (int i = 0; i < 12; i++) {
            led_state[i] = color;
        }
        led_driver_update();
        prev_hue = effect_params.hue;
        prev_brightness = effect_params.brightness;
    }
}

void led_effect_solid_stop(void) {
    for (int i = 0; i < 12; i++) {
        led_state[i] = (RGB_t){0, 0, 0};
    }
    led_driver_update();
}
