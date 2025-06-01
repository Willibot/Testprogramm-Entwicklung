#ifndef LOGIC_ENGINE_H
#define LOGIC_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    LOGIC_MODE_TOGGLE,
    LOGIC_MODE_DOOR_CONTROL,
    LOGIC_MODE_TIMER_SWITCH,
    LOGIC_MODE_CONFIG
} logic_mode_t;

// Initialisiert die zentrale Logik-Engine (Status, Subsysteme, etc.)
void logic_engine_init(void);

// Zyklisches Update der Logik-Engine (z.B. im Mainloop aufrufen)
void logic_engine_update(void);

// Verarbeitet ein globales Event für die Logik-Engine (z.B. Button, Sensor, Timeout)
void logic_engine_handle_event(uint8_t event);

// Optional: Für spezielle Button-Events
void logic_engine_handle_button_event(uint8_t event);

// Copilot/Entwickler-Hinweis:
// - Dieses Modul kapselt die zentrale Steuerung und Koordination aller Logik-Subsysteme.
// - Für neue Subsysteme: Hier und in der .c-Datei ergänzen, ggf. eigene States/Parameter verwalten.
// - Keine blockierenden Delays, nur State-Maschinen/Timer verwenden!

#endif // LOGIC_ENGINE_H
