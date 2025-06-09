// -----------------------------------------------------------------------------
// led_effect_engine.c
// Zweck: Zentrale Verwaltung und Umschaltung aller LED-Effekte.
// Dieses Modul kapselt die Auswahl, Initialisierung und zyklische Aktualisierung
// der LED-Effekte. Jeder Effekt wird über eine einheitliche API angesprochen.
//
// API:
//   - led_effect_engine_init(): Setzt den Effekt-Status zurück
//   - led_effect_engine_set(effect, params): Aktiviert einen Effekt mit Parametern
//   - led_effect_engine_update(): Ruft zyklisch das Update des aktiven Effekts auf
//
// Hinweise für Entwickler & Copilot:
//   - Jeder Effekt muss als eigenes Modul eingebunden werden und eine Update-Funktion bereitstellen.
//   - Die Effekt-Parameter werden zentral verwaltet und an die Effekte übergeben.
//   - Erweiterung: Neue Effekte einfach ergänzen (enum, include, switch-case).
//   - Keine blockierenden Aufrufe! Nur State-Maschinen/Timer in den Effekten.
//   - Für dynamische Effekte: State-Variablen im jeweiligen Effekt-Modul halten.
//
// TODO: Optional: Effekt-Callbacks für Start/Stop, dynamische Registrierung von Effekten
// -----------------------------------------------------------------------------

#include "led_effect_engine.h"
#include "led_effect_solid.h"
#include "led_effect_blink.h"
#include "led_effect_breathe.h"
#include "led_effect_flash_alt.h"
#include "led_effect_chase_left.h"
#include "led_effect_chase_right.h"
#include "led_effect_config_mode.h"
#include "led_effect_single_led.h"

// KORREKTUR: static entfernen, damit die Definition zur extern-Deklaration im Header passt
led_effect_id_t current_effect = LED_EFFECT_NONE;

void led_effect_engine_init(void) {
    current_effect = LED_EFFECT_NONE;
}

void led_effect_engine_set(led_effect_id_t effect) {
    current_effect = effect;
}

void led_effect_engine_update(uint32_t tick) {
    switch (current_effect) {
        case LED_EFFECT_SOLID:
            led_effect_solid_update(tick);
            break;
        case LED_EFFECT_BLINK:
            led_effect_blink_update(tick);
            break;
        case LED_EFFECT_BREATHE:
            led_effect_breathe_update(tick);
            break;
        case LED_EFFECT_FLASH_ALT:
            led_effect_flash_alt_update(tick);
            break;
        case LED_EFFECT_CHASE_LEFT:
            led_effect_chase_left_update(tick);
            break;
        case LED_EFFECT_CHASE_RIGHT:
            led_effect_chase_right_update(tick);
            break;
        case LED_EFFECT_CONFIG_MODE:
            led_effect_config_mode_update(tick);
            break;
        case LED_EFFECT_SINGLE_LED:
            led_effect_single_led_update(tick);
            break;
        default:
            break;
    }
}

// Copilot:
// - Dieses Modul ist die zentrale Schaltstelle für alle LED-Effekte.
// - Neue Effekte: Header einbinden, Enum erweitern, Switch-Case ergänzen.
// - Effekt-Parameter werden zentral verwaltet und an die Effekte übergeben.
// - Keine blockierenden Delays, keine globale Abhängigkeit außer current_params!
