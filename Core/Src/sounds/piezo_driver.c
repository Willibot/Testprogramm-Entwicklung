// -----------------------------------------------------------------------------
// piezo_driver.c
// Zweck: Ansteuerung des Piezo-Buzzers über PWM (TIM14, PA4).
// Dieses Modul kapselt die Erzeugung von Tönen/Sounds für akustisches Feedback.
//
// API:
//   - piezo_init(): Initialisiert den PWM-Ausgang (startet Timer, schaltet Ton aus)
//   - piezo_beep(freq_hz, duration_ms): Spielt einen Ton mit gegebener Frequenz und Dauer
//   - piezo_stop(): Schaltet den Ton sofort aus
//   - piezo_update(): Muss zyklisch aufgerufen werden, um den Ton nach Ablauf zu stoppen
//
// Hinweise für Entwickler & Copilot:
//   - piezo_update() sollte im Mainloop oder Timer-Interrupt regelmäßig aufgerufen werden!
//   - piezo_beep() kann mehrfach hintereinander aufgerufen werden, der letzte Ton gewinnt.
//   - Die Frequenz wird über Timer-Reload eingestellt, Duty-Cycle ist immer 50%.
//   - Keine blockierenden Delays verwenden! Nur Tick-Vergleich.
//   - Für komplexere Sounds (z.B. Melodien): piezo_update() erweitern.
//
// TODO: Optional: Queue für mehrere Sounds, verschiedene Duty-Cycles für andere Klangfarben
// -----------------------------------------------------------------------------

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
    uint32_t prescaler = 0; // Volle Clock!
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

// Copilot:
// - Dieses Modul ist die einzige Stelle, an der direkt auf den Piezo/TIM14 zugegriffen wird.
// - Für neue Soundeffekte: piezo_beep() mehrfach mit unterschiedlichen Parametern aufrufen oder erweitern.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer HAL_GetTick()!
