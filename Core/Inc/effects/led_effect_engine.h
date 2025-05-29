#ifndef LED_EFFECT_ENGINE_H
#define LED_EFFECT_ENGINE_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"
#include "color_utils.h"
#include "led_driver.h"
#include "led_effect_solid.h"
#include "led_effect_blink.h"
#include "led_effect_breathe.h"
#include "led_effect_flash_alt.h"
#include "led_effect_chase_left.h"
#include "led_effect_chase_right.h"
#include "led_effect_config_mode.h"
#include "led_effect_single_led.h"

typedef enum {
    LED_EFFECT_NONE = 0,
    LED_EFFECT_SOLID,
    LED_EFFECT_BLINK,
    LED_EFFECT_BREATHE,
    LED_EFFECT_FLASH_ALT,
    LED_EFFECT_CHASE_LEFT,
    LED_EFFECT_CHASE_RIGHT,
    LED_EFFECT_CONFIG_MODE,
    LED_EFFECT_SINGLE_LED,
    LED_EFFECT_COUNT
} led_effect_id_t;

extern led_effect_id_t current_effect;
extern effect_params_t effect_params;

void led_effect_engine_set(led_effect_id_t effect);
void led_effect_engine_update(void);

#endif // LED_EFFECT_ENGINE_H
