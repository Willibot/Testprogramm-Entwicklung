// sound_beep.c
#include "sound_beep.h"
#include "piezo_driver.h"

static bool beep_active = false;
static uint32_t beep_end_time = 0;

void sound_beep_start(uint16_t duration_ms, uint16_t frequency_hz) {
    piezo_on(frequency_hz);
    beep_end_time = timer_tick + duration_ms;
    beep_active = true;
}

void sound_beep_update(void) {
    if (beep_active && timer_tick >= beep_end_time) {
        piezo_off();
        beep_active = false;
    }
}

bool sound_beep_is_active(void) {
    return beep_active;
}
