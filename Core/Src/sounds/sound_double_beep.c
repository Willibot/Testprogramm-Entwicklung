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

#include "sound_double_beep.h"
#include "piezo_driver.h"

static enum {
    DBEEP_IDLE,
    DBEEP_FIRST_BEEP,
    DBEEP_GAP,
    DBEEP_SECOND_BEEP
} state = DBEEP_IDLE;

static uint32_t timestamp = 0;

void sound_double_beep_start(uint16_t freq, uint16_t len_ms, uint16_t pause_ms) {
    piezo_on(freq);
    state = DBEEP_FIRST_BEEP;
    timestamp = timer_tick + len_ms;
}

void sound_double_beep_update(void) {
    switch (state) {
        case DBEEP_FIRST_BEEP:
            if (timer_tick >= timestamp) {
                piezo_off();
                state = DBEEP_GAP;
                timestamp = timer_tick + 50;  // Kurze Pause (optional: pause_ms)
            }
            break;

        case DBEEP_GAP:
            if (timer_tick >= timestamp) {
                piezo_on(4000); // zweite Beep-Frequenz (z.B. fix oder parametrierbar)
                state = DBEEP_SECOND_BEEP;
                timestamp = timer_tick + 80;
            }
            break;

        case DBEEP_SECOND_BEEP:
            if (timer_tick >= timestamp) {
                piezo_off();
                state = DBEEP_IDLE;
            }
            break;

        default:
            break;
    }
}

bool sound_double_beep_is_active(void) {
    return state != DBEEP_IDLE;
}

// Copilot:
// - Dieses Modul ist ein Template für kurze Doppelton-Signale (State-Maschine).
// - Für weitere Muster: Zustände, Timer und piezo_on/off anpassen.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer timer_tick!
