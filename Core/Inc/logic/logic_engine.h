#ifndef LOGIC_ENGINE_H
#define LOGIC_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LOGIC_MODE_TOGGLE,
    LOGIC_MODE_DOOR_CONTROL,
    LOGIC_MODE_TIMER_SWITCH,
    LOGIC_MODE_CONFIG
} logic_mode_t;

void logic_engine_init(void);
void logic_engine_update(void);
void logic_engine_handle_button_event(uint8_t event);

#endif // LOGIC_ENGINE_H
