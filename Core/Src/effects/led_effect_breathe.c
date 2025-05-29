#include "led_effect_breathe.h"
#include "led_driver.h"
#include "config.h"

extern effect_params_t effect_params;
extern RGB_t led_state[12];

static uint8_t brightness = 0;
static int8_t direction = 1;
static uint32_t last_update = 0;

void led_effect_breathe_start(void) {
    brightness = 0;
    direction = 1;
    last_update = 0;
}

void led_effect_breathe_update(uint32_t tick) {
    if (tick - last_update < (50 - effect_params.speed)) return;
    last_update = tick;

    // Richtung umkehren bei Max/Min
    if (brightness == 255) direction = -1;
    if (brightness == 0) direction = 1;

    brightness += direction;

    RGB_t color = hsv_to_rgb(effect_params.hue, 255, brightness);

    for (int i = 0; i < 12; i++) {
        led_state[i] = color;
    }

    led_driver_update(led_state);
}
