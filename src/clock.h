#include <avr/io.h>

void initClock();

uint32_t getSeconds();
uint32_t getMinutes();
uint32_t getHours();

uint32_t isHalfSecond();

void setTime(int hour, int min);
