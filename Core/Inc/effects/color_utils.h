#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <stdint.h>

// RGB-Farbstruktur für LED-Effekte
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB_t;

// Umwandlung von HSV-Farbwerten (0..255) in RGB
RGB_t hsv_to_rgb(uint8_t h, uint8_t s, uint8_t v);

// Skaliert die Helligkeit eines RGB-Farbwerts (0..255)
RGB_t scale_brightness(RGB_t color, uint8_t brightness);

// Copilot/Entwickler-Hinweis:
// - Diese Datei stellt zentrale Farboperationen für alle LED-Effekte bereit.
// - Für neue Farboperationen: Hier ergänzen und in color_utils.c implementieren.
// - Keine dynamische Speicherallokation, nur 8-Bit Werte verwenden!

#endif // COLOR_UTILS_H
