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

// Aktuell aktiver Effekt und globale Effekt-Parameter
extern led_effect_id_t current_effect;
extern effect_params_t effect_params;

// Setzt den aktuellen Effekt (Initialisierung und Umschaltung)
void led_effect_engine_set(led_effect_id_t effect);

// Ruft zyklisch das Update des aktiven Effekts auf (z.B. im Mainloop)
void led_effect_engine_update(void);

void led_effect_start(uint8_t effect_id);
void led_effect_stop(void);

// Copilot/Entwickler-Hinweis:
// - Neue Effekte: Enum und Switch-Case in led_effect_engine.c ergänzen, Header einbinden.
// - Effekt-Parameter werden zentral über effect_params verwaltet.
// - Keine blockierenden Delays, nur State-Maschinen/Timer in den Effekten!

#define EFFECT_CONFIG_MODE LED_EFFECT_CONFIG_MODE

#endif // LED_EFFECT_ENGINE_H
