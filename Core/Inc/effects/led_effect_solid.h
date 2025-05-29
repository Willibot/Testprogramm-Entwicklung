#ifndef LED_EFFECT_SOLID_H
#define LED_EFFECT_SOLID_H

#include <stdint.h>

void led_effect_solid_start(void);
void led_effect_solid_update(uint32_t tick);
void led_effect_solid_stop(void);

#endif // LED_EFFECT_SOLID_H
