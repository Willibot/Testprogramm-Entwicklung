#ifndef COLOR_UTILS_H
#define COLOR_UTILS_H

#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} RGB_t;

RGB_t hsv_to_rgb(uint8_t h, uint8_t s, uint8_t v);
RGB_t scale_brightness(RGB_t color, uint8_t brightness);

#endif // COLOR_UTILS_H
