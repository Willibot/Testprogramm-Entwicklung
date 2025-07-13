// cy8cmbr3108.h
#pragma once

#include "stm32g0xx_hal.h"

#define CY8CMBR3108_I2C_ADDR      (0x37 << 1) // Standard-Adresse
#define CY8CMBR3108_SENSOR_INPUT_STATUS 0x08  // Sensor Input Status Register

uint8_t cy8cmbr3108_read_sensor_input_status(void);