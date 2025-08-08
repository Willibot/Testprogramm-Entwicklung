// -----------------------------------------------------------------------------
// sound_double_beep.c
// Zweck: Implementiert einen doppelten Beep-Soundeffekt über den Piezo-Buzzer.
// Dieses Modul kapselt eine kurze Doppelton-Sequenz für akustisches Feedback.
//
// API:
//   - sound_double_beep_start(freq, len_ms, pause_ms): Startet die Doppelton-Sequenz
//   - sound_double_beep_update(): Muss zyklisch aufgerufen werden, steuert die Sequenz
//   - sound_double_beep_is_active(): Gibt zurück, ob die Sequenz noch läuft
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt piezo_on()/piezo_off() aus piezo_driver.h für die eigentliche PWM-Steuerung
//   - timer_tick muss regelmäßig aktualisiert werden (z.B. im SysTick-Handler)
//   - Keine blockierenden Delays verwenden! Nur Tick-Vergleich/State-Maschine
//   - Für andere Muster: Zustandsautomat und Parameter anpassen
//
// TODO: Optional: Verschiedene Frequenzen für beide Beeps, Pause parametrierbar machen
// -----------------------------------------------------------------------------

#include "sounds/sound_double_beep.h"
#include "sounds/piezo_driver.h"
#include "stm32g0xx_hal.h"

static volatile bool double_beep_active = false;
static uint8_t beep_phase = 0;
static uint32_t beep_end_time = 0;
static uint16_t last_freq = 0;
static uint16_t last_duration = 0;

void sound_double_beep_start(uint16_t freq, uint16_t duration)
{
    double_beep_active = true;
    beep_phase = 0;
    last_freq = freq;
    last_duration = duration;
    beep_end_time = HAL_GetTick() + duration;
    piezo_beep(freq);
}

void sound_double_beep_stop(void) {
    double_beep_active = false;
    piezo_stop();
}

void sound_double_beep_update(void) {
    if (!double_beep_active) return;

    if (HAL_GetTick() >= beep_end_time) {
        if (beep_phase == 0) {
            // Pause zwischen den Beeps
            beep_phase = 1;
            piezo_stop();
            beep_end_time = HAL_GetTick() + 80; // 80ms Pause
        } else if (beep_phase == 1) {
            // Zweiter Beep
            beep_phase = 2;
            piezo_beep(last_freq);
            beep_end_time = HAL_GetTick() + last_duration;
        } else {
            // Fertig
            double_beep_active = false;
            piezo_stop();
        }
    }
}

bool sound_double_beep_is_active(void) {
    return double_beep_active;
}

// Copilot:
// - Dieses Modul ist ein Template für kurze Doppelton-Signale (State-Maschine).
// - Für weitere Muster: Zustände, Timer und piezo_on/off anpassen.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer timer_tick!
