#ifndef LED_EFFECT_BLINK_H
#define LED_EFFECT_BLINK_H

#include <stdint.h>
#include <stdbool.h>

// Initialisiert den Blink-Effekt (setzt LEDs aus, Timer-Reset)
void led_effect_blink_start(void);

// Wird zyklisch aufgerufen, steuert das Blinken (tick = aktueller Zeitwert)
void led_effect_blink_update(uint32_t tick);

// Copilot/Entwickler-Hinweis:
// - Effektparameter (Farbe, Geschwindigkeit, Helligkeit) werden über effect_params bereitgestellt.
// - Keine blockierenden Delays, nur Timer-Vergleich im Update verwenden!
// - State-Variablen sind statisch im C-Modul zu halten.

#endif // LED_EFFECT_BLINK_H
