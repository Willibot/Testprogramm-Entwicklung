#ifndef LOGIC_TIMER_SWITCH_H
#define LOGIC_TIMER_SWITCH_H

#include <stdint.h>
#include <stdbool.h>

// Initialisiert die Timer-Switch-Logik (Status, Timer, etc.)
void logic_timer_switch_init(void);

// Zyklisches Update der Timer-Switch-Logik (tick_ms = aktueller Zeitwert in ms)
void logic_timer_switch_update(uint32_t tick_ms);

// Löst den Timer-Switch aus (z.B. durch externes Event)
void logic_timer_switch_trigger(void);

// Event-Handler für Timer-Switch (z.B. Button-Event)
void logic_timer_switch_handle(uint8_t event);

// Copilot/Entwickler-Hinweis:
// - Dieses Modul kapselt die gesamte Logik für zeitgesteuerte Schaltvorgänge.
// - Für neue Timer- oder Schaltfunktionen: Logik in .c-Datei erweitern, ggf. eigene States/Parameter ergänzen.
// - Keine blockierenden Delays, nur State-Maschinen/Timer verwenden!

#endif // LOGIC_TIMER_SWITCH_H
