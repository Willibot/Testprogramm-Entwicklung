// sound_double_beep.h
#pragma once

#include <stdint.h>
#include <stdbool.h>

void sound_double_beep_start(uint16_t freq, uint16_t length_ms, uint16_t pause_ms);
void sound_double_beep_update(void);
bool sound_double_beep_is_active(void);
