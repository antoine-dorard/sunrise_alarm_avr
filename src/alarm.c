#include "alarm.h"
#include "utils.h"

#include <util/atomic.h>

uint32_t alarmSeconds = 0;

int isAlarmON = 0;
int isAlarmDismissed = 0;

void toggleAlarm(){
    isAlarmON ^= 1;
}

void setAlarm(int hours, int minutes){
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
        alarmSeconds = (uint32_t)hours * 3600 + (uint32_t)minutes * 60;
    }
}

int getIsAlarmON(){
    return isAlarmON;
}

void dismissAlarm(){
    isAlarmDismissed = 1;
}

uint32_t getAlarmSeconds() {
    return alarmSeconds;
}

int isAlarmRinging(uint32_t currentSeconds){
    if (!isAlarmON) return 0; // Never ringing if off

    if (computeElapsedTime(alarmSeconds, currentSeconds) > 30) {
        isAlarmDismissed = 0;
        return 0;
    } else {
        if(isAlarmDismissed) {
            return 0;
        }
        else {
            return 1;
        }
    }

    return -1;
}