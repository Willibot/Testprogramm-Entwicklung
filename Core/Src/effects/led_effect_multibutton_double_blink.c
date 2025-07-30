#include "effects/led_effect_multibutton_double_blink.h"
#include "led_driver.h"
#include "color_utils.h"

// Rückfallfunktion aus main.c
void set_leds_solid_green(void);

// Interner Zustand für den Effekt
static uint8_t phase = 0;            // 0: Startfarbe, 1: aus, 2: farbe, 3: fertig
static uint32_t last_toggle = 0;     // Zeitstempel für Phasenwechsel
static uint8_t effect_hue = 0;       // Farbe für den Blinkeffekt
static uint8_t effect_brightness = 255; // Helligkeit
static bool effect_active = false;   // Effekt läuft nur, wenn true

#define PHASE_DURATION 150           // Dauer jeder Phase in ms

static RGB_t blink_color; // Buffer für die Blinkfarbe

/**
 * @brief Startet den Doppelblink-Effekt (z.B. blau → aus → blau → grün)
 */
void led_effect_multibutton_double_blink_start(uint8_t hue, uint8_t brightness) {
    effect_hue = hue;
    effect_brightness = brightness;
    phase = 0;
    effect_active = true;
    last_toggle = 0;

    // Farbe nur einmal berechnen und im Buffer speichern
    blink_color = hsv_to_rgb(effect_hue, 255, effect_brightness);
    for (int i = 0; i < LED_COUNT; i++) {
        led_driver_set_led(i, blink_color);
    }
    led_driver_update();
}

/**
 * @brief Muss regelmäßig im Mainloop aufgerufen werden.
 * Ablauf: Farbe (Start) → aus → Farbe → zurück zu grün
 */
void led_effect_multibutton_double_blink_update(uint32_t tick) {
    if (!effect_active) return;
    if ((tick - last_toggle) < PHASE_DURATION) return;
    last_toggle = tick;

    switch (phase) {
        case 0:
            // AUS (alle LEDs auf 0)
            led_driver_clear();
            led_driver_update();
            phase = 1;
            break;
        case 1:
            // FARBE (Buffer mit bereits gesetztem Wert senden)
            for (int i = 0; i < LED_COUNT; i++) {
                led_driver_set_led(i, blink_color);
            }
            led_driver_update();
            phase = 2;
            break;
        case 2:
            set_leds_solid_green();
            effect_active = false;
            phase = 3;
            break;
        default:
            break;
    }
}