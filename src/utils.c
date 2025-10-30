#include "utils.h"


unsigned concatenateDigits(unsigned x, unsigned y) {
    unsigned pow = 10;
    while(y >= pow)
        pow *= 10;
    return x * pow + y;        
}

/*
Param: seconds in a day
Return: time in digits 
*/
void convertSecondsToDigits(uint32_t seconds, int* digit1, int* digit2, int* digit3, int* digit4) {
    uint8_t hours = seconds / 3600;
    uint16_t minutes = (seconds % 3600) / 60;

    *digit1 = hours / 10;
    *digit2 = hours % 10;
    *digit3 = minutes / 10;
    *digit4 = minutes % 10;
}

uint32_t computeElapsedTime(uint32_t initialSeconds, uint32_t currentSeconds) {
    if (currentSeconds >= initialSeconds) return currentSeconds - initialSeconds; // Same day
    return (86400 - initialSeconds) + currentSeconds;
}