#include "display.h"

#define SRCLK _BV(PB0) 
#define SER _BV(PB2) 
#define RCLK _BV(PB1) 

#define F_CPU 1000000UL
#include <util/delay.h>

void reset();

/*
Entirely depends on the pins connections between the shift register and the 7-segment display
*/
uint8_t getDigitMapping(int digit) {
    switch (digit) { //  HGFEDCBA
        case 0: return 0b00111111; // ABCDEF
        case 1: return 0b00000110; // BC - 
        case 2: return 0b01011011; // ABGED
        case 3: return 0b01001111; // ABGCD
        case 4: return 0b01100110; // FGBC
        case 5: return 0b01101101; // AFGCD
        case 6: return 0b01111101; // All but B
        case 7: return 0b00000111; // ABC
        case 8: return 0b01111111; // All
        case 9: return 0b01101111; // All but E
    }
    return 0;
}


void initDisplay() {
    DDRB |= SRCLK; // clock pin
    DDRB |= SER; // serial data pin
    DDRB |= RCLK; // latch pin

    reset();
}

void latch() {
    PORTB |= RCLK;
    PORTB &= ~RCLK;
}

void reset() {
    PORTB |= SER; // set it to high
    for (int i = 0; i < 8; i++) {
        PORTB &= ~SRCLK; // clock low
        PORTB |= SRCLK; // clock high
    }
    PORTB &= ~SER; // set it to low
    latch();
}

void tick() {
    PORTB |= SRCLK; // clock high
    PORTB &= ~SRCLK; // clock low
    _delay_ms(500);
}

void tickDebug() {
    tick();
}

void writeToSR(uint8_t data) {
    data = ~data; // MOVE OUT: The 7-segments is ON on low (Common Anode mode), so invert the bits
    
    for (int i = 0; i < 8; i++) {
        PORTB &= ~SRCLK; // clock low
        if (data & (1 << (7 - i))) {
            PORTB |= SER; // set it to high 
        }
        else {
            PORTB &= ~SER; // set it to low
        }
        PORTB |= SRCLK; // clock high
    }

    latch();

}
