#include "effects/led_effect_multibutton_double_blink.h"
#include "led_driver.h"
#include "color_utils.h"

// Funktionsprototyp aus main.c (damit Rückfall auf grün möglich ist)
void set_leds_solid_green(void);

// Interner Zustand für den Effekt
static uint8_t phase = 0;            // 0: ausstehend, 1: aus, 2: farbe, 3: zurück zu grün
static uint32_t last_toggle = 0;     // Zeitstempel für Phasenwechsel
static uint8_t effect_hue = 0;       // Farbe für den Blinkeffekt
static uint8_t effect_brightness = 255; // Helligkeit

#define PHASE_DURATION 200           // Dauer jeder Phase in ms

/**
 * @brief Startet den Doppelblink-Effekt (z.B. blau → aus → blau → grün)
 */
void led_effect_multibutton_double_blink_start(uint8_t hue, uint8_t brightness) {
    effect_hue = hue;
    effect_brightness = brightness;
    phase = 0;
    last_toggle = 0;

    // Sofort Farbe anzeigen (Impuls 1)
    RGB_t color = hsv_to_rgb(effect_hue, 255, effect_brightness);
    for (int i = 0; i < LED_COUNT; i++) {
        led_driver_set_led(i, color);
    }
    led_driver_update();
}

/**
 * @brief Muss regelmäßig im Mainloop aufgerufen werden.
 * Ablauf: Farbe (Start) → aus → Farbe → zurück zu grün
 */
void led_effect_multibutton_double_blink_update(uint32_t tick) {
    // Warte, bis die aktuelle Phase abgelaufen ist
    if ((tick - last_toggle) < PHASE_DURATION) return;
    last_toggle = tick;

    switch (phase) {
        case 0:
            // Phase 0: Farbe → AUS
            led_driver_clear();
            led_driver_update();
            phase = 1;
            break;
        case 1:
            // Phase 1: AUS → FARBE (Impuls 2)
            {
                RGB_t color = hsv_to_rgb(effect_hue, 255, effect_brightness);
                for (int i = 0; i < LED_COUNT; i++) {
                    led_driver_set_led(i, color);
                }
                led_driver_update();
                phase = 2;
            }
            break;
        case 2:
            // Phase 2: FARBE → zurück zu grün
            set_leds_solid_green();
            phase = 3; // Effekt ist fertig
            break;
        default:
            // Effekt ist beendet, nichts tun
            break;
    }
}