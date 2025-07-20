// cy8cmbr3108.h
#pragma once

#include "stm32g0xx_hal.h"

#define CY8CMBR3108_I2C_ADDR      (0x37 << 1) // Standard-Adresse

// Status-Register (nur falls im Code genutzt)
#define CY8CMBR3108_SENSOR_INPUT_STATUS 0x08
#define CY8CMBR3108_BUTTON_STAT         0xAA
#define CY8CMBR3108_LATCHED_BUTTON_STAT 0xAC

// Funktionsprototypen für Status-Lesen
uint8_t cy8cmbr3108_read_sensor_input_status(void);
uint8_t cy8cmbr3108_read_button_stat(void);
uint8_t cy8cmbr3108_read_latched_button_stat(void);

// Das Konfigurationsarray (Definition in .c-Datei)
extern const uint8_t cy8cmbr3108_config_data[128];