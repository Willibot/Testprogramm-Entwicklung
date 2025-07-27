#include "effects/led_effect_multibutton_double_blink.h"
#include "led_driver.h"
#include "color_utils.h"

// Funktionsprototyp aus main.c:
void set_leds_solid_green(void);

// Interner Zähler, wie viele Umschaltungen (An/Aus) schon erfolgt sind
static uint8_t blink_count = 0;
static uint32_t last_toggle = 0;
static uint8_t state = 1; // Effekt startet mit Farbe!
static uint8_t effect_hue = 0;
static uint8_t effect_brightness = 255;

/**
 * @brief Startet den Multibutton-Doppelblink-Effekt.
 */
void led_effect_multibutton_double_blink_start(uint8_t hue, uint8_t brightness) {
    blink_count = 0;
    last_toggle = 0;
    effect_hue = hue;
    effect_brightness = brightness;
    // LEDs sofort auf Farbe setzen
    RGB_t color = hsv_to_rgb(effect_hue, 255, effect_brightness);
    for (int i = 0; i < LED_COUNT; i++) {
        led_driver_set_led(i, color);
    }
    led_driver_update();
    // phase-Variable zurücksetzen!
    extern uint8_t phase;
    phase = 0;
}

/**
 * @brief Aktualisiert den Multibutton-Doppelblink-Effekt.
 *
 * Ablauf: Startfarbe (durch Startfunktion) -> aus -> farbe -> zurück zu grün
 */
void led_effect_multibutton_double_blink_update(uint32_t tick) {
    // Ablauf: Startfarbe (durch Startfunktion) -> aus -> farbe -> zurück zu grün
    static uint8_t phase = 0; // 0: aus, 1: farbe, 2: fertig

    if (phase == 2) {
        set_leds_solid_green();
        phase = 0;
        blink_count = 0;
        return;
    }

    if (tick - last_toggle < 200) return; // 200 ms pro Phase
    last_toggle = tick;

    if (phase == 0) {
        // 1. Umschalten: AUS
        led_driver_clear();
        led_driver_update();
        phase = 1;
    } else if (phase == 1) {
        // 2. Umschalten: FARBE
        RGB_t color = hsv_to_rgb(effect_hue, 255, effect_brightness);
        for (int i = 0; i < LED_COUNT; i++) {
            led_driver_set_led(i, color);
        }
        led_driver_update();
        phase = 2;
    }
}