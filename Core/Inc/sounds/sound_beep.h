// -----------------------------------------------------------------------------
// sound_beep.h
// Zweck: Header für einfachen Beep-Soundeffekt über Piezo-Buzzer.
// Dieses Modul kapselt die Schnittstelle für einen einzelnen kurzen Piezo-Ton.
//
// API:
//   - sound_beep_start(): Startet einen festen Beep mit gegebener Frequenz, Dauer und Lautstärke
//   - sound_beep_stop(): Stoppt den aktuellen Beep-Ton
//   - sound_beep_update(): Muss zyklisch aufgerufen werden, um den Ton zu steuern
//
// Hinweise für Entwickler & Copilot:
//   - Für weitere Soundeffekte (z.B. Melodien, Sweeps) eigenes Modul anlegen.
// -----------------------------------------------------------------------------

#pragma once

#ifndef SOUND_BEEP_H
#define SOUND_BEEP_H

#include <stdint.h>
#include <stdbool.h>

void sound_beep_start(uint16_t freq, uint16_t duration, uint8_t volume);
void sound_beep_stop(void);
void sound_beep_update(void);
bool sound_beep_is_active(void);

#endif