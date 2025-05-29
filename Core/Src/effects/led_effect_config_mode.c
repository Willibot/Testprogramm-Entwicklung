#include "led_effect_config_mode.h"
#include "led_driver.h"
#include "config.h"
#include "color_utils.h"

extern RGB_t led_state[12];
extern effect_params_t effect_params;

static uint32_t last_update = 0;
static uint8_t active_led = 0;
static uint8_t on = 0;

void led_effect_config_mode_start(void) {
    active_led = 0;
    last_update = 0;
    on = 1;
}

void led_effect_config_mode_update(uint32_t tick) {
    if (tick - last_update < (200 - effect_params.speed * 10)) return;
    last_update = tick;

    on ^= 1;  // toggle LEDs an/aus

    for (int i = 0; i < 12; i++) {
        if (i == active_led && on) {
            led_state[i] = hsv_to_rgb(effect_params.hue, 255, effect_params.brightness);
        } else {
            led_state[i].r = 0;
            led_state[i].g = 0;
            led_state[i].b = 0;
        }
    }

    active_led = (active_led + 1) % 12;

    led_driver_update(led_state);
}
