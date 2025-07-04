// cy8cmbr3108.c
#include "Driver/cy8cmbr3108.h"

extern I2C_HandleTypeDef hi2c1; // Passe ggf. den Handle-Namen an

uint8_t cy8cmbr3108_read_sensor_input_status(void) {
    uint8_t status = 0;
    HAL_I2C_Mem_Read(&hi2c1,
                     CY8CMBR3108_I2C_ADDR,
                     0x08, // <- Muss jetzt 0x08 sein!
                     I2C_MEMADD_SIZE_8BIT,
                     &status,
                     1,
                     10);
    return status;
}