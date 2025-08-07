// sound_double_beep.h
#pragma once

#include <stdint.h>
#include <stdbool.h>

void sound_double_beep_start(uint16_t freq, uint16_t duration, uint8_t volume);
void sound_double_beep_stop(void);
void sound_double_beep_update(void);
