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
// Setzt das Latch-Register (0xAC) zurück, indem 0x08 an das Command-Register (0x86) geschrieben wird
void cy8cmbr3108_reset_latch_status(void)
{
    uint8_t cmd_reg = 0x86;
    uint8_t reset_cmd = 0x08;
    // 0x6E ist die 7-bit I2C-Adresse des CY8CMBR3108 (bit0=0 für Write)
    HAL_I2C_Mem_Write(&hi2c1, 0x6E, cmd_reg, I2C_MEMADD_SIZE_8BIT, &reset_cmd, 1, 100);
}