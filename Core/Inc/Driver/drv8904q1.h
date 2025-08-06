#ifndef DRV8904Q1_H
#define DRV8904Q1_H

#include "stm32g0xx_hal.h"
#include <stdint.h>

// SPI-Befehle / Register-Adressen
#define DRV8904Q1_REG_OUT_CTRL 0x01

// Initialisierung des Treibers (setzt NSLEEP high)
void drv8904q1_init(void);

// Liest den aktuellen Zustand der Ausgänge OUT1..OUT4
uint8_t drv8904q1_read_output_state(void);

// Setzt die Ausgänge OUT1..OUT4 gemäß Bitmuster
// Bit0 = OUT1, Bit1 = OUT2, Bit2 = OUT3, Bit3 = OUT4
void drv8904q1_set_outputs(uint8_t pattern);

// Schaltet einzelne Ausgänge (on/off)
void drv8904q1_set_output(uint8_t out_num, uint8_t state);

// Toggeln eines Ausgangs
void drv8904q1_toggle_output(uint8_t out_num);

// Direkter Schreibzugriff auf ein Register
void drv8904q1_write_register(uint8_t reg, uint8_t value);

#endif // DRV8904Q1_H
