// -----------------------------------------------------------------------------
// sound_single_sweep_1.h
// Zweck: Header für einfachen Soundsweep (4 kHz → 2 kHz, 100 ms) über Piezo-Buzzer.
// Dieses Modul kapselt die Schnittstelle für einen einzelnen Sweep-Soundeffekt.
//
// API:
//   - sound_single_sweep_1_start(): Startet den Sweep
//   - sound_single_sweep_1_update(): Muss zyklisch aufgerufen werden, steuert den Sweep
//   - sound_single_sweep_1_is_active(): Gibt zurück, ob der Sweep noch läuft
// -----------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <stdbool.h>

void sound_single_sweep_1_start(void);
void sound_single_sweep_1_update(void);
bool sound_single_sweep_1_is_active(void);