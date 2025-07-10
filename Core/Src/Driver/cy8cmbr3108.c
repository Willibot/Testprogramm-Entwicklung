// cy8cmbr3108.c
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"
#include "i2c.h"

uint8_t cy8cmbr3108_read_button_stat(void) {
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1, CY8CMBR3108_I2C_ADDR, CY8CMBR3108_BUTTON_STAT, I2C_MEMADD_SIZE_8BIT, &value, 1, 10);
    return value;
}

uint8_t cy8cmbr3108_read_latched_button_stat(void) {
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1, CY8CMBR3108_I2C_ADDR, CY8CMBR3108_LATCHED_BUTTON_STAT, I2C_MEMADD_SIZE_8BIT, &value, 1, 10);
    return value;
}

HAL_StatusTypeDef cy8cmbr3108_clear_latched_button_stat(void) {
    uint8_t clear = 0x01;
    return HAL_I2C_Mem_Write(&hi2c1,
                             CY8CMBR3108_I2C_ADDR,
                             0x86,
                             I2C_MEMADD_SIZE_8BIT,
                             &clear,
                             1,
                             10);
}

HAL_StatusTypeDef cy8cmbr3108_write_config(void) {
    for (uint8_t addr = 0; addr < 0x80; addr++) {
        HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(&hi2c1, CY8CMBR3108_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &cy8cmbr3108_config_data[addr], 1, 100);
        if (ret != HAL_OK) return ret;
    }
    // Save & Activate: Command-Register 0x86, Wert 0xA0
    uint8_t cmd = 0xA0;
    HAL_I2C_Mem_Write(&hi2c1, CY8CMBR3108_I2C_ADDR, 0x86, I2C_MEMADD_SIZE_8BIT, &cmd, 1, 100);
    return HAL_OK;
}