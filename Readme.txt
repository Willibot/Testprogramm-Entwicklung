# Modulares STM32 Sensortaster-Projekt

Dieses Projekt implementiert einen konfigurierbaren kapazitiven Sensortaster mit 12 RGB-LEDs (SK6812), Piezo-Tonrückmeldung und konfigurierbarem Verhalten.

## Architektur

- `main.c` → initialisiert Treiber, startet Loop
- `led_effect_engine.*` → verwaltet LED-Effekte (solid, blink, breathe, etc.)
- `sound_engine.*` → verwaltet Piezo-Soundeffekte (beep, double_beep, etc.)
- `logic_engine.*` → verarbeitet Tasterereignisse und setzt Effekte
- `led_driver.*` → DMA/PWM Ausgabe an SK6812-LEDs
- `piezo_driver.*` → PWM-basierte Tonsignalausgabe

## Ziel

Das System erlaubt LED-/Sound-/Schaltlogik modular zu kombinieren und runtime zu ändern. Ideal für Touch-Taster, Industrieanwendungen oder designorientierte HMI-Projekte.
