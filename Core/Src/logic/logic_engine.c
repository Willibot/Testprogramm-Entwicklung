// -----------------------------------------------------------------------------
// logic_engine.c
// Zweck: Zentrale Steuerung und Verteilung von Logik-Ereignissen im System.
// Dieses Modul kapselt die Initialisierung, zyklische Aktualisierung und
// Ereignisverarbeitung für alle Logikmodi (Toggle, Door Control, Timer Switch, Config).
//
// API:
//   - logic_engine_init(): Initialisiert alle Logikmodule
//   - logic_engine_update(): Zyklische Aktualisierung je nach aktuellem Logikmodus
//   - logic_engine_handle_button_event(uint8_t event): Verteilt Button-Events an das aktive Logikmodul
//
// Hinweise für Entwickler & Copilot:
//   - Die Logikmodi sind über das Enum logic_mode_t definiert.
//   - Jedes Logikmodul (Toggle, Door Control, Timer Switch, Config) kapselt eigene Logik und Effekte.
//   - Die Engine sorgt für Modularität und klare Trennung der Modi.
//   - Erweiterung: Neue Logikmodi können einfach ergänzt werden (Enum, Switch-Case, Modul).
// -----------------------------------------------------------------------------

#include "logic_engine.h"
#include "logic_toggle.h"
#include "logic_door_control.h"
#include "logic_timer_switch.h" // Enthält logic_timer_switch_handle
#include "logic_config.h"

extern logic_mode_t current_logic_mode;
extern uint32_t timer_tick;

void logic_engine_init(void) {
    // Initialisiert alle Logikmodule
    logic_toggle_init();
    logic_door_control_init();
    logic_timer_switch_init();
    logic_config_init();
}

void logic_engine_update(void) {
    // Zyklische Aktualisierung je nach aktuellem Logikmodus
    switch (current_logic_mode) {
        case LOGIC_MODE_TOGGLE:
            logic_toggle_update();
            break;
        case LOGIC_MODE_DOOR_CONTROL:
            logic_door_control_update();
            break;
        case LOGIC_MODE_TIMER_SWITCH:
            logic_timer_switch_update(timer_tick);
            break;
        case LOGIC_MODE_CONFIG:
            logic_config_update(timer_tick);
            break;
    }
}

void logic_engine_handle_button_event(uint8_t event) {
    // Verteilt Button-Events an das aktive Logikmodul
    switch (current_logic_mode) {
        case LOGIC_MODE_TOGGLE:
            logic_toggle_handle(event);
            break;
        case LOGIC_MODE_DOOR_CONTROL:
            logic_door_control_handle(event);
            break;
        case LOGIC_MODE_TIMER_SWITCH:
            logic_timer_switch_handle(event);
            break;
        case LOGIC_MODE_CONFIG:
            logic_config_handle(event);
            break;
    }
}

// Copilot:
// - Dieses Modul ist der zentrale Dispatcher für Logik und Button-Events.
// - Die eigentliche Logik und
