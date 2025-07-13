// cy8cmbr3108.h
#pragma once

#include "stm32g0xx_hal.h"

#define CY8CMBR3108_I2C_ADDR            (0x37 << 1) // 8-bit Adresse
#define CY8CMBR3108_BUTTON_STAT         0xAA        // RICHTIG!
#define CY8CMBR3108_LATCHED_BUTTON_STAT 0xAC        // RICHTIG!

uint8_t cy8cmbr3108_read_latched_button_stat(void);
uint8_t cy8cmbr3108_read_button_stat(void); // <--- HIER ergänzen!
HAL_StatusTypeDef cy8cmbr3108_clear_latched_button_stat(void);
HAL_StatusTypeDef cy8cmbr3108_write_config(void);