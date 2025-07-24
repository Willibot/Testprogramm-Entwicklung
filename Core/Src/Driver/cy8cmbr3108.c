// cy8cmbr3108.c
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"
#include "i2c.h"
#include "config.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;
extern const uint8_t cy8cmbr3108_config_data[128];

// Liest den aktuellen Button-Status (Register 0xAA)
uint8_t cy8cmbr3108_read_button_stat(void) {
#if USE_I2C_CY8CMBR3108_READ
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1,
                     CY8CMBR3108_I2C_ADDR,
                     CY8CMBR3108_BUTTON_STAT,
                     I2C_MEMADD_SIZE_8BIT,
                     &value,
                     1,
                     10);
    return value;
#else
    return 0;
#endif
}

// Liest den latched Button-Status (Register 0xAC)
uint8_t cy8cmbr3108_read_latched_button_stat(void) {
#if USE_I2C_CY8CMBR3108_READ
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1,
                     CY8CMBR3108_I2C_ADDR,
                     CY8CMBR3108_LATCHED_BUTTON_STAT,
                     I2C_MEMADD_SIZE_8BIT,
                     &value,
                     1,
                     10);
    return value;
#else
    return 0;
#endif
}
