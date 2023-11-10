#include<Arduino.h>
#include "avr8-stub.h"
#include "app_api.h"
#include<LiquidCrystal.h>

const int POS_OFFSET = 5;
const int BUZZER_PIN = 8;
const int LEFT_BUTTON_PIN = 9;
const int SET_BUTTON_PIN = 10;
const int RIGHT_BUTTON_PIN = 12;
const int BRIGHTNESS_PIN = 11;
const int FOTO_PIN = A0;
const int clockElementsAmount = 6;

LiquidCrystal lcd(2,3,4,5,6,7);
long int currenttime = 0, clock_temp = 0, beep_temp = 0, alarm_temp = 0, setclock_temp = 0, setbutton_temp = 0, show_temp = 0;
int seconds = 0;
int minutes = 0; 
int hour = 0;
int day = 4;
int month = 6;
int year = 2023;
int elementsPositions[6] = {POS_OFFSET + 6, POS_OFFSET + 3,  POS_OFFSET, POS_OFFSET - 3, POS_OFFSET, POS_OFFSET + 3};
String sdayofweek_eng[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
String sdayofweek_pl[7] = {" Nd", " Pn", " Wt", " Śr", "Czw", " Pt", " So"};
bool dst = true;
int adddst;
bool alarmseton = true;
bool turnonalarm = false;
int alarmstate = LOW;
int beepcounter = 0;
bool beepon = false;
int lastSetButtonState = HIGH;
int lastRightButtonState = HIGH;
int lastLeftButtonState = HIGH;
int countmiliseconds = 0;
bool stoper_flag = false;
bool setclock_flag = false;
bool show = true;


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
void alarm();
void setalarm();
void brightnessControl();
void setclock();
void beep();
int stoper(int tocountdown, long int *temp);
void blink(int element, int value);
void displayValue(int position, int row, char* value);
void displaySigns();

void setup()
{
    debug_init();
    // Serial.begin(9600);
    lcd.begin(16, 2);
    lcd.clear();
    // lcd.setCursor(POS_OFFSET, 0);
    // lcd.print("0:00");
    // lcd.setCursor(POS_OFFSET - 3, 1);
    // lcd.print("25.02.2000");

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(SET_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
    lcd.createChar(0, Bell);
    //lcd.print("test");
    //alarm();
}

void loop()
{   
    if(stoper(500, &show_temp))
    {
        show = !show;
    }

    currenttime = millis();

    int currentRightState = digitalRead(RIGHT_BUTTON_PIN);
    int currentLeftState = digitalRead(LEFT_BUTTON_PIN);

    if(currentRightState == LOW && currentRightState == LOW)
    {
        if(digitalRead(RIGHT_BUTTON_PIN) == LOW && digitalRead(LEFT_BUTTON_PIN) == LOW && stoper(3000, &setclock_temp) && !setclock_flag)
        {
            beepon = true;
            setclock_flag = true;
            setclock();
        }   
    }
    else
    {
        setclock_flag = false;
    }

    lastLeftButtonState = currentLeftState;
    lastRightButtonState = currentRightState; 

    // if(currenttime - clock_temp >= 1000)
    // {
    //     digitalclock();
    //     clock_temp = currenttime;
    // }

    if(stoper(1000, &clock_temp))
    {
        digitalclock();
    }

    if(stoper(100, &setbutton_temp))
    {
        int currentSetState = digitalRead(SET_BUTTON_PIN);

        if(currentSetState == LOW && lastSetButtonState == HIGH)
        {
            beepon = true;

        }
        lastSetButtonState = currentSetState;
    }
    

    beep();
    alarm();
    brightnessControl();


    // Serial.println(timepassed);
    // delay(1000-timepassed);
}

void digitalclock()
{
    char sseconds[3] = {};
    char sminutes[3] = {};
    char shour[3] = {};
    char sday[3] = {};
    char smonth[3] = {};
    char syear[5] = {};
    char sClockElements[6][5] = {};


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

    snprintf(sClockElements[0], 3, "%02d", seconds);
    snprintf(sClockElements[1], 3, "%02d", minutes);
    snprintf(sClockElements[2], 3, "%2d", hour + dst);
    snprintf(sClockElements[3], 3, "%2d", day);
    snprintf(sClockElements[4], 3, "%02d", month);
    snprintf(sClockElements[5], 5, "%4d", year);    

    for(int i = 0; i < clockElementsAmount; i++)
    {
        displayValue(elementsPositions[i], (int)i/3.0,  sClockElements[i]);
    }

    // lcd.setCursor(POS_OFFSET, 0);
    // if(hour + adddst < 10)
    // {
    //     lcd.print(" ");
    // }
    // lcd.print(hour + adddst);
    // lcd.print(":");

    // snprintf(sminutes, 3, "%02d", minutes);
    // lcd.setCursor(POS_OFFSET + 3, 0);
    // lcd.print(sminutes);
    // lcd.print(":");

    // snprintf(sseconds, 3, "%02d", seconds);
    // lcd.setCursor(POS_OFFSET + 6, 0);
    // lcd.print(sseconds);
    
    lcd.setCursor(13, 0);
    lcd.print(sdayofweek_pl[dayofweek(year, month, day)]);


    // lcd.setCursor(POS_OFFSET - 3, 1);
    // if(day < 10)
    // {
    //     lcd.print(" ");
    // }
    // lcd.print(day);
    // lcd.print(".");

    // snprintf(smonth, 3, "%02d", month);
    // lcd.print(smonth);
    // lcd.print(".");

    // lcd.print(year);

    //Serial.print(sseconds);

    displaySigns();
    
    seconds++;
    //day++;
    if(alarmseton && hour + adddst == 1 && minutes == 60)
    {
        turnonalarm = true;
    }
    else
    {
        turnonalarm = false;
        digitalWrite(BUZZER_PIN, LOW);
    }
    // else{
    //     delay(1000);
    // }
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
    if(turnonalarm && currenttime - alarm_temp >= 500)
    {
        digitalWrite(BUZZER_PIN, alarmstate);
        alarmstate = !alarmstate;
        alarm_temp = currenttime;
    }

}

void brightnessControl()
{
    int ilumination = analogRead(FOTO_PIN);
    ilumination = map(ilumination, 0, 1023, 10, 255);
    analogWrite(BRIGHTNESS_PIN, ilumination);
}

void setclock()
{
    int i = 0;
    while(i < clockElementsAmount)
    {

    }

}

void beep()
{   
    if(beepon)
    {   
        digitalWrite(BUZZER_PIN, HIGH);    
        // beepcounter++;
        if(stoper(150, &beep_temp))
        {
            //beepcounter = 0;
            // beep_temp = 0;
            digitalWrite(BUZZER_PIN, LOW);
            beepon = false;
        }
    }

}

int stoper(int tocountdown, long int *temp)
{
    if(*temp == 0)
    {
        *temp = currenttime;
    }
    if(currenttime - *temp >= tocountdown && *temp != 0)
    {
        *temp = 0;
        return 1;
    }

    return 0;
}

// void blink(int position, int row, int value, bool show)
// {
//     char sValue[5] = {};
//     snprintf(sValue, 5, "%02d", value);

//     if(show)
//     {
//         lcd.setCursor(position, row);

//         lcd.print();
//     }
// }

void displayValue(int position, int row, char* value)
{
    lcd.setCursor(position, row);
    lcd.print(value);
}

 void displaySigns()
{
    lcd.setCursor(7, 0);
    lcd.print(":");
    lcd.setCursor(10, 0);
    lcd.print(":");
    lcd.setCursor(4, 1);
    lcd.print(".");
    lcd.setCursor(7, 1);
    lcd.print(".");
    if(alarmseton)
    {
        lcd.setCursor(15, 1);
        lcd.write(byte(0));
    }
}