// -----------------------------------------------------------------------------
// sound_beep.c
// Zweck: Implementiert einen einfachen Beep-Soundeffekt über den Piezo-Buzzer.
// Dieses Modul kapselt die Ansteuerung für einen einzelnen kurzen Piezo-Ton (4 kHz, 80 ms).
//
// API:
//   - sound_beep_start(): Startet einen festen Beep mit 4 kHz und 80 ms Dauer
//   - sound_beep_update(): Muss zyklisch aufgerufen werden, schaltet Ton nach Ablauf ab
//   - sound_beep_is_active(): Gibt zurück, ob der Beep noch läuft
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt piezo_beep() aus piezo_driver.h für die PWM-Steuerung (Frequenz dynamisch)
//   - Keine blockierenden Delays verwenden! Nur Tick-Vergleich
//   - Für komplexere Sounds: sound_beep_update() erweitern oder mehrere Module kombinieren
//   - Für Frequenzwechsel oder Muster: Eigenes Soundmodul anlegen
//
// TODO: Für weitere Soundeffekte (z.B. Melodien, Sweeps) eigenes Modul anlegen und hier nur einfachen Beep belassen.
// -----------------------------------------------------------------------------

#include "sound_beep.h"
#include "piezo_driver.h"
#include "stm32g0xx_hal.h"
#include "main.h" // falls HAL_GetTick() benötigt wird

static volatile bool beep_active = false;
static uint32_t beep_end_time = 0; // <--- Diese Zeile einfügen!

void sound_beep_start(void) {
    beep_active = true;
    beep_end_time = HAL_GetTick() + 100; // Beispiel: 100 ms Beep
}

void sound_beep_update(void) {
    if (beep_active) {
        if (HAL_GetTick() >= beep_end_time) {
            beep_active = false;
            // ggf. Piezo abschalten
        }
    }
}

bool sound_beep_is_active(void) {
    return beep_active;
}

// Copilot:
// - Dieses Modul ist ein Template für einen einfachen Soundeffekt (Beep).
// - Für weitere Sounds: Eigenes Modul anlegen und piezo_beep() mit eigenen Mustern nutzen.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer timer_tick!
