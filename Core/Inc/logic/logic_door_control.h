#ifndef LOGIC_DOOR_CONTROL_H
#define LOGIC_DOOR_CONTROL_H

#include <stdint.h>

// Initialisiert die Türsteuerungs-Logik (Status, Hardware, etc.)
void logic_door_control_init(void);

// Zyklisches Update der Türsteuerung (z.B. im Mainloop aufrufen)
void logic_door_control_update(void);

// Verarbeitet ein Event für die Türsteuerung (z.B. Button, Sensor)
void logic_door_control_handle(uint8_t event);

// Copilot/Entwickler-Hinweis:
// - Dieses Modul kapselt die gesamte Logik für die Türsteuerung.
// - Für neue Events oder Zustände: Logik in .c-Datei erweitern, ggf. eigene States/Parameter ergänzen.
// - Keine blockierenden Delays, nur State-Maschinen/Timer verwenden!

#endif // LOGIC_DOOR_CONTROL_H
