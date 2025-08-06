#ifndef DRV8904Q1_H
#define DRV8904Q1_H

#include <stdint.h>

// --- Registeradressen laut TI-Datenblatt ---
#define DRV8904Q1_REG_OUT_CTRL    0x01
#define DRV8904Q1_REG_FAULT_STAT  0x02
#define DRV8904Q1_REG_DIAG_STAT   0x03
// Weitere Register kannst du hier ergänzen ...

// --- Öffentliche Funktionen ---
void drv8904q1_init(void);
void drv8904q1_write_register(uint8_t reg, uint16_t value);
uint16_t drv8904q1_read_register(uint8_t reg);
void drv8904q1_set_outputs(uint8_t out1, uint8_t out2);
void drv8904q1_toggle_output(uint8_t out_num);

#endif // DRV8904Q1_H
