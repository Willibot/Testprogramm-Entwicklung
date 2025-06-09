// sound_beep.h
#pragma once

#include <stdint.h>
#include <stdbool.h>

void sound_beep_start(uint16_t duration_ms, uint16_t frequency_hz);
void sound_beep_update(void);
bool sound_beep_is_active(void);
