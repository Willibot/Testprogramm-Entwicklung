#include "led_effect_single_led.h"
#include "led_driver.h"
#include "config.h"
#include "color_utils.h"

extern RGB_t led_state[12];
extern effect_params_t effect_params;

static uint32_t last_update = 0;
static uint8_t led_on = 0;

void led_effect_single_led_start(void) {
    last_update = 0;
    led_on = 1;
}

void led_effect_single_led_update(uint32_t tick) {
    if (tick - last_update < (500 - effect_params.speed * 20)) return;
    last_update = tick;
    led_on ^= 1;

    for (int i = 0; i < 12; i++) {
        if (i == 0 && led_on) {
            led_state[i] = hsv_to_rgb(effect_params.hue, 255, effect_params.brightness);
        } else {
            led_state[i].r = 0;
            led_state[i].g = 0;
            led_state[i].b = 0;
        }
    }

    led_driver_update(led_state);
}
