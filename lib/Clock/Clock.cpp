#include "Clock.h"

unsigned long temptime = 0;
unsigned long oneMinute = 0;

_SetClock SetClock;

void DigitalClock()
{
    oneMinute = currenttime - temptime;

    if(Clock.digitalClock_flag && oneMinute >= 60000)
    {
        (*Clock.minutes)++;

        if(*Clock.minutes > 59)
        {
            *Clock.minutes = 0;
            (*Clock.hour)++;

            if(*Clock.hour > 23)
            {
                *Clock.hour = 0;
                (*Clock.day)++;

                if(*Clock.day > 30 && (*Clock.month == 0 || *Clock.month == 2 || *Clock.month == 4 || *Clock.month == 6 || *Clock.month == 7 || *Clock.month == 9 || *Clock.month == 11))
                {
                    *Clock.day = 0;
                    (*Clock.month)++;

                    if(*Clock.month > 11)
                    {
                        *Clock.month = 0;
                        (*Clock.year)++;
                    }
                }
                else if(*Clock.day > 29 && (*Clock.month == 3 || *Clock.month == 5 || *Clock.month == 8 || *Clock.month == 10))
                {
                    *Clock.day = 0;
                    (*Clock.month)++;
                }                  
                else if(*Clock.day > 27 && *Clock.month == 1 && !IsLeapYear(*Clock.year))
                {
                    *Clock.day = 0;
                    (*Clock.month)++;
                }
                else if(*Clock.day > 28 && *Clock.month == 1)
                {
                    *Clock.day = 0;
                    (*Clock.month)++;
                }
            }
        }
        temptime = currenttime;
    }
}

bool IsLeapYear(int year)
{
    bool leapyear = false;
    if(year%4 == 0)
    {
        if(year%100)
        {
            leapyear = true;
        }
        else if(year%400 == 0)
        {
            leapyear = true;
        }
    }

    return leapyear;
}

int DayOfWeek(int y, int m, int d)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    if(m + 1 < 3)
    {
        y -= 1;
    }
    int dow = (y + y/4 - y/100 + y/400 + t[m] + d + 1)%7; 

    return dow;
}

void EnterClockSetting()
{

    if(HoldLRButtons() && !SetClock.entersetclock_flag)
    {
        SetClock.entersetclock_flag = true;
        SetClock.setclock_flag = true;
        Alarm.turnOn_flag = false;
        Alarm.mute_flag = true;
        
    }
    else if(SetClock.entersetclock_flag)
    {
        SetClock.setclock_timer = 0;
        SetClock.entersetclock_flag = false;  
    }
}

void SettingClock()
{
    if(SetClock.setclock_flag)
    {
        Clock.digitalClock_flag = false;
        if(SetClock.setclock_countposition == 0)
        {
            Blink(13, 0, DispClock.sDST[2]);
            if(Clock.dst == 1)
            {
                Blink(13, 1, DispClock.sDST[1]);
            }
            else
            {
                Blink(13, 1, DispClock.sDST[0]);
            }
            if(PushLRButtons(&LButton, LEFT_BUTTON_PIN) || PushLRButtons(&RButton, RIGHT_BUTTON_PIN))
            {
                Clock.dst = !Clock.dst;
                ClockToString(Clock.elementsValue[2], 2, Clock.sClockElements[2]);
                DisplayValue(Clock.elementsPositions[2], 0, Clock.sClockElements[2]);
            }
            if(PushSetButton())
            {
                SetClock.setclock_countposition++;
                DisplayValue(13, 0, DispClock.sDST[2]);
                if(Clock.dst == 1)
                {
                    DisplayValue(13, 1, DispClock.sDST[1]);
                }
                else
                {
                    DisplayValue(13, 1, DispClock.sDST[0]);
                }
            }

        }
        else if(SetClock.setclock_countposition < Clock.clockElementsAmount)
        {

            Blink(Clock.elementsPositions[SetClock.setclock_countposition], (int)SetClock.setclock_countposition/3.0, Clock.sClockElements[SetClock.setclock_countposition]);
            
            if(PushLRButtons(&LButton, LEFT_BUTTON_PIN))
            {
                switch (SetClock.setclock_countposition)
                {
                case 0:
                case 1:
                    Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] - 1, 60);
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;
                case 2:
                    Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] - 1, 24);
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;
                case 3:
                    Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] - 1, 9999);
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;   
                case 4:
                    Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] - 1, 12);
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;                                      
                case 5:
                    if(*Clock.month == 0 || *Clock.month == 2 || *Clock.month == 4 || *Clock.month == 6 || *Clock.month == 7 || *Clock.month == 9 || *Clock.month == 11)
                    {
                        Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] - 1, 31);
                    }
                    else if(*Clock.month == 3 || *Clock.month == 5 || *Clock.month == 8 || *Clock.month == 10)
                    {
                        Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] - 1, 30);
                    }
                    else if(*Clock.month == 1 && !IsLeapYear(*Clock.year))
                    {
                        Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] - 1, 28);
                    }
                    else if(*Clock.month == 1)
                    {
                        Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] - 1, 29);
                    }
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;                               
                default:
                    break;
                }
            }

            if(PushLRButtons(&RButton, RIGHT_BUTTON_PIN))
            {
                switch (SetClock.setclock_countposition)
                {
                case 0:
                case 1:
                    Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] + 1, 60);
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;
                case 2:
                    Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] + 1, 24);
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;
                case 3:
                    Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] + 1, 9999);
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;
                case 4:
                    Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] + 1, 12);
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;
                case 5:
                    if(*Clock.month == 0 || *Clock.month == 2 || *Clock.month == 4 || *Clock.month == 6 || *Clock.month == 7 || *Clock.month == 9 || *Clock.month == 11)
                    {
                        Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] + 1, 31);
                    }
                    else if(*Clock.month == 3 || *Clock.month == 5 || *Clock.month == 8 || *Clock.month == 10)
                    {
                        Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] + 1, 30);
                    }
                    else if(*Clock.month == 1 && !IsLeapYear(*Clock.year))
                    {
                        Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] + 1, 28);
                    }
                    else if(*Clock.month == 1)
                    {
                        Clock.elementsValue[SetClock.setclock_countposition] = Mod(Clock.elementsValue[SetClock.setclock_countposition] + 1, 29);
                    }
                    ClockToString(Clock.elementsValue[SetClock.setclock_countposition], SetClock.setclock_countposition, Clock.sClockElements[SetClock.setclock_countposition]);
                    break;                        
                default:
                    break;
                }
            }

            if(PushSetButton())
            {
                DisplayValue(Clock.elementsPositions[SetClock.setclock_countposition], (int)SetClock.setclock_countposition/3.0, Clock.sClockElements[SetClock.setclock_countposition]);
                SetClock.setclock_countposition++;
            }
        }
        else
        {
            SetClock.setclock_flag = false;
            SetClock.setclock_countposition = 0;
            Clock.digitalClock_flag = true;
            lcd.clear();
            temptime = currenttime;
        }
    }
}