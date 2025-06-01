# Modulares STM32 Sensortaster-Projekt

Dieses Projekt implementiert einen konfigurierbaren kapazitiven Sensortaster mit 12 RGB-LEDs (SK6812), Piezo-Tonrückmeldung und flexibler Logiksteuerung. Es eignet sich für Touch-Taster, Industrieanwendungen oder designorientierte HMI-Projekte.

---

## Architektur

- **main.c**  
  Initialisiert alle Treiber und startet die Hauptschleife.
- **led_effect_engine.\***  
  Verwalten und Umschalten von LED-Effekten (solid, blink, breathe, chase, etc.).
- **sound_engine.\***  
  Erzeugt Piezo-Soundeffekte (beep, double_beep, confirm, etc.).
- **logic_engine.\***  
  Zentrale Logik: verarbeitet Taster-, Sensor- und Timer-Events, steuert Effekte und Ausgänge.
- **led_driver.\***  
  DMA/PWM-Ausgabe an SK6812-LEDs, abstrahiert LED-Hardware.
- **piezo_driver.\***  
  PWM-basierte Tonsignalausgabe für Piezo-Buzzer.
- **config.h**  
  Zentrale Projektkonfiguration (Anzahl LEDs, Standardwerte, Feature-Schalter).
- **effects/\***  
  Einzelne LED-Effekt-Module (z.B. blink, breathe, chase, config mode).
- **logic/\***  
  Einzelne Logik-Module (toggle, door control, timer switch, config mode).

---

## Features

- **Modulare LED-Effekte**: Verschiedene Effekte einfach kombinierbar und erweiterbar.
- **Soundausgabe**: Akustische Rückmeldung für Benutzerinteraktion.
- **Konfigurierbare Logik**: Verschiedene Betriebsmodi (Toggle, Türsteuerung, Timer, Konfigurationsmodus).
- **DMA/PWM für LEDs**: Flackerfreie, effiziente Ansteuerung von SK6812/WS2812.
- **Einfache Erweiterbarkeit**: Neue Effekte und Logik-Module können leicht ergänzt werden.
- **Debug-Pins**: Optional für Entwicklung und Fehlersuche aktivierbar.

---

## Ziel

Das System erlaubt es, LED-, Sound- und Schaltlogik modular zu kombinieren und zur Laufzeit zu ändern.  
Ideal für kapazitive Touch-Taster, Industrie-Bedienfelder oder designorientierte HMI-Projekte.

---

## Getting Started

1. **Hardware anschließen:**  
   - STM32G0-Controller  
   - 12x SK6812-LEDs  
   - Piezo-Buzzer  
   - Optional: Taster, Relais, Debug-Pins

2. **Projekt in STM32CubeIDE öffnen und kompilieren.**

3. **Firmware flashen und testen.**

---

## Hinweise für Entwickler

- **Eigene Effekte/Logik:**  
  Neue Module im jeweiligen Verzeichnis anlegen und in die Engine einbinden.
- **Konfiguration:**  
  Zentrale Parameter in `config.h` anpassen.
- **CubeMX:**  
  Änderungen an Hardware-Pins oder Peripherie immer in CubeMX und im Code synchron halten.
- **USER CODE Bereiche:**  
  Eigene Anpassungen in den `USER CODE`-Bereichen vornehmen, damit sie bei CubeMX-Exports erhalten bleiben.

---

## Lizenz

Siehe LICENSE-Datei im Projektverzeichnis.  
Wenn keine Lizenz beiliegt, gilt: Verwendung auf eigene Gefahr, keine Gewährleistung.

---
