#include <stdbool.h>
#include "logic_config.h"
#include "config.h"
#include "led_effect_engine.h"
#include "sound_engine.h"

#define CONFIG_TIMEOUT_MS 10000

static uint32_t config_start_time = 0;
static uint8_t config_step = 0;
extern uint32_t timer_tick;
extern bool config_mode;

void logic_config_init(void) {
    config_mode = true;
    config_step = 0;
    config_start_time = timer_tick;

    led_effect_start(EFFECT_CONFIG_MODE);
    sound_play(SOUND_CONFIG_MODE);
}

void logic_config_update(uint32_t tick_ms) {
    if (config_mode && (tick_ms - config_start_time > CONFIG_TIMEOUT_MS)) {
        config_mode = false;
        led_effect_stop();
    }
}

void logic_config_button_event(uint8_t event_type) {
    if (!config_mode) return;

    if (event_type == BUTTON_EVENT_SHORT) {
        config_step = (config_step + 1) % 3;

        switch (config_step) {
            case 0:
                // Beispiel: Ausgang 1 aktivieren
                output_state[0] = true;
                output_state[1] = false;
                sound_play(SOUND_BEEP);
                break;
            case 1:
                output_state[0] = false;
                output_state[1] = true;
                sound_play(SOUND_DOUBLE_BEEP);
                break;
            case 2:
                output_state[0] = false;
                output_state[1] = false;
                sound_play(SOUND_BEEP);
                break;
        }
    }
}
