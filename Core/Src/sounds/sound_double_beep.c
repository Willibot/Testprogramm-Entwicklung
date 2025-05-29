// sound_double_beep.c
#include "sound_double_beep.h"
#include "piezo_driver.h"

static enum {
    DBEEP_IDLE,
    DBEEP_FIRST_BEEP,
    DBEEP_GAP,
    DBEEP_SECOND_BEEP
} state = DBEEP_IDLE;

static uint32_t timestamp = 0;

void sound_double_beep_start(uint16_t freq, uint16_t len_ms, uint16_t pause_ms) {
    piezo_on(freq);
    state = DBEEP_FIRST_BEEP;
    timestamp = timer_tick + len_ms;
}

void sound_double_beep_update(void) {
    switch (state) {
        case DBEEP_FIRST_BEEP:
            if (timer_tick >= timestamp) {
                piezo_off();
                state = DBEEP_GAP;
                timestamp = timer_tick + 50;  // Kurze Pause
            }
            break;

        case DBEEP_GAP:
            if (timer_tick >= timestamp) {
                piezo_on(4000); // zweite Beep-Frequenz (z.B. fix)
                state = DBEEP_SECOND_BEEP;
                timestamp = timer_tick + 80;
            }
            break;

        case DBEEP_SECOND_BEEP:
            if (timer_tick >= timestamp) {
                piezo_off();
                state = DBEEP_IDLE;
            }
            break;

        default:
            break;
    }
}

bool sound_double_beep_is_active(void) {
    return state != DBEEP_IDLE;
}
