# Flag-Übersicht (Status- und Kontroll-Flags)

Diese Datei dokumentiert alle globalen und wichtigen lokalen Flags, die im Projekt zur Status- und Ablaufsteuerung verwendet werden.  
Sie dient als zentrale Referenz für die Entwicklung, Wartung und Erweiterung der Firmware.

---

## Globale Flags

| Flag-Name                | Typ             | Bedeutung / Verwendung                                  | Modul / Datei              |
|--------------------------|-----------------|---------------------------------------------------------|----------------------------|
| global_abort_requested   | volatile bool   | Globales Abbruch-/Reset-Flag für Rückfall in Grundzustand (Eingabeerwartung) | main.c, alle Module        |
| effect_active            | volatile bool   | Ist ein LED-Effekt aktiv?                               | main.c, effects/           |
| hold_chase_effect_active | volatile bool   | Läuft gerade der Hold-Chase-Effekt?                     | main.c, effects/           |
| any_button_pressed       | bool            | Ist aktuell mindestens eine Taste gedrückt?             | main.c                     |
| double_beep_played       | bool            | Wurde der Doppelbeep schon gespielt?                    | main.c, sounds/            |
| hold_effect_active[8]    | bool[]          | Pro Taste: Ist Hold-Effekt aktiv?                       | main.c, effects/           |
| touch_event_count        | volatile uint8_t| Zählt Touch-Interrupts (Event-Queue)                    | main.c, cy8cmbr3108.c      |

---

## Lokale/Modulinterne Flags (Beispiele)

| Flag-Name                | Typ             | Bedeutung / Verwendung                                  | Modul / Datei              |
|--------------------------|-----------------|---------------------------------------------------------|----------------------------|
| led_engine_busy          | bool            | LED-Engine ist mit Effekt beschäftigt                   | effects/led_effect_engine.c|
| sound_beep_active        | bool            | Beep-Sound läuft gerade                                | sounds/sound_beep.c        |
| ...                      | ...             | ...                                                     | ...                        |

---

## Hinweise zur Verwendung

- **Namensschema:**  
  Flags sollten sprechend und eindeutig benannt werden (`<funktion>_<status>`, z. B. `effect_active`).
- **Global vs. lokal:**  
  Globale Flags werden in der Übersicht dokumentiert und in Headern deklariert, lokale Flags nur im jeweiligen Modul.
- **Erweiterung:**  
  Neue Flags bitte hier eintragen