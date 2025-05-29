// led_effect_engine.c
#include "led_effect_engine.h"
#include "led_effect_solid.h"
#include "led_effect_blink.h"
#include "led_effect_breathe.h"
#include "led_effect_flash_alt.h"
#include "led_effect_chase_left.h"
#include "led_effect_chase_right.h"
#include "led_effect_config_mode.h"
#include "led_effect_single_led.h"

static led_effect_t current_effect = LED_EFFECT_NONE;
static led_effect_params_t current_params;

void led_effect_engine_init(void) {
    current_effect = LED_EFFECT_NONE;
}

void led_effect_engine_set(led_effect_t effect, led_effect_params_t *params) {
    current_effect = effect;
    if (params) {
        current_params = *params;
    }
}

void led_effect_engine_update(void) {
    switch (current_effect) {
        case LED_EFFECT_SOLID:
            led_effect_solid_update(&current_params);
            break;
        case LED_EFFECT_BLINK:
            led_effect_blink_update(&current_params);
            break;
        case LED_EFFECT_BREATHE:
            led_effect_breathe_update(&current_params);
            break;
        case LED_EFFECT_FLASH_ALT:
            led_effect_flash_alt_update(&current_params);
            break;
        case LED_EFFECT_CHASE_LEFT:
            led_effect_chase_left_update(&current_params);
            break;
        case LED_EFFECT_CHASE_RIGHT:
            led_effect_chase_right_update(&current_params);
            break;
        case LED_EFFECT_CONFIG_MODE:
            led_effect_config_mode_update(&current_params);
            break;
        case LED_EFFECT_SINGLE_LED:
            led_effect_single_led_update(&current_params);
            break;
        default:
            break;
    }
}
