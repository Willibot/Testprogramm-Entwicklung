#ifndef LED_EFFECT_CONFIG_MODE_H
#define LED_EFFECT_CONFIG_MODE_H

#include <stdint.h>

// Initialisiert den Config-Mode-Effekt (setzt Startposition, Timer-Reset)
void led_effect_config_mode_start(void);

// Wird zyklisch aufgerufen, steuert das Blinken/Laufen im Config-Mode (tick = aktueller Zeitwert)
void led_effect_config_mode_update(uint32_t tick);

// Copilot/Entwickler-Hinweis:
// - Effektparameter (Farbe, Geschwindigkeit, Helligkeit) werden über effect_params bereitgestellt.
// - Keine blockierenden Delays, nur Timer-Vergleich im Update verwenden!
// - State-Variablen sind statisch im C-Modul zu halten.

#endif // LED_EFFECT_CONFIG_MODE_H
