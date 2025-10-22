#define F_CPU 1000000UL

#include "display.h"

#include <util/delay.h>

int main() {

    initDisplay();

    uint8_t data = getDigitMapping(1);

    while (1) {
        writeToSR(data);
    }

    return 0;
}