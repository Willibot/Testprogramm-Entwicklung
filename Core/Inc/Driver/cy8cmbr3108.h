#pragma once

#include "stm32g0xx_hal.h"

// WICHTIG: Muss hier stehen – damit es immer zur Kompilierzeit aktiv ist:
#define USE_I2C_CY8CMBR3108_READ 1

#define CY8CMBR3108_I2C_ADDR      (0x37 << 1) // Standard-Adresse

// Status-Register-Adressen
#define CY8CMBR3108_BUTTON_STAT         0xAA
#define CY8CMBR3108_LATCHED_BUTTON_STAT 0xAC

// Funktionsprototypen für Status-Lesen
uint8_t cy8cmbr3108_read_button_stat(void);         // liest 0xAA
uint8_t cy8cmbr3108_read_latched_button_stat(void); // liest 0xAC

// Latch-Register explizit zurücksetzen (0x08 an 0x86 schreiben)
void cy8cmbr3108_reset_latch_status(void);

// Das Konfigurationsarray (Definition in .c-Datei)
extern const uint8_t cy8cmbr3108_config_data[128];

// cy8cmbr3108.h
void cy8cmbr3108_init(void);
