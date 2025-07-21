// cy8cmbr3108.c
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"
#include "i2c.h"
#include "config.h"
#include <stdio.h>

#define CY8CMBR3108_I2C_ADDRESS (0x37 << 1) // = 0x6E

extern I2C_HandleTypeDef hi2c1;
extern const uint8_t cy8cmbr3108_config_data[128];

uint8_t cy8cmbr3108_read_sensor_input_status(void) {
#if USE_I2C_CY8CMBR3108_READ
    uint8_t status = 0;
    HAL_I2C_Mem_Read(&hi2c1,
                     CY8CMBR3108_I2C_ADDR,
                     CY8CMBR3108_SENSOR_INPUT_STATUS,
                     I2C_MEMADD_SIZE_8BIT,
                     &status,
                     1,
                     10);
    return status;
#else
    return 0; // Kein Zugriff möglich
#endif
}

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

uint8_t cy8cmbr3108_read_latched_button_stat(void) {
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1,
                     CY8CMBR3108_I2C_ADDR,
                     CY8CMBR3108_LATCHED_BUTTON_STAT,
                     I2C_MEMADD_SIZE_8BIT,
                     &value,
                     1,
                     10);
    return value;
}


uint8_t cy8cmbr3108_read_config_byte(uint8_t addr) {
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1, CY8CMBR3108_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &value, 1, 10);
    return value;
}