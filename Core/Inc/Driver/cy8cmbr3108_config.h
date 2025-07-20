/*
 * CY8CMBR3108 Konfigurations- und Write-Interface
 * 
 * Hinweise:
 *  - Das CRC-Register (0x7E) MUSS geschrieben werden!
 *  - 0x00, 0x02, 0x04, 0x7C, 0x7D, 0x7F NICHT schreiben!
 *  - Das Konfigurationsarray ist NUR in der .c-Datei definiert!
 */

#ifndef CY8CMBR3108_CONFIG_H
#define CY8CMBR3108_CONFIG_H

#include <stdint.h>
#include "i2c.h"

// Nur Deklaration! Definition in cy8cmbr3108_config.c
extern const uint8_t cy8cmbr3108_config_data[128];

/*
 * Array aller beschreibbaren Registeradressen (laut TRM/EZ-Click)
 * Die Kommentare geben den Zweck des Registers an.
 */
static const uint8_t cy8cmbr3108_writable_registers[] = {
    0x06, // LED_ON_EN
    0x08, // SENSITIVITY0
    0x09, // SENSITIVITY1
    0x0A, // SENSITIVITY2
    0x0B, // SENSITIVITY3
    0x0C, // BASE_THRESHOLD0
    0x0D, // BASE_THRESHOLD1
    0x0E, // FINGER_THRESHOLD2
    0x0F, // FINGER_THRESHOLD3
    0x10, // FINGER_THRESHOLD4
    0x11, // FINGER_THRESHOLD5
    0x12, // FINGER_THRESHOLD6
    0x13, // FINGER_THRESHOLD7
    0x14, // FINGER_THRESHOLD8
    0x15, // FINGER_THRESHOLD9
    0x16, // FINGER_THRESHOLD10
    0x17, // FINGER_THRESHOLD11
    0x18, // FINGER_THRESHOLD12
    0x19, // FINGER_THRESHOLD13
    0x1A, // FINGER_THRESHOLD14
    0x1B, // FINGER_THRESHOLD15
    0x1C, // SENSOR_DEBOUNCE
    0x1D, // BUTTON_HYS
    0x1F, // BUTTON_LBR
    0x20, // BUTTON_NNT
    0x21, // BUTTON_NT
    0x26, // PROX_EN
    0x27, // PROX_CFG
    0x28, // PROX_CFG2
    0x2A, // PROX_TOUCH_TH0
    0x2C, // PROX_TOUCH_TH1
    0x2E, // PROX_RESOLUTION0
    0x2F, // PROX_RESOLUTION1
    0x30, // PROX_HYS
    0x32, // PROX_LBR
    0x33, // PROX_NNT
    0x34, // PROX_NT
    0x35, // PROX_POSITIVE_TH0
    0x36, // PROX_POSITIVE_TH1
    0x39, // PROX_NEGATIVE_TH0
    0x3A, // PROX_NEGATIVE_TH1
    0x3D, // LED_ON_TIME
    0x3E, // BUZZER_CFG
    0x3F, // BUZZER_ON_TIME
    0x40, // GPO_CFG
    0x41, // PWM_DUTYCYCLE_CFG0
    0x42, // PWM_DUTYCYCLE_CFG1
    0x43, // PWM_DUTYCYCLE_CFG2
    0x44, // PWM_DUTYCYCLE_CFG3
    0x45, // PWM_DUTYCYCLE_CFG4
    0x46, // PWM_DUTYCYCLE_CFG5
    0x47, // PWM_DUTYCYCLE_CFG6
    0x48, // PWM_DUTYCYCLE_CFG7
    0x4C, // SPO_CFG
    0x4D, // DEVICE_CFG0
    0x4E, // DEVICE_CFG1
    0x4F, // DEVICE_CFG2
    0x50, // DEVICE_CFG3
    0x51, // I2C_ADDR
    0x52, // REFRESH_CTRL
    0x55, // STATE_TIMEOUT
    0x5D, // SLIDER_CFG
    0x61, // SLIDER1_CFG
    0x62, // SLIDER1_RESOLUTION
    0x63, // SLIDER1_THRESHOLD
    0x67, // SLIDER2_CFG
    0x68, // SLIDER2_RESOLUTION
    0x69, // SLIDER2_THRESHOLD
    0x71, // SLIDER_LBR
    0x72, // SLIDER_NNT
    0x73, // SLIDER_NT
    0x7A, // SCRATCHPAD0
    0x7B, // SCRATCHPAD1
    0x7E  // CONFIG_CRC (muss geschrieben werden!)
};

/*
 * Write-Funktion: Schreibt automatisch alle beschreibbaren Register
 * Holt für jedes Register den Wert aus cy8cmbr3108_config_data[reg]
 * und schreibt ihn per I2C in das entsprechende Register.
 */
static inline HAL_StatusTypeDef cy8cmbr3108_write_config(void) {
    HAL_StatusTypeDef ret;
    for (unsigned i = 0; i < sizeof(cy8cmbr3108_writable_registers)/sizeof(cy8cmbr3108_writable_registers[0]); ++i) {
        uint8_t reg = cy8cmbr3108_writable_registers[i];
        ret = HAL_I2C_Mem_Write(&hi2c1, (0x37 << 1), reg, I2C_MEMADD_SIZE_8BIT,
                                (uint8_t*)&cy8cmbr3108_config_data[reg], 1, 100);
        if (ret != HAL_OK) return ret;
        HAL_Delay(2);
    }
    return HAL_OK;
}

#endif // CY8CMBR3108_CONFIG_H