// cy8cmbr3108.h
#pragma once

#include "stm32g0xx_hal.h"

#define CY8CMBR3108_I2C_ADDR      (0x37 << 1) // Standard-Adresse
#define CY8CMBR3108_SENSOR_INPUT_STATUS 0x08  // Sensor Input Status Register
#define CY8CMBR3108_BUTTON_STAT         0xAA  // Button Status Register
#define CY8CMBR3108_LATCHED_BUTTON_STAT 0xAC  // Latched Button Status Register
#define CY8CMBR3108_REGISTER_START 0x06
#define BLOCK1_LEN  41
#define BLOCK2_LEN  41
#define BLOCK3_LEN  40

uint8_t cy8cmbr3108_read_sensor_input_status(void);
uint8_t cy8cmbr3108_read_button_stat(void);
uint8_t cy8cmbr3108_read_latched_button_stat(void);
HAL_StatusTypeDef cy8cmbr3108_write_config(void);
HAL_StatusTypeDef cy8cmbr3108_write_config_first_block(void);
uint8_t cy8cmbr3108_read_config_byte(uint8_t addr);

extern const uint8_t cy8cmbr3108_config_data[128];