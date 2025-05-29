#include "logic_timer_switch.h"
#include "config.h"

static bool pulse_active = false;
static uint32_t pulse_start_time = 0;
static const uint32_t pulse_duration_ms = 500;

extern uint32_t timer_tick;
extern bool output_state[3];

void logic_timer_switch_init(void) {
    pulse_active = false;
    output_state[0] = false;
}

void logic_timer_switch_trigger(void) {
    pulse_active = true;
    pulse_start_time = timer_tick;
    output_state[0] = true;
}

void logic_timer_switch_update(uint32_t tick_ms) {
    if (pulse_active && (tick_ms - pulse_start_time >= pulse_duration_ms)) {
        pulse_active = false;
        output_state[0] = false;
    }
}
