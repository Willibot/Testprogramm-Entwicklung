// -----------------------------------------------------------------------------
// led_effect_breathe.c
// Zweck: Implementiert einen "Breathe"-Effekt (sanftes Ein- und Ausblenden) für den LED-Ring.
// Die LEDs leuchten in der gewünschten Farbe und "atmen" durch zyklisches Helligkeits-Pulsieren.
// Geschwindigkeit und Farbe sind über effect_params steuerbar.
//
// API:
//   - led_effect_breathe_start(): Initialisiert den Effekt (setzt Startwerte, Timer-Reset)
//   - led_effect_breathe_update(uint32_t tick): Wird zyklisch aufgerufen, steuert das Pulsieren
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt hsv_to_rgb() für die Farbumrechnung (keine hardcodierten Farben!)
//   - Geschwindigkeit ist über effect_params.speed steuerbar (höher = schneller)
//   - led_driver_update() nach jeder Änderung am LED-Buffer aufrufen
//   - Keine blockierenden Wartezeiten verwenden! Nur Timer-Vergleich (tick)
//   - State-Variablen sind statisch, damit mehrere Effekte unabhängig laufen können
//
// TODO: Optional: S-Kurve für sanftere Übergänge, Parameter für Min/Max-Helligkeit
// -----------------------------------------------------------------------------

#include "led_effect_breathe.h"
#include "led_driver.h"
#include "config.h"
#include "color_utils.h"

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
    // Geschwindigkeit: Je höher effect_params.speed, desto schneller das Atmen
    uint32_t interval = 50 - effect_params.speed; // Wertebereich ggf. anpassen

    if (tick - last_update < interval) return;
    last_update = tick;

    // Richtung umkehren bei Max/Min
    if (brightness == 255) direction = -1;
    if (brightness == 0) direction = 1;

    brightness += direction;

    RGB_t color = hsv_to_rgb(effect_params.hue, 255, brightness);

    for (int i = 0; i < 12; i++) {
        led_state[i] = color;
    }

    led_driver_update();
}

// Copilot:
// - Dieser Effekt zeigt, wie man sanfte Animationen mit Timer und State-Variablen realisiert.
// - Für andere Effekte: Nutze ebenfalls Timer-Vergleiche und effect_params.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer effect_params und timer_tick!
