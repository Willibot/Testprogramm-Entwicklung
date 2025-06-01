// -----------------------------------------------------------------------------
// sound_beep.c
// Zweck: Implementiert einen einfachen Beep-Soundeffekt über den Piezo-Buzzer.
// Dieses Modul kapselt die Ansteuerung für einen einzelnen kurzen Piezo-Ton.
//
// API:
//   - sound_beep_start(duration_ms, frequency_hz): Startet einen Beep mit Dauer und Frequenz
//   - sound_beep_update(): Muss zyklisch aufgerufen werden, schaltet Ton nach Ablauf ab
//   - sound_beep_is_active(): Gibt zurück, ob der Beep noch läuft
//
// Hinweise für Entwickler & Copilot:
//   - Nutzt piezo_on()/piezo_off() aus piezo_driver.h für die eigentliche PWM-Steuerung
//   - timer_tick muss regelmäßig aktualisiert werden (z.B. im SysTick-Handler)
//   - Keine blockierenden Delays verwenden! Nur Tick-Vergleich
//   - Für komplexere Sounds: sound_beep_update() erweitern oder mehrere Module kombinieren
//
// TODO: Optional: Verschiedene Beep-Muster, Lautstärke-/Dutycycle-Parameter
// -----------------------------------------------------------------------------

#include "sound_beep.h"
#include "piezo_driver.h"

static bool beep_active = false;
static uint32_t beep_end_time = 0;

void sound_beep_start(uint16_t duration_ms, uint16_t frequency_hz) {
    piezo_on(frequency_hz);
    beep_end_time = timer_tick + duration_ms;
    beep_active = true;
}

void sound_beep_update(void) {
    if (beep_active && timer_tick >= beep_end_time) {
        piezo_off();
        beep_active = false;
    }
}

bool sound_beep_is_active(void) {
    return beep_active;
}

// Copilot:
// - Dieses Modul ist ein Template für einfache Soundeffekte.
// - Für weitere Sounds: Eigenes Modul anlegen und piezo_on()/piezo_off() mit eigenen Mustern nutzen.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer timer_tick!
