#include "effects/led_effect_multibutton_double_blink.h"
#include "led_driver.h"
#include "color_utils.h"
#include "stm32g0xx_hal.h" // <-- Diese Zeile ergänzen!

// Rückfallfunktion aus main.c
void set_leds_solid_green(void);

// Interner Zustand für den Effekt
static uint8_t phase = 0;            // 0: Startfarbe, 1: aus, 2: farbe, 3: fertig
static uint32_t last_toggle = 0;     // Zeitstempel für Phasenwechsel
static uint8_t effect_hue = 0;       // Farbe für den Blinkeffekt
static uint8_t effect_brightness = 255; // Helligkeit
static bool effect_active = false;   // Effekt läuft nur, wenn true

#define PHASE_ON_DURATION   120  // ms, Dauer der Farbblitze
#define PHASE_OFF_DURATION   80  // ms, Dauer der Dunkelphasen

static RGB_t blink_color; // Buffer für die Blinkfarbe

/**
 * @brief Startet den Doppelblink-Effekt (z.B. blau → aus → blau → grün)
 */
void led_effect_multibutton_double_blink_start(uint8_t hue, uint8_t brightness) {
    effect_hue = hue;
    effect_brightness = brightness;
    phase = 0;
    effect_active = true;
    last_toggle = HAL_GetTick();

    // Farbe nur einmal berechnen und im Buffer speichern
    blink_color = hsv_to_rgb(effect_hue, 255, effect_brightness);

    // Start mit Dunkelphase
    led_driver_clear();
    led_driver_update();
}

/**
 * @brief Muss regelmäßig im Mainloop aufgerufen werden.
 * Ablauf: Farbe (Start) → aus → Farbe → zurück zu grün
 */
void led_effect_multibutton_double_blink_update(uint32_t tick) {
    if (!effect_active) return;

    uint32_t duration = (phase % 2 == 0) ? PHASE_OFF_DURATION : PHASE_ON_DURATION;
    if ((tick - last_toggle) < duration) return;
    last_toggle = tick;

    switch (phase) {
        case 0: // Dunkelphase vor erstem Blink
            for (int i = 0; i < LED_COUNT; i++) {
                led_driver_set_led(i, blink_color);
            }
            led_driver_update();
            phase = 1;
            break;
        case 1: // Erster Blink
            led_driver_clear();
            led_driver_update();
            phase = 2;
            break;
        case 2: // Dunkelphase zwischen den Blinks
            for (int i = 0; i < LED_COUNT; i++) {
                led_driver_set_led(i, blink_color);
            }
            led_driver_update();
            phase = 3;
            break;
        case 3: // Zweiter Blink
            led_driver_clear();
            led_driver_update();
            phase = 4;
            break;
        case 4: // Dunkelphase nach dem zweiten Blink
            effect_active = false;
            phase = 5;
            break;
        default:
            break;
    }
}