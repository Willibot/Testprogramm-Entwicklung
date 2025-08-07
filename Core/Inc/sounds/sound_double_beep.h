// sound_double_beep.h
#ifndef SOUND_DOUBLE_BEEP_H
#define SOUND_DOUBLE_BEEP_H

#include <stdint.h>
#include <stdbool.h>

void sound_double_beep_start(uint16_t freq, uint16_t duration, uint8_t volume);
void sound_double_beep_stop(void);
void sound_double_beep_update(void);
bool sound_double_beep_is_active(void);

#endif
