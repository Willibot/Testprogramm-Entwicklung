# Zielstellung: Modularer Aufbau für Tasten- und LED-Logik

Ziel dieses Projekts ist die Entwicklung einer **modularen Firmware-Architektur**, die das im Dokument *Tastenverhalten-multibutton.txt* beschriebene Verhalten implementiert und flexibel erweitert werden kann.

---

## 1. Hauptprogramm (`main.c`)

- Enthält eine schlanke Hauptschleife mit:
    - Statusanzeige über grüne LED im Bereitschaftszustand.
    - Warteschleife auf Benutzereingaben (Ereignisorientierung).
- Reagiert auf Interrupts, indem ein separates Eingabemodul aufgerufen wird.
- Kapselt keine Logik, sondern delegiert an spezialisierte Module.

---

## 2. Eingabemodul

- Erkennt, welche Taste oder Tastenkombination gedrückt wurde.
- Unterstützt die Kombination der vier Sensorflächen in verschiedenen Betriebsmodi:
    1. **Vierfach-Betrieb:** Jede Fläche einzeln.
    2. **Zweifach-Betrieb links/rechts:** Sensorflächen werden logisch zu zwei Tasten zusammengefasst.
    3. **Zweifach-Betrieb oben/unten:** Sensorflächen werden logisch zu zwei Tasten zusammengefasst.
- Die Zuweisung der Sensorflächen zu logischen Tasten ist **konfigurierbar** über eine Einstellungsdatei oder ein Konfigurationsmodul.
- Ereignisorientierte Verarbeitung: Jeder Interrupt (Statusänderung) löst eine Statusabfrage und -auswertung aus.

---

## 3. Hardware-Begrenzungen

- Es stehen nur **vier Schaltzustände** zur Verfügung, die sich aus zwei Ausgängen (2²-Kombination) ergeben, ggf. zusätzlicher High-Z-Zustand.
- Die Schaltlogik ist unabhängig von der konkreten Hardware-Ansteuerung und kann für andere Aktoren angepasst werden.

---

## 4. Abstraktionsprinzip

- Die Erkennung und Logik der Tasten wird **hardwareunabhängig** in einem Abstraktionslayer implementiert.
- Das LED- und Ton-Feedback wird über separate Module gesteuert und kann ebenfalls ausgetauscht bzw. erweitert werden.
- Die Logik ist vollständig kapselbar und für andere Projekte wiederverwendbar.
- Die Architektur ist **erweiterbar**: Neue Betriebsmodi, zusätzliche Sensorflächen oder weitere Ausgabemodule (z. B. Display, weitere Aktoren) können mit minimalem Aufwand integriert werden.
- Die Schnittstellen der Module sind dokumentiert, um die Wiederverwendbarkeit und Testbarkeit zu unterstützen.

---

## 5. Zielarchitektur

- **main.c:** Steuerlogik, Zustandsverwaltung, Interruptregistrierung, Event-Dispatch.
- **Button-Modul:** Auswertung von Tastenereignissen (kurz, lang, keine Aktion) entsprechend der definierten Zeitfenster und Betriebsmodi.
- **Pattern-Modul:** Umsetzung der LED-Muster und Tonfolgen gemäß Tastenverhalten.
- **Treiber-Layer:** Direkte Ansteuerung von DRV8904Q1, LEDs, Audioausgabe, ggf. weitere Aktoren.
- **Konfigurationsmodul:** Interpretation der Betriebsmodi und Tastenmuster aus Konfigurationsdateien oder persistentem Speicher.
- **Testbarkeit:** Module sind so gestaltet, dass sie unabhängig voneinander getestet und simuliert werden können (z. B. Unit-Tests).

---

## 6. Ereignisorientierung und Modularität

- Die Architektur basiert auf einem **ereignisorientierten Ansatz**:  
  Hardware-Interrupts oder externe Ereignisse lösen gezielt die Verarbeitung in den jeweiligen Modulen aus.
- Die Trennung der Module gewährleistet, dass Änderungen am Tastenverhalten oder an den Betriebsmodi **ohne Anpassung der Hardware-Treiber** und ohne Eingriff in die Hauptschleife erfolgen können.
- Die Modularität ermöglicht eine einfache Wartung, Erweiterung und Wiederverwendung der einzelnen Komponenten.

---

**Zusammenfassung:**  
Diese Architektur stellt sicher, dass die Firmware flexibel, wartbar und zukunftssicher bleibt. Änderungen an der Logik, den Betriebsmodi oder der Hardware können unabhängig voneinander erfolgen, was die Entwicklung und Pflege des Projekts erheblich vereinfacht.

***
