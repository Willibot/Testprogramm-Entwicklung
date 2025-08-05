#ifndef DRV8904Q1_H
#define DRV8904Q1_H

#include "stm32g0xx_hal.h" // oder deine passende HAL-Datei

// === SPI Read/Write-Bit ===
#define DRV8904Q1_SPI_READ    (1U << 15)
#define DRV8904Q1_SPI_WRITE   (0U << 15)

// === Registeradressen ===
#define DRV8904Q1_REG_OUT_CTRL      0x1
#define DRV8904Q1_REG_FAULT_STATUS  0x2
// ggf. weitere Registeradressen ergänzen

// === Fault-Bits ===
// Diese Bits im Fault-Status-Register prüfen
#define DRV8904Q1_FAULT_OVERCURRENT   (1U << 0)  // Überstrom
#define DRV8904Q1_FAULT_OVERTEMP      (1U << 1)  // Übertemperatur
#define DRV8904Q1_FAULT_UNDERVOLTAGE  (1U << 2)  // Unterspannung
#define DRV8904Q1_FAULT_OPENLOAD      (1U << 3)  // Last offen
#define DRV8904Q1_FAULT_SPI_ERROR     (1U << 4)  // SPI-Kommunikationsfehler

// === API-Funktionen ===
void     drv8904q1_init(void);
uint16_t drv8904q1_read_register(uint8_t reg_addr);
void     drv8904q1_write_register(uint8_t reg_addr, uint16_t value);

uint16_t drv8904q1_read_output_state(void);
void     drv8904q1_set_output(uint8_t out_num, uint8_t state);
void     drv8904q1_toggle_output(uint8_t out_num);

uint16_t drv8904q1_read_faults(void);
uint8_t  drv8904q1_fault_active(uint16_t fault_mask);

#endif // DRV8904Q1_H
