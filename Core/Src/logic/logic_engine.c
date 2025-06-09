#include "logic_engine.h"
#include "logic_toggle.h"
#include "logic_door_control.h"
#include "logic_timer_switch.h"
#include "logic_config.h"

extern logic_mode_t current_logic_mode;
extern uint32_t timer_tick;

void logic_engine_init(void) {
    // Init all logic modules
    logic_toggle_init();
    logic_door_control_init();
    logic_timer_switch_init();
    logic_config_init();
}

void logic_engine_update(void) {
    switch (current_logic_mode) {
        case LOGIC_MODE_TOGGLE:
            logic_toggle_update();
            break;
        case LOGIC_MODE_DOOR_CONTROL:
            logic_door_control_update();
            break;
        case LOGIC_MODE_TIMER_SWITCH:
            logic_timer_switch_update(timer_tick);
            break;
        case LOGIC_MODE_CONFIG:
            logic_config_update(timer_tick);
            break;
    }
}

void logic_engine_handle_button_event(uint8_t event) {
    switch (current_logic_mode) {
        case LOGIC_MODE_TOGGLE:
            logic_toggle_handle(event);
            break;
        case LOGIC_MODE_DOOR_CONTROL:
            logic_door_control_handle(event);
            break;
        case LOGIC_MODE_TIMER_SWITCH:
            logic_timer_switch_handle(event);
            break;
        case LOGIC_MODE_CONFIG:
            logic_config_handle(event);
            break;
    }
}
