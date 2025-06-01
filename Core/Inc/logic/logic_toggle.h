#ifndef LOGIC_TOGGLE_H
#define LOGIC_TOGGLE_H

#include <stdint.h>

// Initialisiert die Toggle-Logik (Status, Hardware, etc.)
void logic_toggle_init(void);

// Zyklisches Update der Toggle-Logik (z.B. im Mainloop aufrufen)
void logic_toggle_update(void);

// Verarbeitet ein Event für die Toggle-Logik (z.B. Button, Sensor)
void logic_toggle_handle(uint8_t event);

// Copilot/Entwickler-Hinweis:
// - Dieses Modul kapselt die gesamte Logik für einfache Umschaltvorgänge (Toggle).
// - Für neue Events oder Zustände: Logik in .c-Datei erweitern, ggf. eigene States/Parameter ergänzen.
// - Keine blockierenden Delays, nur State-Maschinen/Timer verwenden!

#endif // LOGIC_TOGGLE_H
