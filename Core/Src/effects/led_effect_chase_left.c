#include "led_effect_chase_left.h"
#include "led_driver.h"
#include "config.h"

extern effect_params_t effect_params;
extern RGB_t led_state[12];

static uint8_t current_pos = 0;
static uint32_t last_update = 0;

void led_effect_chase_left_start(void) {
    current_pos = 0;
    last_update = 0;
}

void led_effect_chase_left_update(uint32_t tick) {
    if (tick - last_update < (150 - effect_params.speed * 10)) return;
    last_update = tick;

    for (int i = 0; i < 12; i++) {
        led_state[i] = (i == current_pos)
            ? hsv_to_rgb(effect_params.hue, 255, effect_params.brightness)
            : (RGB_t){0, 0, 0};
    }

    current_pos = (current_pos + 1) % 12;
    led_driver_update(led_state);
}
