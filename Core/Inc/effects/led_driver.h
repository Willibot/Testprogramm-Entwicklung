#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"     // Für LED_COUNT und RGB_t
#include "color_utils.h"

// Initialisiert den LED-Treiber und startet die PWM-Ausgabe
void led_driver_init(void);

// Kodiert den aktuellen LED-Buffer in das PWM-Format (nach jeder Änderung aufrufen)
void led_driver_update(void);

// Setzt eine einzelne LED auf eine Farbe (Index 0..LED_COUNT-1)
void led_driver_set_led(uint8_t index, RGB_t color);

// Setzt alle LEDs auf eine Farbe
void led_driver_set_all(RGB_t color);

// Schaltet alle LEDs aus (setzt sie auf schwarz)
void led_driver_clear(void);

// Startet die DMA-Übertragung erneut (z.B. nach led_driver_update())
void led_driver_refresh(void);

// Globaler LED-Buffer (RGB-Werte für alle LEDs)
extern RGB_t led_state[LED_COUNT];

// Copilot/Entwickler-Hinweis:
// - Dieses Modul kapselt die gesamte Kommunikation mit dem LED-Ring (SK6812/WS2812) über DMA/PWM.
// - Für neue LED-Typen oder Treiber: API beibehalten, Implementierung in led_driver.c anpassen.
// - Keine direkten Manipulationen am DMA-Buffer außerhalb dieses Moduls!

#endif // LED_DRIVER_H
