// cy8cmbr3108.c
#include "Driver/cy8cmbr3108.h"
#include "Driver/cy8cmbr3108_config.h"
#include "i2c.h"
#include <stdio.h>

#define CY8CMBR3108_I2C_ADDRESS (0x37 << 1) // = 0x6E
#define CY8CMBR3108_REGISTER_START 0x06
#define CY8CMBR3108_REGISTER_COUNT 0x7D - 0x06 + 1 // = 122

#define BLOCK1_LEN  41
#define BLOCK2_LEN  41
#define BLOCK3_LEN  40

extern I2C_HandleTypeDef hi2c1;
extern const uint8_t cy8cmbr3108_config_data[128];

uint8_t cy8cmbr3108_read_sensor_input_status(void) {
    uint8_t status = 0;
    HAL_I2C_Mem_Read(&hi2c1,
                     CY8CMBR3108_I2C_ADDR,
                     CY8CMBR3108_SENSOR_INPUT_STATUS,
                     I2C_MEMADD_SIZE_8BIT,
                     &status,
                     1,
                     10);
    return status;
}

uint8_t cy8cmbr3108_read_button_stat(void) {
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1,
                     CY8CMBR3108_I2C_ADDR,
                     CY8CMBR3108_BUTTON_STAT,
                     I2C_MEMADD_SIZE_8BIT,
                     &value,
                     1,
                     10);
    return value;
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

HAL_StatusTypeDef cy8cmbr3108_write_config(void) {
    // Nur gültige Konfigurationsdaten übertragen (0x06–0x7D)
    HAL_StatusTypeDef ret = HAL_I2C_Mem_Write(
        &hi2c1,
        CY8CMBR3108_I2C_ADDRESS,
        CY8CMBR3108_REGISTER_START,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t*)&cy8cmbr3108_config_data[CY8CMBR3108_REGISTER_START],
        CY8CMBR3108_REGISTER_COUNT,
        100
    );
    if (ret != HAL_OK) return ret;

    HAL_Delay(10);

    // Konfiguration speichern (Command 0xA0 → Register 0x86)
    uint8_t cmd = 0xA0;
    ret = HAL_I2C_Mem_Write(
        &hi2c1, CY8CMBR3108_I2C_ADDRESS,
        0x86, I2C_MEMADD_SIZE_8BIT,
        &cmd, 1, 100
    );
    if (ret != HAL_OK) return ret;

    HAL_Delay(10);

    // Scanning aktivieren (Command 0xB1 → Register 0x86)
    cmd = 0xB1;
    ret = HAL_I2C_Mem_Write(
        &hi2c1, CY8CMBR3108_I2C_ADDRESS,
        0x86, I2C_MEMADD_SIZE_8BIT,
        &cmd, 1, 100
    );
    return ret;
}

void cy8cmbr3108_dump_config(void) {
    for (uint8_t addr = 0; addr < 0x80; addr++) {
        uint8_t value = 0;
        HAL_I2C_Mem_Read(&hi2c1, CY8CMBR3108_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &value, 1, 10);
        printf("Reg 0x%02X: 0x%02X\r\n", addr, value);
    }
}

uint8_t cy8cmbr3108_read_config_byte(uint8_t addr) {
    uint8_t value = 0;
    HAL_I2C_Mem_Read(&hi2c1, CY8CMBR3108_I2C_ADDR, addr, I2C_MEMADD_SIZE_8BIT, &value, 1, 10);
    return value;
}

HAL_StatusTypeDef cy8cmbr3108_write_config_partial(void) {
    HAL_StatusTypeDef ret;

    // Erster Block (0x06 bis 0x2E)
    ret = HAL_I2C_Mem_Write(
        &hi2c1,
        CY8CMBR3108_I2C_ADDRESS,
        CY8CMBR3108_REGISTER_START,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t*)&cy8cmbr3108_config_data[CY8CMBR3108_REGISTER_START],
        BLOCK1_LEN,
        100
    );
    if (ret != HAL_OK) return ret;
    HAL_Delay(10);

    // Zweiter Block (0x2F bis 0x57)
    ret = HAL_I2C_Mem_Write(
        &hi2c1,
        CY8CMBR3108_I2C_ADDRESS,
        CY8CMBR3108_REGISTER_START + BLOCK1_LEN,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t*)&cy8cmbr3108_config_data[CY8CMBR3108_REGISTER_START + BLOCK1_LEN],
        BLOCK2_LEN,
        100
    );
    if (ret != HAL_OK) return ret;
    HAL_Delay(10);

    // Dritter Block (0x58 bis 0x7D)
    ret = HAL_I2C_Mem_Write(
        &hi2c1,
        CY8CMBR3108_I2C_ADDRESS,
        CY8CMBR3108_REGISTER_START + BLOCK1_LEN + BLOCK2_LEN,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t*)&cy8cmbr3108_config_data[CY8CMBR3108_REGISTER_START + BLOCK1_LEN + BLOCK2_LEN],
        BLOCK3_LEN,
        100
    );
    return ret;
}

HAL_StatusTypeDef cy8cmbr3108_write_config_first_block(void) {
    HAL_StatusTypeDef ret;

    // Erster Block (0x06 bis 0x2E, 41 Bytes)
    ret = HAL_I2C_Mem_Write(
        &hi2c1,
        CY8CMBR3108_I2C_ADDRESS,
        CY8CMBR3108_REGISTER_START,
        I2C_MEMADD_SIZE_8BIT,
        (uint8_t*)&cy8cmbr3108_config_data[CY8CMBR3108_REGISTER_START],
        BLOCK1_LEN,
        100
    );
    return ret;
}