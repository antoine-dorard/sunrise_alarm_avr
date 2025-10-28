#include "display.h"
#include "clock.h"

#define SRCLK _BV(PC2) 
#define SER _BV(PC3) 
#define RCLK _BV(PC1) 

#define DATA_HIGH()  (PORTC |=  SER)
#define DATA_LOW()   (PORTC &= ~SER)

#define CLK_PULSE()  do { PORTC |=  SRCLK; PORTC &= ~SRCLK; } while(0)
#define LATCH_PULSE() do { PORTC |=  RCLK; PORTC &= ~RCLK; } while(0)

#define DIGITS 4

#define DIGIT0 0b00111111
#define DIGIT1 0b00000110
#define DIGIT2 0b01011011
#define DIGIT3 0b01001111
#define DIGIT4 0b01100110
#define DIGIT5 0b01101101
#define DIGIT6 0b01111101
#define DIGIT7 0b00000111
#define DIGIT8 0b01111111
#define DIGIT9 0b01101111

int isHourBlinking = 0;
int isMinuteBlinking = 0;


void reset();

uint8_t getDigitMapping(int digit) {
    switch (digit) { //  HGFEDCBA
        case 0: return DIGIT0; // ABCDEF
        case 1: return DIGIT1; // BC - 
        case 2: return DIGIT2; // ABGED
        case 3: return DIGIT3; // ABGCD
        case 4: return DIGIT4; // FGBC
        case 5: return DIGIT5; // AFGCD
        case 6: return DIGIT6; // All but B
        case 7: return DIGIT7; // ABC
        case 8: return DIGIT8; // All
        case 9: return DIGIT9; // All but E
        case 10: return 0b00000000;
    }
    return 0;
}

void reset() {
    DATA_HIGH();
    for (int i = 0; i < DIGITS * 8; i++) {
        CLK_PULSE();
    }
    DATA_LOW();

    LATCH_PULSE();
}

void initDisplay() {
    DDRC |= SRCLK; // clock pin
    DDRC |= SER; // serial data pin
    DDRC |= RCLK; // latch pin

    reset();
}

void shiftByte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        if (data & (1 << (7 - i))) {
            DATA_HIGH();
        }
        else {
            DATA_LOW();
        }
        CLK_PULSE();
    }
}

void writeToSR(uint32_t data) {

    shiftByte((data >> 24) & 0xFF);
    shiftByte((data >> 16) & 0xFF);
    shiftByte((data >> 8) & 0xFF);
    shiftByte(data & 0xFF);

    LATCH_PULSE();

}


void setDisplay(int digit1, int digit2, int digit3, int digit4){

    if(isHourBlinking && getHalfSecond()){
        digit1 = 10;
        digit2 = 10;
    } else if (isMinuteBlinking && getHalfSecond()) {
        digit3 = 10;
        digit4 = 10;
    }

    uint32_t data = getDigitMapping(digit4);
    data <<= 8;
    data |= getDigitMapping(digit3);
    data <<= 8;
    data |= getDigitMapping(digit2);
    data <<= 8;
    data |= getDigitMapping(digit1);
    
    data = ~data; // The 7-segments is ON on low (Common Anode mode), so invert the bits

    writeToSR(data);
}

void setDisplayBlink(int digitGroup){
    switch (digitGroup) {
        case NO_BLINK: isHourBlinking = 0; isMinuteBlinking = 0; break;
        case BLINK_HOURS: isHourBlinking = 1; isMinuteBlinking = 0; break;
        case BLINK_MINUTES: isHourBlinking = 0; isMinuteBlinking = 1; break;
    }
}