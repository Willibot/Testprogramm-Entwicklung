#ifndef LED_EFFECT_BREATHE_H
#define LED_EFFECT_BREATHE_H

#include <stdint.h>
#include <stdbool.h>

void led_effect_breathe_start(void);
void led_effect_breathe_update(uint32_t tick);

#endif
