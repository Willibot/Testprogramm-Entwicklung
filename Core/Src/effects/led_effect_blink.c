// -----------------------------------------------------------------------------
// led_effect_blink.c
// Zweck: Implementiert einen einfachen Blink-Effekt für den LED-Ring.
// Die LEDs blinken in der gewünschten Farbe und Helligkeit, Geschwindigkeit
// ist über effect_params.speed steuerbar.
//
// API:
//   - led_effect_blink_start(): Initialisiert den Effekt (setzt LEDs aus, Timer-Reset)
//   - led_effect_blink_update(uint32_t tick): Wird zyklisch aufgerufen, steuert das Blinken
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt hsv2rgb() für die Farbumrechnung (keine hardcodierten Farben!)
//   - Geschwindigkeit ist über effect_params.speed (0=langsam, 255=schnell) steuerbar
//   - led_driver_update() nach jeder Änderung am LED-Buffer aufrufen
//   - Keine blockierenden Wartezeiten verwenden! Nur Timer-Vergleich (tick)
//   - State-Variablen sind statisch, damit mehrere Effekte unabhängig laufen können
//
// TODO: Optional: Fade-In/Fade-Out statt hartem An/Aus
// -----------------------------------------------------------------------------

#include "led_effect_blink.h"
#include "led_driver.h"
#include "color_utils.h"
#include "config.h"
#include "main.h" // für timer_tick
#include "led_effect_engine.h"

extern uint32_t timer_tick;

static bool state = false;
static uint32_t last_toggle = 0;

void led_effect_blink_start(void) {
    state = false;
    last_toggle = timer_tick;
    led_driver_clear();
}

void led_effect_blink_update(uint32_t tick)
{
    uint32_t interval = 200 + (1000 - (uint32_t)effect_params.speed * 8);

    if (tick - last_toggle >= interval) {
        last_toggle = tick;
        state = !state;

        if (state) {
            RGB_t color = hsv_to_rgb(effect_params.hue, 255, effect_params.brightness);
            for (int i = 0; i < LED_COUNT; i++) {
                led_state[i] = color;
            }
        } else {
            led_driver_clear();
        }

        led_driver_update();
    }
}

// Copilot: 
// - Dieser Effekt ist ein Template für einfache zeitbasierte Animationen.
// - Für andere Effekte: Nutze ebenfalls Timer-Vergleiche und effect_params.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer effect_params und timer_tick!
