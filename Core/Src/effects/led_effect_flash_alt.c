#include "led_effect_flash_alt.h"
#include "led_driver.h"
#include "config.h"
#include "color_utils.h"

extern RGB_t led_state[12];
extern effect_params_t effect_params;

static uint8_t flash_on = 0;
static uint32_t last_update = 0;

void led_effect_flash_alt_start(void) {
    flash_on = 0;
    last_update = 0;
}

void led_effect_flash_alt_update(uint32_t tick) {
    if (tick - last_update < (300 - effect_params.speed * 20)) return;
    last_update = tick;

    flash_on ^= 1;

    RGB_t color = flash_on
        ? hsv_to_rgb(effect_params.hue, 255, effect_params.brightness)
        : hsv_to_rgb((effect_params.hue + 128) % 256, 255, effect_params.brightness); // Kontrastfarbe

    for (int i = 0; i < 12; i++) {
        led_state[i] = color;
    }

    led_driver_update(led_state);
}
