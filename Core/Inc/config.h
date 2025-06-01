#ifndef CONFIG_H
#define CONFIG_H

// === Systemkonfiguration ===
#define NUM_LEDS             12
#define NUM_OUTPUTS          3

// === Debug-Pins ===
#define DEBUG_PIN_A8         1
#define DEBUG_PIN_A11        1
#define DEBUG_PIN_A12        1

// === LED-Standardwerte ===
#define DEFAULT_HUE          0       // Rot
#define DEFAULT_BRIGHTNESS   128     // 0..255
#define DEFAULT_SPEED        10      // 1..20 (Effektgeschwindigkeit)
#define DEFAULT_DURATION     0       // 0 = unendlich

// === Standard-Effekt beim Start ===
#define DEFAULT_EFFECT       LED_EFFECT_SOLID

// === Aktivierung einzelner Module ===
#define USE_SOUND_ENGINE     1
#define USE_PIEZO_DRIVER     1
#define USE_LOGIC_ENGINE     1
#define USE_LED_EFFECTS      1

// === Konfigurations-Modus freischalten ===
#define ENABLE_CONFIG_MODE   1

// === Soundzuordnung ===
#define SOUND_EVENT_DEFAULT  SOUND_BEEP
#define SOUND_EVENT_CONFIRM  SOUND_DOUBLE_BEEP
#define SOUND_EVENT_CONFIG   SOUND_CONFIG_MODE

// === LED-Typ und Timing (z.B. für SK6812/WS2812) ===
#define LED_TYPE_SK6812      1
#define LED_PWM_FREQ_HZ      800000   // 800kHz für SK6812/WS2812

// === Effekt-Parameterstruktur ===
typedef struct {
    uint8_t hue;
    uint8_t brightness;
    uint8_t speed;
    uint8_t duration;
} effect_params_t;

// === LED-Buffer-Größe ===
#define LED_COUNT            NUM_LEDS

// Copilot/Entwickler-Hinweis:
// - Diese Datei enthält zentrale Projektparameter und Schalter für alle Module.
// - Für neue Features: Hier zentral konfigurieren und in den Modulen verwenden.
// - Keine Magic Numbers im Code – immer Defines aus config.h nutzen!

#endif // CONFIG_H
