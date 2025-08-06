#include "drv8904q1.h"
#include "main.h"
#include "spi.h"

#define DRV8904Q1_CS_PIN    GPIO_PIN_0
#define DRV8904Q1_CS_PORT   GPIOB

// --- SPI Chip Select Steuerung ---
static void drv8904q1_select(void) {
    HAL_GPIO_WritePin(DRV8904Q1_CS_PORT, DRV8904Q1_CS_PIN, GPIO_PIN_RESET);
}

static void drv8904q1_deselect(void) {
    HAL_GPIO_WritePin(DRV8904Q1_CS_PORT, DRV8904Q1_CS_PIN, GPIO_PIN_SET);
}

// --- Initialisierung ---
void drv8904q1_init(void) {
    // DRV8904Q1 aus Sleep holen (NSLEEP an PB6 auf HIGH)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_Delay(1);
}

// --- Register schreiben ---
void drv8904q1_write_register(uint8_t reg, uint16_t value) {
    uint8_t tx[3];
    tx[0] = reg & 0x7F;             // MSB=0 → Write
    tx[1] = (value >> 8) & 0xFF;    // High-Byte
    tx[2] = value & 0xFF;           // Low-Byte

    drv8904q1_select();
    HAL_SPI_Transmit(&hspi1, tx, 3, HAL_MAX_DELAY);
    drv8904q1_deselect();
}

// --- Register lesen ---
uint16_t drv8904q1_read_register(uint8_t reg) {
    uint8_t tx[3];
    uint8_t rx[3];
    tx[0] = reg | 0x80;  // MSB=1 → Read
    tx[1] = 0x00;
    tx[2] = 0x00;

    drv8904q1_select();
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 3, HAL_MAX_DELAY);
    drv8904q1_deselect();

    return ((uint16_t)rx[1] << 8) | rx[2];
}

// --- Ausgänge direkt setzen ---
void drv8904q1_set_outputs(uint8_t out1, uint8_t out2) {
    uint16_t value = 0x0000;

    // OUT1 = Bit0, OUT2 = Bit1 (Anpassen falls anders gemappt)
    if (out1) value |= (1 << 0);
    if (out2) value |= (1 << 1);

    drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, value);
}

// --- Einzelnen Ausgang toggeln ---
void drv8904q1_toggle_output(uint8_t out_num) {
    uint16_t reg_val = drv8904q1_read_register(DRV8904Q1_REG_OUT_CTRL);
    if (out_num >= 1 && out_num <= 2) {
        reg_val ^= (1 << (out_num - 1));
        drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, reg_val);
    }
}
