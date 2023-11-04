#include<Arduino.h>
#include "avr8-stub.h"
#include "app_api.h"
#include<LiquidCrystal.h>

const int pos_offset = 5;
const int alarmpin = 8;

LiquidCrystal lcd(2,3,4,5,6,7);
int seconds = 0;
int minutes = 0; 
int hour = 0;
int day = 4;
int month = 6;
int year = 2023;
String sdayofweek_eng[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String sdayofweek_pl[7] = {" Nd", " Pn", " Wt", " Śr", "Czw", " Pt", " So"};
bool dst = true;
int adddst;

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
int dayofweek(int y, int m, int d);

void setup()
{
    debug_init();
    //Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.clear();
    // lcd.setCursor(pos_offset, 0);
    // lcd.print("0:00");
    // lcd.setCursor(pos_offset - 3, 1);
    // lcd.print("25.02.2000");

    pinMode(alarmpin, OUTPUT);
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

    if(dst)
    {
        adddst = 1;
    }
    else
    {
        adddst = 0;
    }

    lcd.setCursor(pos_offset, 0);
    if(hour + adddst < 10)
    {
        lcd.print(" ");
    }
    lcd.print(hour + adddst);
    lcd.print(":");

    snprintf(sminutes, 3, "%02d", minutes);
    lcd.setCursor(pos_offset + 3, 0);
    lcd.print(sminutes);
    // lcd.print(":");

    // snprintf(sseconds, 3, "%02d", seconds);
    // lcd.setCursor(10, 0);
    // lcd.print(sseconds);
    
    lcd.setCursor(13, 0);
    lcd.print(sdayofweek_pl[dayofweek(year, month, day)]);


    lcd.setCursor(pos_offset - 3, 1);
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

int dayofweek(int y, int m, int d)
{
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    if(m < 3)
    {
        y -= 1;
    }
    int dow = (y + y/4 - y/100 + y/400 + t[m-1] + d)%7; 

    return dow;
}

void alarm()
{
    digitalWrite(alarmpin, HIGH);
    delay(500);
    digitalWrite(alarmpin, LOW);
    delay(500);
}