#include <sys/types.h>
#define F_CPU 2000000UL

#include <util/delay.h>
#include <avr/interrupt.h>

#include "display.h"
#include "clock.h"
#include "utils.h"
#include "alarm.h"

#define UP 1
#define DOWN 2


#define UP_BUTTON  _BV(PB2)
#define DOWN_BUTTON _BV(PB1)
#define SET_BUTTON  _BV(PD7)

#define ALARM_LED _BV(PD5)

int up_debounce = 0;
int down_debounce = 0;
int set_debounce = 0;

int menu_mode = 0; // 0: normal, 1: set hours, 2: set minutes, 3: alarm 1 on/off, 4: alarm 1 hours, 5: alarm 1 minutes

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
            dismissAlarm();
            break;
        case 1:
            cycleDigits(direction, digit1, digit2, HOURS);
            break;
        case 2:
            cycleDigits(direction, digit3, digit4, MINUTES);
            break;
        case 3: 
            toggleAlarm();

            if(getIsAlarmON()) {
                *digit1 = D_O;
                *digit2 = D_N;
            }
            else {
                *digit1 = D_DASH;
                *digit2 = D_DASH;
            }
            break;
        case 4:
            cycleDigits(direction, digit1, digit2, HOURS);
            break;
        case 5:
            cycleDigits(direction, digit3, digit4, MINUTES);
            break;
    }
}

void cycleMenuMode(int* digit1, int* digit2, int* digit3, int* digit4) {
    // SET
    switch (menu_mode) {
        case 0: 
            setDisplayBlink(HOURS, 1);
            break;
        case 1:
            setDisplayBlink(HOURS, 0);
            setTime(concatenateDigits(*digit1, *digit2), concatenateDigits(*digit3, *digit4));
            setDisplayBlink(MINUTES, 1);
            break;
        case 2: 
            setDisplayBlink(MINUTES, 0);
            setTime(concatenateDigits(*digit1, *digit2), concatenateDigits(*digit3, *digit4));
            
            *digit3 = D_A;
            *digit4 = 1;

            if(getIsAlarmON()) {
                *digit1 = D_O;
                *digit2 = D_N;
            }
            else {
                *digit1 = D_DASH;
                *digit2 = D_DASH;
            }
            break;
        case 3:
            convertSecondsToDigits(getAlarmSeconds(), digit1, digit2, digit3, digit4);
            setDisplayBlink(HOURS, 1);
            break;
        case 4:
            setDisplayBlink(HOURS, 0);
            setAlarm(concatenateDigits(*digit1, *digit2), concatenateDigits(*digit3, *digit4));
            setDisplayBlink(MINUTES, 1);
            break;
        case 5:
            setDisplayBlink(MINUTES, 0);
            setAlarm(concatenateDigits(*digit1, *digit2), concatenateDigits(*digit3, *digit4));
            break;
    }

    // NEXT MENU
    menu_mode += 1;
    if (menu_mode == 6) menu_mode = 0;
}

void initButtons(){
    DDRB |= UP_BUTTON;
    DDRB |= DOWN_BUTTON;
    DDRD |= SET_BUTTON;

    DDRD |= ALARM_LED; // TODO use alarm.c to manage alarm LED
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
            digit1 = getClockHours() / 10;
            digit2 = getClockHours() % 10;
            digit3 = getClockMinutes() / 10;
            digit4 = getClockMinutes() % 10;
        }

        isAlarmRinging(getSeconds()) && isHalfSecond() ? (PORTD |= ALARM_LED) : (PORTD &= ~ALARM_LED);
        
        setDisplay(digit1, digit2, digit3, digit4);
    }

    return 0;
}