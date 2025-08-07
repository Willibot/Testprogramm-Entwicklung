#ifndef CONFIG_H
#define CONFIG_H

#include <stdbool.h>
#include <stdint.h>

// === Globale Ausgänge ===
// Array für den Zustand der drei Ausgänge (z.B. Relais, Motoren, LEDs).
// Jeder Eintrag steht für einen Ausgang. true = EIN, false = AUS.
extern bool output_state[3];

// === Button-Event-Typen ===
// Definition für einen kurzen Tastendruck (z.B. für Event-Handling).
#define BUTTON_EVENT_SHORT 1

// === Systemkonfiguration ===
// Anzahl der LEDs im System (z.B. für LED-Stripe oder Matrix).
#define NUM_LEDS             12
// Anzahl der Ausgänge (z.B. Relais, Motoren, etc.).
#define NUM_OUTPUTS          3

// === Debug-Pins ===
// Aktivierung der Debug-Pins für Messungen oder Fehleranalyse.
// 1 = aktiviert, 0 = deaktiviert.
#define DEBUG_PIN_A8         1
#define DEBUG_PIN_A11        1
#define DEBUG_PIN_A12        1

// === LED-Standardwerte ===
// Standard-Farbton (Hue) für LEDs beim Start (z.B. 0 = Rot).
#define DEFAULT_HUE          0       // Rot
// Standard-Helligkeit (0..255) für LEDs beim Start.
#define DEFAULT_BRIGHTNESS   128     // 0..255
// Standard-Geschwindigkeit für Effekte (1..20).
#define DEFAULT_SPEED        10      // 1..20 (Effektgeschwindigkeit)
// Standard-Dauer für Effekte (0 = unendlich).
#define DEFAULT_DURATION     0       // 0 = unendlich

// === Standard-Effekt beim Start ===
// Effekt, der beim Systemstart angezeigt wird.
#define DEFAULT_EFFECT       LED_EFFECT_SOLID

// === Aktivierung einzelner Module ===
// Mit diesen Schaltern können einzelne Funktionsbereiche aktiviert/deaktiviert werden.
// 1 = aktiviert, 0 = deaktiviert.
#define USE_SOUND_ENGINE     1
#define USE_PIEZO_DRIVER     1
#define USE_LOGIC_ENGINE     1
#define USE_LED_EFFECTS      1
#define USE_I2C_CY8CMBR3108_READ   1   // 1 = Lesen vom CY8 erlaubt
#define USE_I2C_CY8CMBR3108_WRITE  1   // 1 = Schreiben/Konfiguration erlaubt

// === Konfigurations-Modus freischalten ===
// Aktiviert einen speziellen Modus für die Konfiguration über Tasten.
#define ENABLE_CONFIG_MODE   1

// === Soundzuordnung ===
// Zuordnung von Soundeffekten zu bestimmten Events.
#define SOUND_EVENT_DEFAULT  SOUND_BEEP
#define SOUND_EVENT_CONFIRM  SOUND_DOUBLE_BEEP
#define SOUND_EVENT_CONFIG   SOUND_CONFIG_MODE

// === LED-Typ und Timing (z.B. für SK6812/WS2812) ===
// Auswahl des LED-Typs und der PWM-Frequenz für die Ansteuerung.
#define LED_TYPE_SK6812      1
#define LED_PWM_FREQ_HZ      800000   // 800kHz für SK6812/WS2812

// === Effekt-Parameterstruktur ===
// Struktur für die Übergabe von Effektparametern (Farbe, Helligkeit, Geschwindigkeit, Dauer).
typedef struct {
    uint8_t hue;         // Farbton (0..255)
    uint8_t brightness;  // Helligkeit (0..255)
    uint8_t speed;       // Geschwindigkeit (1..20)
    uint8_t duration;    // Dauer (0 = unendlich)
} effect_params_t;

// === LED-Buffer-Größe ===
// Anzahl der LEDs für Buffer-Definitionen.
#define LED_COUNT            NUM_LEDS

// === Effekt-Konfigurationsmodus ===
// Effekt-ID für den Konfigurationsmodus.
#define EFFECT_CONFIG_MODE LED_EFFECT_CONFIG_MODE

// === Tastenüberwachung (CS0, CS1, CS5, CS6) ===
// Diese Maske gibt an, welche Tasten überwacht werden sollen.
// Bit 0 = CS0, Bit 1 = CS1, Bit 5 = CS5, Bit 6 = CS6
// Du kannst die Maske anpassen, falls du andere Tasten überwachen willst.
// Die Maske wird verwendet, um aus dem Status-Register des CY8CMBR3108
// nur die gewünschten Tastenbits auszuwerten.
#define BUTTON_MASK ((1<<0)|(1<<1)|(1<<5)|(1<<6))

// Zeit in Millisekunden, wie lange eine Taste gehalten werden muss,
// damit ein Soundeffekt ausgelöst wird.
// Beispiel: 10000 = 10 Sekunden Halten für einen Spezialeffekt
#define BUTTON_HOLD_TIME_MS 10000

// === Globale Variablen für die Tastenüberwachung ===

// Dieses Flag wird vom Interrupt gesetzt, wenn ein Touch-Event erkannt wurde.
// Das Hauptprogramm prüft dieses Flag regelmäßig und setzt es dann zurück.
// Vorteil: Die eigentliche Auswertung passiert nicht im Interrupt, sondern im Mainloop.
// Das ist sicherer und entspricht der Cypress-Empfehlung.
extern volatile bool touch_event_pending;

// Zeitstempel für jede Taste (CS0-CS7), um die Haltedauer zu messen.
// Nur die Tasten aus BUTTON_MASK werden tatsächlich genutzt.
// Beispiel: button_press_timestamp[0] ist für CS0, [1] für CS1, [5] für CS5, [6] für CS6.
// Die anderen Einträge bleiben ungenutzt, stören aber nicht.
extern uint32_t button_press_timestamp[8];

#endif // CONFIG_H
