#ifndef LED_EFFECT_MULTIBUTTON_DOUBLE_BLINK_H
#define LED_EFFECT_MULTIBUTTON_DOUBLE_BLINK_H

#include <stdint.h>

/**
 * @brief Startet den Multibutton-Doppelblink-Effekt.
 *
 * Diese Funktion initialisiert den Effekt, sodass die LEDs zweimal blinken.
 * Die Farbe (hue) und Helligkeit (brightness) werden übergeben und für beide Blinkzyklen verwendet.
 * Sie sollte immer dann aufgerufen werden, wenn ein Taster-Event einen neuen Blink-Effekt auslösen soll.
 *
 * @param hue        Farbwert im HSV-Farbraum (0-255), z.B. 0=Rot, 85=Grün, 170=Blau
 * @param brightness Helligkeit der LEDs (0-255)
 */
void led_effect_multibutton_double_blink_start(uint8_t hue, uint8_t brightness);

/**
 * @brief Aktualisiert den Multibutton-Doppelblink-Effekt.
 *
 * Diese Funktion muss regelmäßig (z.B. in jedem Mainloop-Durchlauf) aufgerufen werden,
 * damit der Effekt zeitgesteuert ablaufen kann. Sie schaltet die LEDs im 200ms-Takt an und aus,
 * insgesamt viermal (zwei Blinks). Nach Abschluss des Effekts bleiben die LEDs aus.
 *
 * @param tick Aktueller Zeitwert (z.B. von HAL_GetTick()), wird für das Timing verwendet.
 */
void led_effect_multibutton_double_blink_update(uint32_t tick);

#endif // LED_EFFECT_MULTIBUTTON_DOUBLE_BLINK_H