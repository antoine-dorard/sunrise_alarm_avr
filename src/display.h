#include <avr/io.h>

#define NO_BLINK 0
#define BLINK_HOURS 1
#define BLINK_MINUTES 2


void initDisplay();

void setDisplay(int digit1, int digit2, int digit3, int digit4); // Maximum abstraction for main.c

void setDisplayBlink(int digitGroup);
