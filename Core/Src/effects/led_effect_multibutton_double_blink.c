#include "effects/led_effect_multibutton_double_blink.h"
#include "led_driver.h"
#include "color_utils.h"

// Interner Zähler, wie viele Umschaltungen (An/Aus) schon erfolgt sind (2 = 1x Blinken)
static uint8_t blink_count = 0;

// Zeitpunkt der letzten Umschaltung (für das Timing)
static uint32_t last_toggle = 0;

// Aktueller Zustand: 0 = aus, 1 = an
static uint8_t state = 1; // Effekt startet mit Farbe!

// Gespeicherte Farbe und Helligkeit für den Effekt
static uint8_t effect_hue = 0;
static uint8_t effect_brightness = 255;

/**
 * @brief Startet den Multibutton-Doppelblink-Effekt.
 *
 * Setzt alle internen Variablen zurück und speichert die gewünschte Farbe und Helligkeit.
 * Die LEDs werden initial auf die gewünschte Farbe gesetzt.
 *
 * @param hue        Farbwert im HSV-Farbraum (0-255)
 * @param brightness Helligkeit (0-255)
 */
void led_effect_multibutton_double_blink_start(uint8_t hue, uint8_t brightness) {
    blink_count = 0;        // Noch keine Umschaltung erfolgt
    last_toggle = 0;        // Startzeitpunkt zurücksetzen
    state = 1;              // LEDs starten AN (Farbe)
    effect_hue = hue;       // Gewünschte Farbe speichern
    effect_brightness = brightness; // Gewünschte Helligkeit speichern
    // LEDs sofort auf Farbe setzen
    RGB_t color = hsv_to_rgb(effect_hue, 255, effect_brightness);
    for (int i = 0; i < LED_COUNT; i++) {
        led_driver_set_led(i, color);
    }
    led_driver_update();
}

/**
 * @brief Aktualisiert den Multibutton-Doppelblink-Effekt.
 *
 * Diese Funktion muss regelmäßig (z.B. in jedem Mainloop-Durchlauf) aufgerufen werden.
 * Sie schaltet die LEDs im 100ms-Takt an und aus, insgesamt dreimal (Farbe-Aus-Farbe).
 * Nach Abschluss des Effekts übernimmt main.c wieder den Grundzustand (z.B. grün).
 *
 * @param tick Aktueller Zeitwert (z.B. von HAL_GetTick()), wird für das Timing verwendet.
 */
void led_effect_multibutton_double_blink_update(uint32_t tick) {
    // Effekt nach zwei Umschaltungen (aus, farbe) beenden!
    if (blink_count >= 2) {
        return;
    }

    // Prüfe, ob seit der letzten Umschaltung mindestens 100 ms vergangen sind
    if (tick - last_toggle < 30) return;
    last_toggle = tick;

    if (blink_count == 0) {
        // 1. Umschalten: AUS
        led_driver_clear();
        led_driver_update();
        state = 0;
        blink_count++;
    } else if (blink_count == 1) {
        // 2. Umschalten: FARBE
        RGB_t color = hsv_to_rgb(effect_hue, 255, effect_brightness);
        for (int i = 0; i < LED_COUNT; i++) {
            led_driver_set_led(i, color);
        }
        led_driver_update();
        state = 1;
        blink_count++;
    }
}