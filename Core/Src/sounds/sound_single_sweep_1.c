// -----------------------------------------------------------------------------
// sound_single_sweep_1.c
// Zweck: Erzeugt einen einfachen Soundsweep von 4 kHz auf 2 kHz in 100 ms über den Piezo-Buzzer.
// Dieses Modul kapselt die Ansteuerung für einen einzelnen Sweep-Soundeffekt.
//
// API:
//   - sound_single_sweep_1_start(): Startet den Sweep (4 kHz → 2 kHz, 100 ms)
//   - sound_single_sweep_1_update(): Muss zyklisch aufgerufen werden, steuert den Sweep
//   - sound_single_sweep_1_is_active(): Gibt zurück, ob der Sweep noch läuft
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt piezo_beep() aus piezo_driver.h für die PWM-Steuerung (Frequenz dynamisch)
//   - Keine blockierenden Delays verwenden! Nur Tick-Vergleich und State-Maschine
//   - Für andere Sweeps: Parameter und Logik anpassen
// -----------------------------------------------------------------------------

#include "sounds/sound_single_sweep_1.h"
#include "sounds/piezo_driver.h"
#include "stm32g0xx_hal.h"

static bool sweep_active = false;
static uint32_t sweep_start_time = 0;
static const uint32_t sweep_duration = 100; // ms

void sound_single_sweep_1_start(void) {
    sweep_active = true;
    sweep_start_time = HAL_GetTick();
    piezo_beep(4000, sweep_duration); // Start mit 4 kHz, Dauer 100 ms
}

void sound_single_sweep_1_update(void) {
    if (!sweep_active) return;
    uint32_t now = HAL_GetTick();
    uint32_t elapsed = now - sweep_start_time;
    if (elapsed >= sweep_duration) {
        piezo_stop();
        sweep_active = false;
        return;
    }
    // Frequenz linear von 4 kHz auf 2 kHz
    uint16_t freq = 4000 - ((2000 * elapsed) / sweep_duration);
    piezo_beep(freq, sweep_duration - elapsed);
}

bool sound_single_sweep_1_is_active(void) {
    return sweep_active;
}

void sound_single_sweep_1_stop(void) {
    piezo_stop(); // Piezo-Buzzer abschalten
}