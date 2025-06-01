#ifndef LED_EFFECT_SINGLE_LED_H
#define LED_EFFECT_SINGLE_LED_H

#include <stdint.h>

// Initialisiert den Einzel-LED-Effekt (setzt Startwerte, Timer-Reset)
void led_effect_single_led_start(void);

// Wird zyklisch aufgerufen, steuert das Blinken der Einzel-LED (tick = aktueller Zeitwert)
void led_effect_single_led_update(uint32_t tick);

// Copilot/Entwickler-Hinweis:
// - Effektparameter (Farbe, Geschwindigkeit, Helligkeit) werden über effect_params bereitgestellt.
// - Keine blockierenden Delays, nur Timer-Vergleich im Update verwenden!
// - State-Variablen sind statisch im C-Modul zu halten.

#endif // LED_EFFECT_SINGLE_LED_H
