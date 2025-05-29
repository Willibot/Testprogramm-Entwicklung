#ifndef LED_EFFECT_BLINK_H
#define LED_EFFECT_BLINK_H

#include <stdint.h>
#include <stdbool.h>

void led_effect_blink_start(void);
void led_effect_blink_update(uint32_t tick);

#endif // LED_EFFECT_BLINK_H
