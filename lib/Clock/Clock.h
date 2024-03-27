#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "Timer_Beep_Modulo.h"
#include "Buttons.h"
#include "Alarm.h"
#include "DisplayClock.h"
#include <Arduino.h>
#include<LiquidCrystal.h>

const int POS_OFFSET = 5;
extern LiquidCrystal lcd;

struct _Clock
{
    const int clockElementsAmount = 6;
    int elementsValue[6] = {0, 0, 0, 2023, 0, 0};
    int* seconds = &elementsValue[0];
    int* minutes = &elementsValue[1]; 
    int* hour = &elementsValue[2];
    int* day = &elementsValue[5];
    int* month = &elementsValue[4];
    int* year = &elementsValue[3];
    int elementsPositions[6] = {POS_OFFSET + 6, POS_OFFSET + 3,  POS_OFFSET, POS_OFFSET + 3, POS_OFFSET, POS_OFFSET - 3};
    int dst = 0;
    char sClockElements[6][5] = {};
    bool digitalClock_flag = true;
    bool dstAdded = false;
};

struct _SetClock
{
    bool setclock_flag = false;
    bool entersetclock_flag = false;
    int setclock_countposition = 0;
    unsigned long setclock_timer = 0;
};

extern _Clock Clock;            //used in Alarm.cpp and DisplayClock.cpp

void DigitalClock();
bool IsLeapYear(int year);
int DayOfWeek(int y, int m, int d);
void EnterClockSetting();
void SettingClock();

#endif