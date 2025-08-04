// -----------------------------------------------------------------------------
// led_effect_hold_mulitbutton_chase_left.c
// Zweck: Signalisierung "Taste gehalten" – Alle LEDs leuchten in Tasterfarbe,
// eine LED ist aus und läuft im Kreis, solange die Taste gehalten wird.
//
// API:
//   - led_effect_hold_multibutton_chase_left_start(): Initialisiert den Effekt (setzt Startposition, Timer-Reset)
//   - led_effect_hold_multibutton_chase_left_update(uint32_t tick): Wird zyklisch aufgerufen, steuert das Laufen
//   - led_effect_hold_multibutton_chase_left_stop(): Stoppt den Effekt und schaltet alle LEDs aus
//
// Hinweise für Entwickler & Copilot:
//   - Die Farbe wird über effect_params.hue gesetzt (z.B. in main.c je nach Taste).
//   - Geschwindigkeit kann über effect_params.speed gesteuert werden.
//   - led_driver_update() nach jeder Änderung am LED-Buffer aufrufen.
//   - Keine blockierenden Wartezeiten verwenden! Nur Timer-Vergleich (tick).
//   - Effekt läuft solange, bis led_effect_hold_multibutton_chase_left_stop() aufgerufen wird.
// -----------------------------------------------------------------------------

#include "led_effect_hold_multibutton_chase_left.h"
#include "led_driver.h"
#include "config.h"
#include "color_utils.h"

extern effect_params_t effect_params;
extern RGB_t led_state[LED_COUNT];

static uint8_t current_pos = 0;
static uint32_t last_update = 0;
extern volatile bool hold_chase_effect_active;

/**
 * @brief Startet den "Taste gehalten"-Effekt.
 * Setzt Startposition und aktiviert den Effekt.
 */
void led_effect_hold_multibutton_chase_left_start(void) {
    current_pos = 0;
    last_update = 0;
    hold_chase_effect_active = true;
}

/**
 * @brief Aktualisiert den "Taste gehalten"-Effekt.
 * Alle LEDs leuchten in Tasterfarbe, eine LED (current_pos) ist aus und läuft im Kreis.
 * Wird zyklisch im Mainloop aufgerufen.
 */
void led_effect_hold_multibutton_chase_left_update(uint32_t tick) {
    if (!hold_chase_effect_active) return;

    // Geschwindigkeit: Je höher effect_params.speed, desto kürzer das Intervall
    uint32_t interval = 150 - effect_params.speed * 10;
    if (tick - last_update < interval) return; // Noch nicht genug Zeit vergangen
    last_update = tick;

    // Alle LEDs in Tasterfarbe, eine kreisend AUS
    for (int i = 0; i < LED_COUNT; i++) {
        if (i == current_pos) {
            // Die laufende LED wird ausgeschaltet (schwarz)
            led_state[i] = (RGB_t){0, 0, 0};
        } else {
            // Alle anderen LEDs leuchten in der aktuellen Tasterfarbe und Helligkeit
            led_state[i] = hsv_to_rgb(effect_params.hue, 255, effect_params.brightness);
        }
    }

    // Laufposition für die nächste Runde erhöhen (Ring-Effekt)
    current_pos = (current_pos + 1) % LED_COUNT;

    // LED-Daten an den Treiber senden
    led_driver_update();
}

/**
 * @brief Stoppt den "Taste gehalten"-Effekt und schaltet alle LEDs aus.
 */
void led_effect_hold_multibutton_chase_left_stop(void) {
    hold_chase_effect_active = false;
    for (int i = 0; i < LED_COUNT; i++) {
        led_state[i] = (RGB_t){0, 0, 0};
    }
    led_driver_update();
}

// Copilot:
// - Die Farbe wird immer aus effect_params.hue übernommen (z.B. in main.c je nach Taste setzen).
// - Effekt läuft solange, bis led_effect_hold_multibutton_chase_left_stop() aufgerufen wird.
// - Für weitere Varianten: Geschwindigkeit, Richtung, Helligkeit anpassen.