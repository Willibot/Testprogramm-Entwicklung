// -----------------------------------------------------------------------------
// sound_beep.h
// Zweck: Header für einfachen Beep-Soundeffekt (4 kHz, 80 ms) über Piezo-Buzzer.
// Dieses Modul kapselt die Schnittstelle für einen einzelnen kurzen Piezo-Ton.
//
// API:
//   - sound_beep_start(): Startet einen festen Beep mit 4 kHz und 80 ms Dauer
//   - sound_beep_update(): Muss zyklisch aufgerufen werden, schaltet Ton nach Ablauf ab
//   - sound_beep_is_active(): Gibt zurück, ob der Beep noch läuft
//
// Hinweise für Entwickler & Copilot:
//   - Die Parameter für Frequenz und Dauer sind fest im Modul (siehe sound_beep.c).
//   - Für weitere Soundeffekte (z.B. Melodien, Sweeps) eigenes Modul anlegen.
// -----------------------------------------------------------------------------

#pragma once

#include <stdint.h>
#include <stdbool.h>

void sound_beep_start(void);
void sound_beep_update(void);
