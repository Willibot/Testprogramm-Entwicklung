#include "drv8904q1.h"

// Chip-Select-Pin
#define DRV8904Q1_CS_PIN   GPIO_PIN_0
#define DRV8904Q1_CS_PORT  GPIOB

extern SPI_HandleTypeDef hspi1;

static void drv8904q1_select(void)   { HAL_GPIO_WritePin(DRV8904Q1_CS_PORT, DRV8904Q1_CS_PIN, GPIO_PIN_RESET); }
static void drv8904q1_deselect(void) { HAL_GPIO_WritePin(DRV8904Q1_CS_PORT, DRV8904Q1_CS_PIN, GPIO_PIN_SET); }

void drv8904q1_init(void) {
    // NSLEEP aktivieren (PB6)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
}

uint8_t drv8904q1_read_output_state(void) {
    uint8_t tx[2] = {DRV8904Q1_REG_OUT_CTRL, 0x00};
    uint8_t rx[2] = {0};
    drv8904q1_select();
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, HAL_MAX_DELAY);
    drv8904q1_deselect();
    return rx[1];
}

void drv8904q1_write_register(uint8_t reg, uint8_t value) {
    uint8_t tx[2] = {reg, value};
    drv8904q1_select();
    HAL_SPI_Transmit(&hspi1, tx, 2, HAL_MAX_DELAY);
    drv8904q1_deselect();
}

void drv8904q1_set_outputs(uint8_t pattern) {
    uint8_t reg = drv8904q1_read_output_state();
    reg &= ~0x0F;                // OUT1..OUT4 löschen
    reg |= (pattern & 0x0F);     // neues Muster
    drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, reg);
}

void drv8904q1_set_output(uint8_t out_num, uint8_t state) {
    if (out_num < 1 || out_num > 4) return; // nur OUT1..OUT4 gültig
    uint8_t reg = drv8904q1_read_output_state();
    if (state)
        reg |= (1 << (out_num - 1));
    else
        reg &= ~(1 << (out_num - 1));
    drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, reg);
}

void drv8904q1_toggle_output(uint8_t out_num) {
    if (out_num < 1 || out_num > 4) return;
    uint8_t reg = drv8904q1_read_output_state();
    reg ^= (1 << (out_num - 1));
    drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, reg);
}
