// -----------------------------------------------------------------------------
// led_driver.c
// Zweck: Ansteuerung von SK6812/WS2812-kompatiblen LEDs über DMA und PWM.
// Diese Datei kapselt die gesamte Kommunikation mit dem LED-Ring und stellt
// Funktionen bereit, um einzelne oder alle LEDs zu setzen und die Daten
// per DMA/PWM auszugeben.
//
// API:
//   - led_driver_init(): Initialisiert den Treiber und startet die PWM-Ausgabe.
//   - led_driver_update(): Kodiert den aktuellen LED-Buffer in das PWM-Format.
//   - led_driver_set_led(index, color): Setzt eine einzelne LED auf eine Farbe.
//   - led_driver_set_all(color): Setzt alle LEDs auf eine Farbe.
//   - led_driver_clear(): Schaltet alle LEDs aus.
//   - led_driver_refresh(): Startet die DMA-Übertragung erneut (falls nötig).
//
/// Hinweise für Entwickler & Copilot:
//   - Die Funktion encode_leds_to_pwm() wandelt die RGB-Daten in das PWM-Muster
//     für die SK6812/WS2812 um. Reihenfolge: GRB!
//   - Die Timing-Konstanten (TIMER_PWM_HIGH/LOW) sind auf 800kHz ausgelegt.
//   - Nach jeder Änderung an led_state[] muss led_driver_update() aufgerufen werden,
//     damit die Änderung in den DMA-Buffer übernommen wird.
//   - led_driver_refresh() kann genutzt werden, um die Übertragung zu erzwingen.
//
// TODO: Fehlerbehandlung für DMA busy? Optional: Rückmeldung, wenn DMA-Transfer fertig.
// -----------------------------------------------------------------------------

#include "led_driver.h"
#include "tim.h"         // CubeMX generierte Timer-Konfiguration
#include "dma.h"         // DMA Handle
#include <string.h>

#define WS2812_BITS_PER_LED 24
#define LED_RESET_SLOTS     48  // ca. 50µs Reset-Zeit für Latch

#define TIMER_PWM_HIGH  65  // ca. 0.7 µs bei 800kHz
#define TIMER_PWM_LOW   30  // ca. 0.35 µs

// DMA Buffer für PWM-Daten (alle LEDs + Reset)
static uint16_t pwm_buffer[(LED_COUNT * WS2812_BITS_PER_LED) + LED_RESET_SLOTS];

// LED-Zustände (RGB für jede LED)
RGB_t led_state[LED_COUNT];

// -----------------------------------------------------------------------------
// encode_leds_to_pwm
// Kodiert den aktuellen Zustand von led_state[] in das PWM-Bufferformat für DMA.
// Reihenfolge: GRB (wie von SK6812/WS2812 erwartet).
// -----------------------------------------------------------------------------
static void encode_leds_to_pwm(void) {
    uint16_t *p = pwm_buffer;

    for (uint8_t i = 0; i < LED_COUNT; ++i) {
        uint8_t colors[3] = { led_state[i].g, led_state[i].r, led_state[i].b };
        for (int c = 0; c < 3; ++c) {
            for (int bit = 7; bit >= 0; --bit) {
                *p++ = (colors[c] & (1 << bit)) ? TIMER_PWM_HIGH : TIMER_PWM_LOW;
            }
        }
    }

    // Reset-Slots füllen (Latch-Zeit)
    for (int i = 0; i < LED_RESET_SLOTS; ++i) {
        *p++ = 0;
    }
}

// -----------------------------------------------------------------------------
// led_driver_init
// Initialisiert den LED-Treiber, setzt alle LEDs aus und startet die erste DMA-Übertragung.
// -----------------------------------------------------------------------------
void led_driver_init(void) {
    led_driver_clear();
    encode_leds_to_pwm();
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2, (uint32_t *)pwm_buffer, sizeof(pwm_buffer) / sizeof(uint16_t));
}

// -----------------------------------------------------------------------------
// led_driver_update
// Kodiert den aktuellen LED-Zustand in den DMA-Buffer (ohne Übertragung zu starten).
// Nach jedem Setzen von LEDs aufrufen!
// -----------------------------------------------------------------------------
void led_driver_update(void) {
    encode_leds_to_pwm();
}

// -----------------------------------------------------------------------------
// led_driver_set_led
// Setzt eine einzelne LED auf die gewünschte Farbe (RGB).
// -----------------------------------------------------------------------------
void led_driver_set_led(uint8_t index, RGB_t color) {
    if (index < LED_COUNT) {
        led_state[index] = color;
    }
}

// -----------------------------------------------------------------------------
// led_driver_set_all
// Setzt alle LEDs auf die gewünschte Farbe (RGB).
// -----------------------------------------------------------------------------
void led_driver_set_all(RGB_t color) {
    for (uint8_t i = 0; i < LED_COUNT; ++i) {
        led_state[i] = color;
    }
}

// -----------------------------------------------------------------------------
// led_driver_clear
// Schaltet alle LEDs aus (setzt sie auf schwarz).
// -----------------------------------------------------------------------------
void led_driver_clear(void) {
    RGB_t black = {0, 0, 0};
    led_driver_set_all(black);
}

// -----------------------------------------------------------------------------
// led_driver_refresh
// Startet die DMA-Übertragung erneut (z.B. nach led_driver_update()).
// -----------------------------------------------------------------------------
void led_driver_refresh(void) {
    HAL_TIM_PWM_Start_DMA(&htim3, TIM_CHANNEL_2, (uint32_t *)pwm_buffer, sizeof(pwm_buffer) / sizeof(uint16_t));
}

// Copilot: 
// - Diese Datei ist die einzige Stelle, an der direkt auf DMA/PWM für die LEDs zugegriffen wird.
// - Alle LED-Effekte sollen ausschließlich über led_state[] und die bereitgestellten Funktionen arbeiten.
// - Keine direkten Manipulationen am DMA-Buffer außerhalb dieses Moduls!
// - Für neue LED-Treiber oder andere LED-Typen kann dieses Modul ausgetauscht werden.
