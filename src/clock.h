#include <avr/io.h>

void initClock();

uint32_t getSeconds();
//uint16_t getMinutes();

uint8_t getClockMinutes();
uint8_t getClockHours();

unsigned isHalfSecond();

void setTime(int hour, int min);
