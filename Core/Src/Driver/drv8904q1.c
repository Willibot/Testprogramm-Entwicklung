#include "Driver/drv8904q1.h"
#include "main.h"
#include "stm32g0xx_hal.h"
#include "spi.h"

// Chip-Select Pin
#define DRV8904Q1_CS_PIN   GPIO_PIN_0
#define DRV8904Q1_CS_PORT  GPIOB

// SPI Read/Write-Bit
#define DRV8904Q1_SPI_READ   (1U << 15)
#define DRV8904Q1_SPI_WRITE  (0U << 15)

// Registeradressen laut Datenblatt
#define DRV8904Q1_REG_OUT_CTRL     0x1
#define DRV8904Q1_REG_FAULT_STATUS 0x2

// Fault-Bits laut Datenblatt (Beispiele – Datenblatt prüfen!)
#define DRV8904Q1_FAULT_OVERCURRENT   (1U << 0)  // Überstrom
#define DRV8904Q1_FAULT_OVERTEMP      (1U << 1)  // Übertemperatur
#define DRV8904Q1_FAULT_UNDERVOLTAGE  (1U << 2)  // Unterspannung
#define DRV8904Q1_FAULT_OPENLOAD      (1U << 3)  // Last offen
#define DRV8904Q1_FAULT_SPI_ERROR     (1U << 4)  // SPI-Kommunikationsfehler
// Weitere Bits ggf. aus dem Datenblatt übernehmen

extern SPI_HandleTypeDef hspi1;

// --- Lokale Hilfsfunktionen ---
static void drv8904q1_select(void)   { HAL_GPIO_WritePin(DRV8904Q1_CS_PORT, DRV8904Q1_CS_PIN, GPIO_PIN_RESET); }
static void drv8904q1_deselect(void) { HAL_GPIO_WritePin(DRV8904Q1_CS_PORT, DRV8904Q1_CS_PIN, GPIO_PIN_SET); }

static uint16_t drv8904q1_spi_transfer(uint16_t data)
{
    uint8_t tx[2] = { (data >> 8) & 0xFF, data & 0xFF };
    uint8_t rx[2] = { 0, 0 };
    drv8904q1_select();
    HAL_SPI_TransmitReceive(&hspi1, tx, rx, 2, 10);
    drv8904q1_deselect();
    return (rx[0] << 8) | rx[1];
}

// --- Öffentliche API ---

void drv8904q1_init(void)
{
    // NSLEEP aktivieren (PB6)
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_Delay(2); // Mind. 1 ms laut Datenblatt

    // Optional: Outputs auf bekannten Zustand setzen
    drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, 0x0000);
}

uint16_t drv8904q1_read_register(uint8_t reg_addr)
{
    uint16_t cmd = DRV8904Q1_SPI_READ | ((reg_addr & 0xF) << 12);
    return drv8904q1_spi_transfer(cmd);
}

void drv8904q1_write_register(uint8_t reg_addr, uint16_t value)
{
    uint16_t cmd = DRV8904Q1_SPI_WRITE | ((reg_addr & 0xF) << 12) | (value & 0x0FFF);
    drv8904q1_spi_transfer(cmd);
}

uint16_t drv8904q1_read_output_state(void)
{
    return drv8904q1_read_register(DRV8904Q1_REG_OUT_CTRL) & 0x0FFF;
}

void drv8904q1_set_output(uint8_t out_num, uint8_t state)
{
    if (out_num < 1 || out_num > 8) return; // Schutz gegen falsche Werte
    uint16_t reg = drv8904q1_read_output_state();
    if (state)
        reg |= (1 << (out_num - 1));
    else
        reg &= ~(1 << (out_num - 1));
    drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, reg);
}

void drv8904q1_toggle_output(uint8_t out_num)
{
    if (out_num < 1 || out_num > 8) return;
    uint16_t reg = drv8904q1_read_output_state();
    reg ^= (1 << (out_num - 1));
    drv8904q1_write_register(DRV8904Q1_REG_OUT_CTRL, reg);
}

uint16_t drv8904q1_read_faults(void)
{
    return drv8904q1_read_register(DRV8904Q1_REG_FAULT_STATUS);
}

uint8_t drv8904q1_fault_active(uint16_t fault_mask)
{
    uint16_t faults = drv8904q1_read_faults();
    return (faults & fault_mask) ? 1 : 0;
}
