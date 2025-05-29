#include "color_utils.h"

RGB_t hsv_to_rgb(uint8_t h, uint8_t s, uint8_t v) {
    RGB_t rgb = {0, 0, 0};

    uint8_t region = h / 43;
    uint8_t remainder = (h - (region * 43)) * 6;

    uint8_t p = (v * (255 - s)) >> 8;
    uint8_t q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    uint8_t t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;

    switch (region) {
        case 0:
            rgb.r = v; rgb.g = t; rgb.b = p; break;
        case 1:
            rgb.r = q; rgb.g = v; rgb.b = p; break;
        case 2:
            rgb.r = p; rgb.g = v; rgb.b = t; break;
        case 3:
            rgb.r = p; rgb.g = q; rgb.b = v; break;
        case 4:
            rgb.r = t; rgb.g = p; rgb.b = v; break;
        default:
            rgb.r = v; rgb.g = p; rgb.b = q; break;
    }

    return rgb;
}

RGB_t scale_brightness(RGB_t color, uint8_t brightness) {
    RGB_t result;
    result.r = (color.r * brightness) >> 8;
    result.g = (color.g * brightness) >> 8;
    result.b = (color.b * brightness) >> 8;
    return result;
}
