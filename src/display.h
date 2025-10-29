#include <avr/io.h>

#define NO_BLINK 0
#define HOURS 1
#define MINUTES 2


void initDisplay();

void setDisplay(int digit1, int digit2, int digit3, int digit4); // Maximum abstraction for main.c

void setDisplayBlink(int digitGroup);
