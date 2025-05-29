#ifndef LOGIC_CONFIG_H
#define LOGIC_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

void logic_config_init(void);
void logic_config_update(uint32_t tick_ms);
void logic_config_button_event(uint8_t event_type);

#endif // LOGIC_CONFIG_H
