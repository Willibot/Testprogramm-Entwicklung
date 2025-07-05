// cy8cmbr3108.h
#pragma once

#include "stm32g0xx_hal.h"

#define CY8CMBR3108_I2C_ADDR            (0x37 << 1) // 8-bit Adresse
#define CY8CMBR3108_BUTTON_STAT         0xAA        // Echtzeit-Tastenstatus
#define CY8CMBR3108_LATCHED_BUTTON_STAT 0xAC        // Latched Button Status

uint8_t cy8cmbr3108_read_button_stat(void);
uint8_t cy8cmbr3108_read_latched_button_stat(void);