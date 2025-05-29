// sound_engine.c
#include "sound_engine.h"
#include "sound_beep.h"
#include "sound_double_beep.h"
#include "sound_config_mode.h"
#include "config.h"

// Globale Zustände
static sound_id_t current_sound = SOUND_NONE;
static uint8_t sound_repeat_counter = 0;

void sound_engine_set(sound_id_t sound_id, uint8_t repeat) {
    current_sound = sound_id;
    sound_repeat_counter = repeat;
}

void sound_engine_tick(void) {
    if (current_sound == SOUND_NONE) return;

    switch (current_sound) {
        case SOUND_BEEP:
            sound_beep_play();
            break;
        case SOUND_DOUBLE_BEEP:
            sound_double_beep_play();
            break;
        case SOUND_CONFIG_MODE:
            sound_config_mode_play();
            break;
        default:
            break;
    }

    if (--sound_repeat_counter == 0) {
        current_sound = SOUND_NONE;
    }
}
