#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>
#include <stdbool.h>
#include "config.h"     // Für LED_COUNT und RGB_t

void led_driver_init(void);
void led_driver_update(void);
void led_driver_set_led(uint8_t index, RGB_t color);
void led_driver_set_all(RGB_t color);
void led_driver_clear(void);
void led_driver_refresh(void);

extern RGB_t led_state[LED_COUNT];

#endif // LED_DRIVER_H
