#include "DisplayClock.h"

LiquidCrystal lcd(2,3,4,5,6,7);

void DisplayValue(int position, int row, char* value)
{
    lcd.setCursor(position, row);
    lcd.print(value);
}

 void DisplaySigns()
{
    lcd.setCursor(7, 0);
    lcd.print(":");
    lcd.setCursor(4, 1);
    lcd.print(".");
    lcd.setCursor(7, 1);
    lcd.print(".");
    if(Alarm.armed_flag)
    {
        lcd.setCursor(15, 1);
        lcd.write(byte(0));
    }
    else
    {
        lcd.setCursor(15, 1);
        lcd.print(" ");
    }
}

void Blink(int position, int row, char* value)
{
    if(Timer(500, &DispClock.show_timer))
    {
        DispClock.show = !DispClock.show;
    }

    int len = strlen(value);

    if(DispClock.show)
    {
        DisplayValue(position, row, value);
    }
    else
    {
        lcd.setCursor(position, row);
        for(int i = 0; i < len; i++)
        {
            lcd.print(" ");
        }
    }
}

void ClockToString(int value, int i, char* sValue)
{
    switch(i)
    {
        case 0:
        case 1:
            snprintf(sValue, 3, "%02d", value);
            break;        
        case 2:
            snprintf(sValue, 3, "%2d", Mod(value + Clock.dst, 24));
            break;
        case 3:
            snprintf(sValue, 5, "%4d", value);
            break;
        case 4:
            snprintf(sValue, 3, "%02d", value + 1);
            break;            
        case 5:
            snprintf(sValue, 3, "%2d", value + 1);
            break;
        default:
            break;   
    }
}

void DisplayClock()
{
    if(DispClock.displayClock_flag && Clock.digitalClock_flag)
    {
        for(int i = 1; i < Clock.clockElementsAmount; i++)
        {
            ClockToString(Clock.elementsValue[i], i, Clock.sClockElements[i]);
            DisplayValue(Clock.elementsPositions[i], (int)i/3.0, Clock.sClockElements[i]);
        }

        lcd.setCursor(13, 0);
        lcd.print(DispClock.sDayOfWeek_pl[DayOfWeek(*Clock.year, *Clock.month, *Clock.day)]);
        DisplaySigns();
    }
}