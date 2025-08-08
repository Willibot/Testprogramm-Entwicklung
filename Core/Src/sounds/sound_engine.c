// -----------------------------------------------------------------------------
// sound_engine.c
// Zweck: Zentrale Verwaltung und Steuerung aller Soundeffekte.
// Dieses Modul kapselt die Auswahl, Initialisierung und zyklische Ausführung
// von Soundeffekten (Beep, Double-Beep, Config-Mode, ...).
//
// API:
//   - sound_engine_set(sound_id_t sound_id, uint8_t repeat): Startet einen Soundeffekt (optional mehrfach)
//   - sound_engine_play(sound_id_t sound): Startet einen Soundeffekt einmal
//   - sound_engine_tick(): Muss zyklisch aufgerufen werden, steuert die Soundausgabe
//   - sound_engine_init(): Initialisiert Piezo-Treiber
//
// Hinweise für Entwickler & Copilot:
//   - Jeder Soundeffekt muss als eigenes Modul eingebunden werden und eine Play-/Status-Funktion bereitstellen.
//   - Erweiterung: Neue Sounds einfach ergänzen (enum, include, switch-case).
//   - Keine blockierenden Aufrufe! Nur State-Maschinen/Timer in den Sounds.
//   - Für komplexere Sequenzen: repeat-Logik und Sound-IDs erweitern.
//   - Die zyklischen Update-Funktionen (sound_beep_update, ...) müssen im Mainloop aufgerufen werden!
//
// TODO: Optional: Priorisierung von Sounds, Queue für mehrere Soundereignisse
// -----------------------------------------------------------------------------

#include "sound_engine.h"
#include "sound_beep.h"
#include "sound_double_beep.h"
#include "sound_config_mode.h"
#include "config.h"
#include "piezo_driver.h"

// Globale Zustände
static sound_id_t current_sound = SOUND_NONE;
static uint8_t sound_repeat_counter = 0;

/**
 * @brief Setzt den aktuellen Soundeffekt und die Wiederholungsanzahl.
 */
void sound_engine_set(sound_id_t sound_id, uint8_t repeat) {
    current_sound = sound_id;
    sound_repeat_counter = repeat;
}

/**
 * @brief Startet einen Soundeffekt einmal.
 */
void sound_engine_play(sound_id_t sound) {
    sound_engine_set(sound, 1);
}

/**
 * @brief Zyklische Steuerung der Soundausgabe.
 * Muss regelmäßig im Mainloop aufgerufen werden!
 */
void sound_engine_tick(void) {
    if (current_sound == SOUND_NONE) return;

    switch (current_sound) {
        case SOUND_BEEP:
            // Nur starten, wenn kein Beep aktiv ist!
            if (!sound_beep_is_active()) {
                sound_beep_start(4000, 80); // Frequenz, Dauer, Lautstärke
                sound_repeat_counter--;
                if (sound_repeat_counter == 0) {
                    current_sound = SOUND_NONE;
                }
            }
            break;
        case SOUND_DOUBLE_BEEP:
            // Analog: Hier ggf. sound_double_beep_is_active() prüfen und sound_double_beep_start() aufrufen!
            break;
        case SOUND_CONFIG_MODE:
            // Analog: Hier ggf. sound_config_mode_is_active() prüfen und sound_config_mode_start() aufrufen!
            break;
        default:
            break;
    }
}

void sound_engine_init(void) {
    piezo_init();
}
