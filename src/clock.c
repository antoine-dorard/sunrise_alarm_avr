
#include "clock.h"
#include <avr/interrupt.h>

volatile uint32_t seconds = 0;
volatile uint32_t minutes = 0;
volatile uint32_t hours = 0;

volatile uint32_t half_seconds = 0;

void initClock(){
    // Initialize the clock here
    // Configure timer 1 for CTC mode (WGM12 is in TCCR1B)
    TCCR1B |= (1 << WGM12);
    // Set prescaler to 64 (CS11 and CS10)
    TCCR1B |= (1 << CS11) | (1 << CS10);
    TIMSK1 |= (1 << OCIE1A); // Enable CTC interrupt

    // For F_CPU = 2MHz and prescaler = 64:
    // ticks per second = 2_000_000 / 64 = 31250
    // OCR1A should be (ticks_per_second - 1) for CTC -> 31249
    OCR1A = 15624; // 2 Hz

    seconds = 0;
    minutes = 0;
    hours = 0;
}

void setTime(int hour, int min){
    // Perform arithmetic in 32-bit to avoid 16-bit int overflow on AVR (int is 16-bit)
    seconds = (uint32_t)hour * 3600U + (uint32_t)min * 60U;
    minutes = (uint32_t)min;
    hours = (uint32_t)hour;
}

uint32_t getSeconds(){
    return seconds;
}

uint32_t getMinutes(){
    return minutes;
}

uint32_t getHours(){
    return hours;
}

uint32_t isHalfSecond(){
    return half_seconds;
}

ISR(TIMER1_COMPA_vect){
        half_seconds ^= 1U;
        if (half_seconds == 0) {
            seconds++;
        }

    if (seconds >= 86400){
        seconds = 0;
    }

    minutes = (seconds % 3600) / 60;
    hours = seconds / 3600;
}