/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : piezo_driver.c/.h Integration
  * @brief          : Piezo Driver with Time-based Beep + Sweep
  ******************************************************************************
  */
/* USER CODE END Header */

#include "tim.h"
#include "piezo_driver.h"

static uint32_t start_tick = 0;
static uint16_t duration = 0;
static uint8_t piezo_active = 0;
static uint8_t sweep_active = 0;

static uint16_t sweep_start_freq = 0;
static uint16_t sweep_end_freq = 0;
static uint16_t sweep_duration = 0;

void piezo_init(void) {
    HAL_TIM_PWM_Start(&htim14, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
    piezo_active = 0;
}

void piezo_play_beep(uint16_t frequency, uint16_t duration_ms) {
    uint32_t period = 1000000 / frequency;
    __HAL_TIM_SET_AUTORELOAD(&htim14, period - 1);
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, period / 2);
    start_tick = HAL_GetTick();
    duration = duration_ms;
    piezo_active = 1;
    sweep_active = 0;
}

void piezo_play_sweep(uint16_t start_freq, uint16_t end_freq, uint16_t duration_ms) {
    sweep_start_freq = start_freq;
    sweep_end_freq = end_freq;
    sweep_duration = duration_ms;
    start_tick = HAL_GetTick();
    duration = duration_ms;
    piezo_active = 1;
    sweep_active = 1;
}

void piezo_stop(void) {
    __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, 0);
    piezo_active = 0;
    sweep_active = 0;
}

void piezo_tick(void) {
    if (!piezo_active) return;

    uint32_t elapsed = HAL_GetTick() - start_tick;
    if (elapsed >= duration) {
        piezo_stop();
        return;
    }

    if (sweep_active) {
        float progress = (float)elapsed / sweep_duration;
        uint16_t freq = sweep_start_freq + (uint16_t)((sweep_end_freq - sweep_start_freq) * progress);
        if (freq < 100) freq = 100;
        uint32_t period = 1000000 / freq;
        __HAL_TIM_SET_AUTORELOAD(&htim14, period - 1);
        __HAL_TIM_SET_COMPARE(&htim14, TIM_CHANNEL_1, period / 2);
    }
}

void piezo_beep(uint16_t freq) {
    piezo_play_beep(freq, 80); // 80 ms als Standarddauer
}
