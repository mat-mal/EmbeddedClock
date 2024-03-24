#ifndef _DISPLAY_CLOCK_H_
#define _DISPLAY_CLOCK_H_
#include "Timer_Beep_Modulo.h"
#include "Clock.h"
#include "Alarm.h"
#include "Buttons.h"
#include <LiquidCrystal.h>

struct _DispClock
{
    bool show = false;
    unsigned long show_timer = 0;
    bool displayClock_flag = true;
    String sDayOfWeek_eng[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    String sDayOfWeek_pl[7] = {" Nd", " Pn", " Wt", " Sr", "Czw", " Pt", " So"};
    char sDST[3][4] = {"OFF", " ON", "DST"};
};

extern _DispClock DispClock;        //used in Clock.cpp and Alarm.cpp

void Blink(int position, int row, char*);
void DisplayValue(int position, int row, char* value);
void DisplaySigns(); 
void ClockToString(int value, int i, char* sValue);
void DisplayClock();

#endif