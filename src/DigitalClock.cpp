#include<Arduino.h>
#include "avr8-stub.h"
#include "app_api.h"
#include<LiquidCrystal.h>

typedef char* strin;

LiquidCrystal lcd(2,3,4,5,6,7);
int seconds = 0;
int minutes = 0; 
int hour = 0;
int day = 25;
int month = 1;
int year = 2000;


byte Bell[] = {
    B00000,
    B00100,
    B01010,
    B01010,
    B01010,
    B11111,
    B00100,
    B00000
};


void digitalclock();
bool IsLeapYear(int year);
void blinkincolon();

void setup()
{
    debug_init();
    //Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.clear();
    lcd.setCursor(7, 0);
    lcd.print("0:00");
    lcd.setCursor(3, 1);
    lcd.print("25.02.2000");

    lcd.createChar(0, Bell);
    //lcd.print("test");

}

void loop()
{
    digitalclock();
    delay(10);
}

void digitalclock()
{
    char sseconds[3] = {};
    char sminutes[3] = {};
    char smonth[3] = {};

    if(seconds > 59)
    {
        seconds = 0;
        minutes++;

        if(minutes > 59)
        {
            minutes = 0;
            hour++;

            if(hour > 23)
            {
                hour = 0;
                day++;

                if(day > 31 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12))
                {
                    day = 1;
                    month++;

                    if(month > 12)
                    {
                        month = 1;
                        year++;
                    }
                }
                else if(day > 30 && (month == 4 || month == 6 || month == 9 || month == 11))
                {
                    day = 1;
                    month++;
                }                  
                else if(day > 28 && month == 2 && !IsLeapYear(year))
                {
                    day = 1;
                    month++;
                }
                else if(day > 29 && month == 2)
                {
                    day = 1;
                    month++;
                }
            }
        }
    }

    lcd.setCursor(6, 0);
    if(hour < 10)
    {
        lcd.print(" ");
    }
    lcd.print(hour);
    lcd.print(":");

    snprintf(sminutes, 3, "%02d", minutes);
    lcd.setCursor(9, 0);
    lcd.print(sminutes);
    // lcd.print(":");

    // snprintf(sseconds, 3, "%02d", seconds);
    // lcd.setCursor(10, 0);
    // lcd.print(sseconds);

    lcd.setCursor(3, 1);
    if(day < 10)
    {
        lcd.print(" ");
    }
    lcd.print(day);
    lcd.print(".");

    snprintf(smonth, 3, "%02d", month);
    lcd.print(smonth);
    lcd.print(".");

    lcd.print(year);

    //Serial.print(sseconds);

    //lcd.setCursor(15, 1);
    //lcd.write(byte(0));

    
    seconds++;
    //day++;
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

void blinkincolon()
{
    lcd.setCursor(8, 0);
    lcd.print(" ");
    delay(500);
    lcd.setCursor(8, 0);
    lcd.print(":");
    delay(500);
}
