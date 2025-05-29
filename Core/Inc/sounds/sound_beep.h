// sound_beep.h
#pragma once

#include <stdint.h>
#include <stdbool.h>

void sound_beep_start(uint16_t freq, uint16_t duration_ms);
void sound_beep_update(void);
bool sound_beep_is_active(void);
