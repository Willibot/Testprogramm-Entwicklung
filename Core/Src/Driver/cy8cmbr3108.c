// cy8cmbr3108.c
#include "Driver/cy8cmbr3108.h"
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