#define F_CPU 1000000UL

#include <util/delay.h>

#include "display.h"

int main() {

    initDisplay();

    uint32_t sec;

    while (1) {
        // Not the way to do it, use RTC later
        for (sec = 77280; sec < 86400; sec++) {
            int hours = sec / 3600;
            int digit1 = hours / 10;
            int digit2 = hours % 10;
            int minutes = (sec % 3600) / 60;
            int digit3 = minutes / 10;
            int digit4 = minutes % 10;
            setDisplay(digit1, digit2, digit3, digit4);
            _delay_ms(1000);
        }
    }

    return 0;
}