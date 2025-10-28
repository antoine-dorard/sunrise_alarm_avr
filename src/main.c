#include <sys/types.h>
#define F_CPU 2000000UL

#include <util/delay.h>
#include <avr/interrupt.h>

#include "display.h"
#include "clock.h"

#define UP 1
#define DOWN 2


#define UP_BUTTON  _BV(PB2)
#define DOWN_BUTTON _BV(PB1)
#define SET_BUTTON  _BV(PD7)

int up_debounce = 0;
int down_debounce = 0;
int set_debounce = 0;

int menu_mode = 0; // 0: normal, 1: set hours, 2: set minutes


void handleMenuMode(int direction) {
    switch (menu_mode) {
        case 0:
            // Normal mode
            break;
        case 1:
            // Set hours mode
            if(direction & 1) setTime(getHours() + 1, getMinutes());
            if(direction & 2) setTime(getHours() - 1, getMinutes());
            break;
        case 2:
            // Set minutes mode
            if(direction & 1) setTime(getHours(), getMinutes() + 1);
            if(direction & 2) setTime(getHours(), getMinutes() - 1);
            break;
    }
}

void cycleMenuMode() {
    menu_mode += 1;
    if (menu_mode == 3) menu_mode = 0;
    setDisplayBlink(menu_mode);
}

void initButtons(){
    DDRB |= UP_BUTTON;
    DDRB |= DOWN_BUTTON;
    DDRD |= SET_BUTTON;
}

int main() {
    initDisplay();
    initClock();
    initButtons();

    setTime(21, 12); // Set initial time to 21:12

    sei();

    while (1) {
        int digit1 = getHours() / 10;
        int digit2 = getHours() % 10;
        int digit3 = getMinutes() / 10;
        int digit4 = getMinutes() % 10;
        setDisplay(digit1, digit2, digit3, digit4);

        if (PINB & UP_BUTTON) {
            if (up_debounce == 0) {
                handleMenuMode(UP);
                up_debounce = 1;
            }
        } else {
            up_debounce = 0;
        }

        if (PINB & DOWN_BUTTON) {
            if (down_debounce == 0) {
                handleMenuMode(DOWN);
                down_debounce = 1;
            }
        } else {
            down_debounce = 0;
        }

        if (PIND & SET_BUTTON) {
            if (set_debounce == 0) {
                cycleMenuMode();
                set_debounce = 1;
            }
        } else {
            set_debounce = 0;
        }

        

    }

    return 0;
}