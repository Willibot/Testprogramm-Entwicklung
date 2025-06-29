// -----------------------------------------------------------------------------
// led_effect_flash_alt.c
// Zweck: Implementiert einen alternierenden Flash-Effekt für den LED-Ring.
// Die LEDs wechseln zyklisch zwischen zwei kontrastierenden Farben (180° Farbkreis).
// Geschwindigkeit und Grundfarbe sind über effect_params steuerbar.
//
// API:
//   - led_effect_flash_alt_start(): Initialisiert den Effekt (setzt Startwerte, Timer-Reset)
//   - led_effect_flash_alt_update(uint32_t tick): Wird zyklisch aufgerufen, steuert das Umschalten
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt hsv_to_rgb() für die Farbumrechnung (keine hardcodierten Farben!)
//   - Geschwindigkeit ist über effect_params.speed steuerbar (höher = schneller)
//   - led_driver_update() nach jeder Änderung am LED-Buffer aufrufen
//   - Keine blockierenden Wartezeiten verwenden! Nur Timer-Vergleich (tick)
//   - State-Variablen sind statisch, damit mehrere Effekte unabhängig laufen können
//
// TODO: Optional: Verschiedene Kontrastfarben, Fade statt hartem Umschalten
// -----------------------------------------------------------------------------

#include "led_effect_flash_alt.h"
#include "led_driver.h"
#include "config.h"
#include "color_utils.h"

extern RGB_t led_state[LED_COUNT];
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

    for (int i = 0; i < LED_COUNT; i++) {
        led_state[i] = color;
    }

    led_driver_update();
}

// Copilot:
// - Nutze dieses Muster für weitere alternierende oder blinkende Effekte.
// - Für weichere Übergänge: Fade-Logik zwischen den Farben einbauen.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer effect_params und timer_tick!
