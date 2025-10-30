#include <avr/io.h>

#define NO_BLINK 0
#define HOURS 1
#define MINUTES 2

#define D_DASH 11
#define D_O 12
#define D_N 13
#define D_A 14

void initDisplay();

void setDisplay(int digit1, int digit2, int digit3, int digit4); // Maximum abstraction for main.c

void setDisplayBlink(int digitGroup, int blink); // digitGroup: HOURS, MINUTES; blink: 0 or 1
