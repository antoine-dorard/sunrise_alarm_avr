#include <avr/io.h>

void toggleAlarm();
void setAlarm(int hours, int minutes);
uint32_t getAlarmSeconds();
int getIsAlarmON();
void dismissAlarm();

int isAlarmRinging(uint32_t currentSeconds);


