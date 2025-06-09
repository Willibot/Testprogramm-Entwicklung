#include "logic_door_control.h"
#include "config.h"
#include <stdbool.h>   // <-- hinzufügen

extern bool output_state[3];
static uint8_t door_state = 0; // 0 = geschlossen, 1 = halb, 2 = offen

void logic_door_control_init(void) {
    door_state = 0;
    output_state[0] = true;   // Tür zu
    output_state[1] = false;
    output_state[2] = false;
}

void logic_door_control_update(void) {
    // Keine zyklische Logik nötig
}

void logic_door_control_handle(uint8_t event) {
    if (event == 1) { // kurzer Tastendruck
        door_state = (door_state + 1) % 3;

        output_state[0] = (door_state == 0); // Tür zu
        output_state[1] = (door_state == 1); // Tür halb offen
        output_state[2] = (door_state == 2); // Tür offen
    }
}
