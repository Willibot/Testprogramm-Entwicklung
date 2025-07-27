#include "effects/led_effect_multibutton_double_blink.h"
#include "led_driver.h"
#include "color_utils.h"

// Interner Zähler, wie viele Umschaltungen (An/Aus) schon erfolgt sind (4 = 2x Blinken)
static uint8_t blink_count = 0;

// Zeitpunkt der letzten Umschaltung (für das Timing)
static uint32_t last_toggle = 0;

// Aktueller Zustand: 0 = aus, 1 = an
static uint8_t state = 0;

// Gespeicherte Farbe und Helligkeit für den Effekt
static uint8_t effect_hue = 0;
static uint8_t effect_brightness = 255;

/**
 * @brief Startet den Multibutton-Doppelblink-Effekt.
 *
 * Setzt alle internen Variablen zurück und speichert die gewünschte Farbe und Helligkeit.
 * Die LEDs werden initial ausgeschaltet.
 *
 * @param hue        Farbwert im HSV-Farbraum (0-255)
 * @param brightness Helligkeit (0-255)
 */
void led_effect_multibutton_double_blink_start(uint8_t hue, uint8_t brightness) {
    blink_count = 0;        // Noch keine Umschaltung erfolgt
    last_toggle = 0;        // Startzeitpunkt zurücksetzen
    state = 0;              // LEDs starten aus
    effect_hue = hue;       // Gewünschte Farbe speichern
    effect_brightness = brightness; // Gewünschte Helligkeit speichern
    led_driver_clear();     // LEDs ausschalten
}

/**
 * @brief Aktualisiert den Multibutton-Doppelblink-Effekt.
 *
 * Diese Funktion muss regelmäßig (z.B. in jedem Mainloop-Durchlauf) aufgerufen werden.
 * Sie schaltet die LEDs im 200ms-Takt an und aus, insgesamt viermal (zwei Blinks).
 * Nach Abschluss des Effekts bleiben die LEDs aus.
 *
 * @param tick Aktueller Zeitwert (z.B. von HAL_GetTick()), wird für das Timing verwendet.
 */
void led_effect_multibutton_double_blink_update(uint32_t tick) {
    // Prüfe, ob seit der letzten Umschaltung mindestens 200 ms vergangen sind
    if (tick - last_toggle < 200) return;
    last_toggle = tick;

    // Nach vier Umschaltungen (An-Aus-An-Aus = 2x Blinken) Effekt beenden
    if (blink_count >= 4) {
        led_driver_clear(); // LEDs aus
        return;
    }

    // Zustand umschalten: an <-> aus
    state ^= 1;
    if (state) {
        // LEDs an: gewünschte Farbe und Helligkeit setzen
        RGB_t color = hsv_to_rgb(effect_hue, 255, effect_brightness);
        for (int i = 0; i < LED_COUNT; i++) {
            led_driver_set(i, color);
        }
    } else {
        // LEDs aus
        led_driver_clear();
    }
    blink_count++; // Eine Umschaltung mehr
    led_driver_update(); // LEDs aktualisieren
}