// sound_config_mode.c
#include "sound_config_mode.h"
#include "piezo_driver.h"

static enum {
    CONFIG_IDLE,
    CONFIG_BEEP1,
    CONFIG_PAUSE,
    CONFIG_BEEP2
} config_state = CONFIG_IDLE;

static uint32_t config_timer = 0;

void sound_config_mode_start(void) {
    config_state = CONFIG_BEEP1;
    config_timer = 0;
    piezo_start(4000);  // Hz
}

void sound_config_mode_update(void) {
    switch (config_state) {
        case CONFIG_BEEP1:
            if (++config_timer >= 100) {  // 100 ms
                piezo_stop();
                config_timer = 0;
                config_state = CONFIG_PAUSE;
            }
            break;

        case CONFIG_PAUSE:
            if (++config_timer >= 50) {  // 50 ms Pause
                piezo_start(6000);       // zweiter Ton höher
                config_timer = 0;
                config_state = CONFIG_BEEP2;
            }
            break;

        case CONFIG_BEEP2:
            if (++config_timer >= 150) { // 150 ms
                piezo_stop();
                config_state = CONFIG_IDLE;
            }
            break;

        case CONFIG_IDLE:
        default:
            break;
    }
}

bool sound_config_mode_is_active(void) {
    return config_state != CONFIG_IDLE;
}
