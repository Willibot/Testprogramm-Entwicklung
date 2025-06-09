#include <stdbool.h>
#include "logic_toggle.h"
#include "config.h"

extern bool output_state[3];

void logic_toggle_init(void) {
    // Optional: Reset Ausgangszustände
    output_state[0] = false;
}

void logic_toggle_update(void) {
    // In diesem Logiktyp keine zyklische Logik nötig
}

void logic_toggle_handle(uint8_t event) {
    // Toggle bei kurzem Tastendruck
    if (event == 1) { // 1 = Kurzer Tastendruck
        output_state[0] = !output_state[0];
    }
}
