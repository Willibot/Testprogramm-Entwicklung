// -----------------------------------------------------------------------------
// led_effect_hold_multibutton_chase_left.h
// Zweck: Header für "Taste gehalten"-Chase-Effekt (siehe .c-Datei).
// -----------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <stdbool.h>

void led_effect_hold_multibutton_chase_left_start(uint8_t hue, uint8_t brightness);
void led_effect_hold_multibutton_chase_left_update(uint32_t tick);
void led_effect_hold_multibutton_chase_left_stop(void);