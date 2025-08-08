# 🧠 KI-Aufforderung zur projektweiten Analyse (für GPT & Copilot)

Dieses Projekt besteht aus mehreren Modulen mit vielen voneinander abhängigen Funktionen, Variablen, Timings und GPIO-Zuweisungen. Ziel ist eine vollständige Analyse und Änderungsempfehlung auf **Projekt-Ebene**, nicht auf Datei-Ebene isoliert.

---

## 🧭 Ziel

**Vollständige, projektübergreifende Prüfung auf:**

- ✅ **Konsistenz aller Funktions- und Variablennamen**
- ✅ **Logische Integrität der Zustandsmaschinen und Events**
- ✅ **Synchronität zwischen LED-, Sound- und Ausgangssteuerung**
- ✅ **Zeitliches Verhalten bei Button-Interaktionen**
- ✅ **SPI-, I2C-, PWM- und GPIO-Funktionalität**
- ✅ **Fehlende oder nicht angebundene Funktionsaufrufe**
- ✅ **Dead Code, ungenutzte Variablen oder mehrfach definierte Logik**
- ✅ **ISR-Tauglichkeit (Interrupts, Race Conditions vermeiden)**

---

## 🔍 Analysevorgaben

1. **Referenzstruktur**  
   → Die projektweite Funktionsübersicht (siehe unten) ist autoritative Quelle.  
   → Keine Funktion darf unregistriert auftreten oder verändert werden, ohne dass sie in dieser Übersicht aktualisiert ist.

2. **Abhängigkeiten auflösen**  
   → Jede vorgeschlagene Änderung muss **projektweit gedacht sein**.  
   → Liste ALLE betroffenen Module mit.  
   → Vorschläge dürfen **nicht zu temporären Inkonsistenzen** führen (wie es GitHub Copilot oft tut).

3. **Benennungspflicht & Dokumentation**  
   → Neue Funktions- oder Variablennamen sind **nur erlaubt mit Erklärung und tabellarischer Ergänzung**.

4. **Kontextsensitive Logik**  
   → Zeitsteuerung (z. B. bei `hold`-Effekten oder `double_beep`) ist **integraler Bestandteil der Funktion**.  
   → Prüfe, ob Timestamps und Statusflags richtig ausgewertet, rückgesetzt und weitergeleitet werden.

5. **Fehlertoleranz und Rückfallzustände**  
   → Sollte ein Effekt abgebrochen, doppelt aufgerufen oder inkonsistent gestartet werden, ist dies als Fehler zu werten.

---

## 📋 Projektweite Funktions- und Variablenübersicht (autorisiert)

Diese Tabelle wurde zuletzt durch GPT validiert und erweitert:

👉 *Bitte für jede Code-Analyse auf diese Übersicht referenzieren und Abweichungen markieren!*

→ [siehe Canvas-Datei `Projekt Funktionsuebersicht`]  
→ oder referenziere die Datei `Projekt-Funktionsuebersicht.txt`

---

## 🧪 Typischer Prüfablauf (Phase 1–3)

1. **Phase 1: Analyse**
   - Lies alle Dateien ein.
   - Erzeuge eine konsolidierte interne Darstellung aller Funktionen, Zustände und GPIOs.
   - Prüfe Timing, Initialisierung, Flag-Handling.

2. **Phase 2: Bewertung**
   - Ermittle Inkonsistenzen.
   - Suche fehlende Übergänge, vergessene Rücksetzungen oder konkurrierende Zustände.

3. **Phase 3: Maßnahmenplan**
   - Liste ALLE nötigen Änderungen vollständig (nicht inkrementell).
   - Ordne sie den Dateien zu.
   - Gib Begründung und Korrekturziel je Änderung an.

---

## 📂 Pfade & Module (zur Referenz)

- `/main.c`
- `/Driver/cy8cmbr3108.c`
- `/Driver/cy8cmbr3108_config.c`
- `/Driver/drv8904q1.c`
- `/effects/led_effect_engine.c`
- `/effects/led_effect_multibutton_double_blink.c`
- `/effects/led_effect_hold_multibutton_chase_left.c`
- `/sounds/sound_engine.c`
- `/sounds/sound_beep.c`
- `/sounds/sound_double_beep.c`
- `/sounds/sound_single_sweep_1.c`

---

## 📍 Beispielbefehl für Analyse-Start

