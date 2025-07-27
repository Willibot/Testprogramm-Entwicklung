#include "effects/led_effect_multibutton_double_blink.h"
#include "led_driver.h"
#include "color_utils.h"

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
    state = 1;
    effect_hue = hue;
    effect_brightness = brightness;
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
 * Ablauf: Farbe (Start) -> aus -> Farbe -> zurück zu grün
 */
void led_effect_multibutton_double_blink_update(uint32_t tick) {
    if (blink_count >= 2) {
        // Effekt ist fertig: Sofort zurück zu solid grün
        set_leds_solid_green();
        return;
    }

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