// -----------------------------------------------------------------------------
// led_effect_chase_left.c
// Zweck: Implementiert einen "Chase Left"-Effekt für den LED-Ring.
// Ein einzelnes Licht läuft im Kreis nach links, Farbe und Geschwindigkeit sind über effect_params steuerbar.
//
// API:
//   - led_effect_chase_left_start(): Initialisiert den Effekt (setzt Startposition, Timer-Reset)
//   - led_effect_chase_left_update(uint32_t tick): Wird zyklisch aufgerufen, steuert das Laufen.
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt hsv_to_rgb() für die Farbumrechnung (keine hardcodierten Farben!)
//   - Geschwindigkeit ist über effect_params.speed steuerbar (höher = schneller)
//   - led_driver_update() nach jeder Änderung am LED-Buffer aufrufen
//   - Keine blockierenden Wartezeiten verwenden! Nur Timer-Vergleich (tick)
//   - State-Variablen sind statisch, damit mehrere Effekte unabhängig laufen können
//
// TODO: Optional: Spur mit abklingender Helligkeit, Richtung umschaltbar machen
// -----------------------------------------------------------------------------

#include "led_effect_chase_left.h"
#include "led_driver.h"
#include "config.h"
#include "color_utils.h"

extern effect_params_t effect_params;
extern RGB_t led_state[LED_COUNT];

static uint8_t current_pos = 0;
static uint32_t last_update = 0;

void led_effect_chase_left_start(void) {
    current_pos = 0;
    last_update = 0;
}

void led_effect_chase_left_update(uint32_t tick) {
    // Geschwindigkeit: Je höher effect_params.speed, desto kürzer das Intervall
    uint32_t interval = 150 - effect_params.speed * 10;
    if (tick - last_update < interval) return;
    last_update = tick;

    for (int i = 0; i < LED_COUNT; i++) {
        led_state[i] = (i == current_pos)
            ? hsv_to_rgb(effect_params.hue, 255, effect_params.brightness)
            : (RGB_t){0, 0, 0};
    }

    current_pos = (current_pos + 1) % LED_COUNT;
    led_driver_update();
}

// Copilot:
// - Nutze dieses Muster für weitere Lauflichteffekte (z.B. Chase Right, Ping-Pong).
// - Für Spureffekte: Helligkeit für vorherige LEDs schrittweise absenken.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer effect_params und timer_tick!
