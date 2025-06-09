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
extern uint32_t timer_tick;

static enum {
    DBEEP_IDLE,
    DBEEP_FIRST_BEEP,
    DBEEP_GAP,
    DBEEP_SECOND_BEEP
} state = DBEEP_IDLE;

static uint32_t timestamp = 0;
static uint16_t freq1 = 4000;
static uint16_t len1 = 80;
static uint16_t pause = 50;
static uint16_t freq2 = 4000;
static uint16_t len2 = 80;

void sound_double_beep_start(uint16_t freq, uint16_t len_ms, uint16_t pause_ms) {
    freq1 = freq;
    len1 = len_ms;
    pause = pause_ms;
    freq2 = freq;   // Optional: zweiten Ton parametrierbar machen
    len2 = len_ms;  // Optional: zweite Länge parametrierbar machen

    piezo_beep(freq1, len1);
    state = DBEEP_FIRST_BEEP;
    timestamp = timer_tick + len1;
}

void sound_double_beep_update(void) {
    switch (state) {
        case DBEEP_FIRST_BEEP:
            if (timer_tick >= timestamp) {
                piezo_stop();
                state = DBEEP_GAP;
                timestamp = timer_tick + pause;
            }
            break;

        case DBEEP_GAP:
            if (timer_tick >= timestamp) {
                piezo_beep(freq2, len2);
                state = DBEEP_SECOND_BEEP;
                timestamp = timer_tick + len2;
            }
            break;

        case DBEEP_SECOND_BEEP:
            if (timer_tick >= timestamp) {
                piezo_stop();
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
