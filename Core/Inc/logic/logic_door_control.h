#ifndef LOGIC_DOOR_CONTROL_H
#define LOGIC_DOOR_CONTROL_H

#include <stdint.h>

void logic_door_control_init(void);
void logic_door_control_update(void);
void logic_door_control_handle(uint8_t event);

#endif // LOGIC_DOOR_CONTROL_H
