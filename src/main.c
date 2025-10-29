#include <sys/types.h>
#define F_CPU 2000000UL

#include <util/delay.h>
#include <avr/interrupt.h>

#include "display.h"
#include "clock.h"
#include "utils.h"

#define UP 1
#define DOWN 2


#define UP_BUTTON  _BV(PB2)
#define DOWN_BUTTON _BV(PB1)
#define SET_BUTTON  _BV(PD7)

int up_debounce = 0;
int down_debounce = 0;
int set_debounce = 0;

int menu_mode = 0; // 0: normal, 1: set hours, 2: set minutes

/*
Digit: A B : A B 
Time:  1 1 : 2 3 

*/
void cycleDigits(int direction, int* digitA, int* digitB, int digitGroup){
    if (direction & 1) {
          if (digitGroup & HOURS && *digitA == 2  && *digitB == 3) {
            *digitA = 0;
            *digitB = 0;
        } else if (digitGroup & MINUTES && *digitA == 5  && *digitB == 9) {
            *digitA = 0;
            *digitB = 0;
        } else {
            *digitA += (*digitB + 1) / 10;
            *digitB = (*digitB + 1) % 10;
        }

    } else if (direction & 2) {
        if (*digitA == 0  && *digitB == 0) {
            if (digitGroup & HOURS) {
                *digitA = 2;
                *digitB = 3;
            } else if (digitGroup & MINUTES) {
                *digitA = 5;
                *digitB = 9;
            }
        }
        else {
            *digitA = *digitB == 0 ? *digitA - 1 : *digitA;
            *digitB = *digitB == 0 ? 9 : (*digitB - 1) % 10;
        }
    }
}

void handleMenuMode(int direction, int* digit1, int* digit2, int* digit3, int* digit4) {
    switch (menu_mode) {
        case 0:
            // Nothing happens when up or down presses in normal/clock mode
            break;
        case 1:
            cycleDigits(direction, digit1, digit2, HOURS);
            break;
        case 2:
            cycleDigits(direction, digit3, digit4, MINUTES);
            break;
    }
}

void cycleMenuMode(int* digit1, int* digit2, int* digit3, int* digit4) {
    // SET
    switch (menu_mode) {
        case 0:
            // Do nothing, nothing to set when in normal/clock mode
            break;
        case 1:
            // Set hours
            setTime(concatenate(*digit1, *digit2), concatenate(*digit3, *digit4));
            break;
        case 2:
            // Set minutes
            setTime(concatenate(*digit1, *digit2), concatenate(*digit3, *digit4));
            break;
    }

    // NEXT MENU
    menu_mode += 1;
    if (menu_mode == 3) menu_mode = 0;

    setDisplayBlink(menu_mode); // TODO odd abstraction, the display should not have to deal with the menu modes directly. Instead pass in BLINK/NO_BLINK) depending on the menu mode checked here
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

    int digit1 = 0;
    int digit2 = 0;
    int digit3 = 0;
    int digit4 = 0;

    while (1) {
        
        if (PINB & UP_BUTTON) {
            if (up_debounce == 0) {
                handleMenuMode(UP, &digit1, &digit2, &digit3, &digit4);
                up_debounce = 1;
            }
        } else {
            up_debounce = 0;
        }
        
        if (PINB & DOWN_BUTTON) {
            if (down_debounce == 0) {
                handleMenuMode(DOWN, &digit1, &digit2, &digit3, &digit4);
                down_debounce = 1;
            }
        } else {
            down_debounce = 0;
        }
        
        if (PIND & SET_BUTTON) {
            if (set_debounce == 0) {
                cycleMenuMode(&digit1, &digit2, &digit3, &digit4);
                set_debounce = 1;
            }
        } else {
            set_debounce = 0;
        }

        if (menu_mode == 0) {
            digit1 = getHours() / 10;
            digit2 = getHours() % 10;
            digit3 = getMinutes() / 10;
            digit4 = getMinutes() % 10;
        }
        
        setDisplay(digit1, digit2, digit3, digit4);
    }

    return 0;
}