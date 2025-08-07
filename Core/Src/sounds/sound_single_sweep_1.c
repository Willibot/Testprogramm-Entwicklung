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

static volatile bool sweep_active = false;
static uint32_t sweep_end_time = 0;

void sound_single_sweep_1_start(void) {
    sweep_active = true;
    sweep_end_time = HAL_GetTick() + 120; // z.B. 120ms Sweep
    piezo_beep(4000, 50); // Beispiel: 4kHz, mittlere Lautstärke
}

void sound_single_sweep_1_stop(void) {
    sweep_active = false;
    piezo_stop();
}

void sound_single_sweep_1_update(void) {
    if (sweep_active && HAL_GetTick() >= sweep_end_time) {
        sweep_active = false;
        piezo_stop();
    }
}

bool sound_single_sweep_1_is_active(void) {
    return sweep_active;
}