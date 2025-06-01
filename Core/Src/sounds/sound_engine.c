// -----------------------------------------------------------------------------
// sound_engine.c
// Zweck: Zentrale Verwaltung und Steuerung aller Soundeffekte.
// Dieses Modul kapselt die Auswahl, Initialisierung und zyklische Ausführung
// von Soundeffekten (Beep, Double-Beep, Config-Mode, ...).
//
// API:
//   - sound_engine_set(sound_id_t sound_id, uint8_t repeat): Startet einen Soundeffekt (optional mehrfach)
//   - sound_engine_tick(): Muss zyklisch aufgerufen werden, steuert die Soundausgabe
//
// Hinweise für Entwickler & Copilot:
//   - Jeder Soundeffekt muss als eigenes Modul eingebunden werden und eine Play-Funktion bereitstellen.
//   - Erweiterung: Neue Sounds einfach ergänzen (enum, include, switch-case).
//   - Keine blockierenden Aufrufe! Nur State-Maschinen/Timer in den Sounds.
//   - Für komplexere Sequenzen: repeat-Logik und Sound-IDs erweitern.
//
// TODO: Optional: Priorisierung von Sounds, Queue für mehrere Soundereignisse
// -----------------------------------------------------------------------------

#include "sound_engine.h"
#include "sound_beep.h"
#include "sound_double_beep.h"
#include "sound_config_mode.h"
#include "config.h"

// Globale Zustände
static sound_id_t current_sound = SOUND_NONE;
static uint8_t sound_repeat_counter = 0;

void sound_engine_set(sound_id_t sound_id, uint8_t repeat) {
    current_sound = sound_id;
    sound_repeat_counter = repeat;
}

void sound_engine_tick(void) {
    if (current_sound == SOUND_NONE) return;

    switch (current_sound) {
        case SOUND_BEEP:
            sound_beep_play();
            break;
        case SOUND_DOUBLE_BEEP:
            sound_double_beep_play();
            break;
        case SOUND_CONFIG_MODE:
            sound_config_mode_play();
            break;
        default:
            break;
    }

    if (--sound_repeat_counter == 0) {
        current_sound = SOUND_NONE;
    }
}

// Copilot:
// - Dieses Modul ist die zentrale Schaltstelle für alle Soundeffekte.
// - Neue Sounds: Header einbinden, Enum erweitern, Switch-Case ergänzen.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer repeat-Logik!
