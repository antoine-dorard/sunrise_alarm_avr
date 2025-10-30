#ifndef __ASSEMBLER__
/* These only work in C programs.  */
#include <inttypes.h>
#endif

unsigned concatenateDigits(unsigned x, unsigned y);

void convertSecondsToDigits(uint32_t seconds, int* digit1, int* digit2, int* digit3, int* digit4);

uint32_t computeElapsedTime(uint32_t initialSeconds, uint32_t currentSeconds);
