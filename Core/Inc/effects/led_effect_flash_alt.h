#ifndef LED_EFFECT_FLASH_ALT_H
#define LED_EFFECT_FLASH_ALT_H

#include <stdint.h>

// Initialisiert den alternierenden Flash-Effekt (setzt Startwerte, Timer-Reset)
void led_effect_flash_alt_start(void);

// Wird zyklisch aufgerufen, steuert das Umschalten zwischen den Farben (tick = aktueller Zeitwert)
void led_effect_flash_alt_update(uint32_t tick);

// Copilot/Entwickler-Hinweis:
// - Effektparameter (Farbe, Geschwindigkeit, Helligkeit) werden über effect_params bereitgestellt.
// - Keine blockierenden Delays, nur Timer-Vergleich im Update verwenden!
// - State-Variablen sind statisch im C-Modul zu halten.

#endif // LED_EFFECT_FLASH_ALT_H
