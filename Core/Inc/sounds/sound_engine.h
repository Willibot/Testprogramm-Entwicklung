#ifndef SOUND_ENGINE_H
#define SOUND_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

// Sound-IDs
typedef enum {
    SOUND_NONE = 0,
    SOUND_BEEP,
    SOUND_DOUBLE_BEEP,
    SOUND_CONFIG_MODE,
    SOUND_CUSTOM_1,
    SOUND_CUSTOM_2,
    SOUND_MAX
} sound_id_t;

void sound_engine_init(void);
void sound_engine_update(uint32_t tick);
void sound_engine_play(sound_id_t sound);
void sound_engine_stop(void);
bool sound_engine_is_playing(void);
void sound_play(uint8_t sound_id);

#endif // SOUND_ENGINE_H
