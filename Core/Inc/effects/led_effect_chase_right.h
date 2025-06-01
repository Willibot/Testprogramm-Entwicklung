#ifndef LED_EFFECT_CHASE_RIGHT_H
#define LED_EFFECT_CHASE_RIGHT_H

#include <stdint.h>

// Initialisiert den "Chase Right"-Effekt (setzt Startposition, Timer-Reset)
void led_effect_chase_right_start(void);

// Wird zyklisch aufgerufen, steuert das Laufen (tick = aktueller Zeitwert)
void led_effect_chase_right_update(uint32_t tick);

// Copilot/Entwickler-Hinweis:
// - Effektparameter (Farbe, Geschwindigkeit, Helligkeit) werden über effect_params bereitgestellt.
// - Keine blockierenden Delays, nur Timer-Vergleich im Update verwenden!
// - State-Variablen sind statisch im C-Modul zu halten.

#endif // LED_EFFECT_CHASE_RIGHT_H
