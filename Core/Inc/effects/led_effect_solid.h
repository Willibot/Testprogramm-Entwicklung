#ifndef LED_EFFECT_SOLID_H
#define LED_EFFECT_SOLID_H

#include <stdint.h>

// Initialisiert den statischen Farbeffekt (erzwingt Update beim ersten Tick)
void led_effect_solid_start(void);

// Setzt die Farbe, wenn Parameter sich ändern (tick = aktueller Zeitwert, meist ignoriert)
void led_effect_solid_update(uint32_t tick);

// Schaltet alle LEDs aus
void led_effect_solid_stop(void);

// Copilot/Entwickler-Hinweis:
// - Effektparameter (Farbe, Helligkeit) werden über effect_params bereitgestellt.
// - led_driver_update() nur bei Änderung der Parameter aufrufen!
// - Keine blockierenden Delays, nur State-Variablen im C-Modul.

#endif // LED_EFFECT_SOLID_H
