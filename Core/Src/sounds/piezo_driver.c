#include "piezo_driver.h"
#include "tim.h"      // Nutzung von TIM14 (PA4)
#include "main.h"

static uint32_t beep_end_time = 0;

void piezo_init(void) {
    HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
}

void piezo_beep(uint16_t freq_hz, uint16_t duration_ms) {
    uint32_t timer_clock = HAL_RCC_GetPCLK1Freq(); // z.B. 64 MHz
    uint32_t prescaler = 1;
    uint32_t period = (timer_clock / freq_hz) - 1;

    __HAL_TIM_SET_PRESCALER(&htim14, prescaler);
    __HAL_TIM_SET_AUTORELOAD(&htim14, period);
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, period / 2);

    beep_end_time = HAL_GetTick() + duration_ms;
}

void piezo_stop(void) {
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
}

void piezo_update(void) {
    if (beep_end_time && HAL_GetTick() >= beep_end_time) {
        piezo_stop();
        beep_end_time = 0;
    }
}
