#include <stdbool.h>
#include "logic_config.h"
#include "config.h"
#include "led_effect_engine.h"
#include "sound_engine.h"
#include "stm32g0xx_hal.h" // Für HAL_GetTick()

// Maximale Dauer des Konfigurationsmodus in Millisekunden (10 Sekunden)
#define CONFIG_TIMEOUT_MS 10000

// Interne Zustandsvariablen für den Konfigurationsmodus
static uint32_t config_start_time = 0; // Zeitpunkt des Starts des Modus
static uint8_t config_step = 0;        // Schritt im Konfigurationsablauf
extern bool config_mode;               // Flag: Ist der Konfigurationsmodus aktiv?

/**
 * @brief Initialisiert den Konfigurationsmodus.
 * - Setzt das Flag, dass der Modus aktiv ist.
 * - Setzt den Schrittzähler zurück.
 * - Startet die Zeitmessung.
 * - Startet einen speziellen LED-Effekt und Sound zur Rückmeldung.
 */
void logic_config_init(void) {
    config_mode = true;
    config_step = 0;
    config_start_time = HAL_GetTick();

    led_effect_start(EFFECT_CONFIG_MODE); // LEDs zeigen Konfigurationsmodus an
    sound_play(SOUND_CONFIG_MODE);        // Akustische Rückmeldung
}

/**
 * @brief Muss regelmäßig im Mainloop aufgerufen werden.
 * - Prüft, ob der Konfigurationsmodus abgelaufen ist (Timeout).
 * - Beendet ggf. den Modus und stoppt den LED-Effekt.
 * @param tick_ms Aktueller System-Tick in Millisekunden
 */
void logic_config_update(uint32_t tick_ms) {
    if (config_mode && (tick_ms - config_start_time > CONFIG_TIMEOUT_MS)) {
        config_mode = false;
        led_effect_stop(); // LEDs wieder normal
    }
}

/**
 * @brief Wird aufgerufen, wenn im Konfigurationsmodus ein Button-Event erkannt wurde.
 * - Bei kurzem Tastendruck wird zum nächsten Konfigurationsschritt gewechselt.
 * - Je nach Schritt werden verschiedene Ausgänge gesetzt und Sounds abgespielt.
 * @param event_type Typ des Button-Events (z.B. kurz gedrückt)
 */
void logic_config_button_event(uint8_t event_type) {
    if (!config_mode) return; // Nur im Konfigurationsmodus reagieren

    if (event_type == BUTTON_EVENT_SHORT) {
        config_step = (config_step + 1) % 3; // Schrittzähler zyklisch erhöhen (0,1,2)

        switch (config_step) {
            case 0:
                // Beispiel: Ausgang 1 aktivieren, Ausgang 2 deaktivieren
                output_state[0] = true;
                output_state[1] = false;
                sound_play(SOUND_BEEP); // Einfacher Piepton
                break;
            case 1:
                // Ausgang 1 deaktivieren, Ausgang 2 aktivieren
                output_state[0] = false;
                output_state[1] = true;
                sound_play(SOUND_DOUBLE_BEEP); // Doppelton
                break;
            case 2:
                // Beide Ausgänge deaktivieren
                output_state[0] = false;
                output_state[1] = false;
                sound_play(SOUND_BEEP); // Einfacher Piepton
                break;
        }
    }
}
