#ifndef PIEZO_DRIVER_H
#define PIEZO_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

void piezo_init(void);
void piezo_beep(uint16_t freq_hz, uint16_t duration_ms);
void piezo_stop(void);
void piezo_update(void);

#endif // PIEZO_DRIVER_H
