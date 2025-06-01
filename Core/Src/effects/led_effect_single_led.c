// -----------------------------------------------------------------------------
// led_effect_single_led.c
// Zweck: Einfacher Effekt, bei dem nur eine einzelne LED (LED 0) blinkt.
// Farbe und Geschwindigkeit sind über effect_params steuerbar.
//
// API:
//   - led_effect_single_led_start(): Initialisiert den Effekt (setzt Startwerte, Timer-Reset)
//   - led_effect_single_led_update(uint32_t tick): Wird zyklisch aufgerufen, steuert das Blinken
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt hsv_to_rgb() für die Farbumrechnung (keine hardcodierten Farben!)
//   - Geschwindigkeit ist über effect_params.speed steuerbar (höher = schneller)
//   - led_driver_update() nach jeder Änderung am LED-Buffer aufrufen
//   - Keine blockierenden Wartezeiten verwenden! Nur Timer-Vergleich (tick)
//   - State-Variablen sind statisch, damit mehrere Effekte unabhängig laufen können
//
// TODO: Optional: Andere LED auswählbar machen, Fade-In/Fade-Out statt hartem Blinken
// -----------------------------------------------------------------------------

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

    led_driver_update();
}

// Copilot:
// - Nutze dieses Muster für weitere Einzel-LED-Effekte oder Statusanzeigen.
// - Für andere LEDs: Index dynamisch machen (z.B. über Parameter).
// - Keine blockierenden Delays, keine globale Abhängigkeit außer effect_params und timer_tick!
