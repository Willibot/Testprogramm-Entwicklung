#ifndef LOGIC_CONFIG_H
#define LOGIC_CONFIG_H

#include <stdint.h>
#include <stdbool.h>

// Initialisiert den Konfigurationsmodus (Status, Parameter, etc.)
void logic_config_init(void);

// Zyklisches Update des Konfigurationsmodus (tick_ms = aktueller Zeitwert in ms)
void logic_config_update(uint32_t tick_ms);

// Verarbeitet ein Button-Event (event_type = z.B. PRESS, RELEASE, LONGPRESS)
void logic_config_button_event(uint8_t event_type);

// Copilot/Entwickler-Hinweis:
// - Dieses Modul kapselt die gesamte Logik für den Konfigurationsmodus.
// - Für neue Konfigurationsschritte: Logik in .c-Datei erweitern, ggf. eigene States/Parameter ergänzen.
// - Keine blockierenden Delays, nur State-Maschinen/Timer verwenden!

#endif // LOGIC_CONFIG_H
