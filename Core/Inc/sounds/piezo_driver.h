#ifndef PIEZO_DRIVER_H
#define PIEZO_DRIVER_H

#include <stdint.h>

/**
 * @brief Initialisiert den Piezo-PWM-Ausgang (TIM14 auf PA4).
 */
void piezo_init(void);

/**
 * @brief Spielt einen Ton mit bestimmter Frequenz.
 * @param freq Frequenz in Hz
 */
void piezo_beep(uint16_t freq);

/**
 * @brief Sofortiges Abschalten des Tons.
 */
void piezo_stop(void);

/**
 * @brief Muss regelmäßig aufgerufen werden, um den Ton zu stoppen, sobald die Dauer abgelaufen ist.
 */
void piezo_update(void);

#endif // PIEZO_DRIVER_H
