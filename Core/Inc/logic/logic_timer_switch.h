#ifndef LOGIC_TIMER_SWITCH_H
#define LOGIC_TIMER_SWITCH_H

#include <stdint.h>
#include <stdbool.h>

void logic_timer_switch_init(void);
void logic_timer_switch_update(uint32_t tick_ms);
void logic_timer_switch_trigger(void);

#endif // LOGIC_TIMER_SWITCH_H
