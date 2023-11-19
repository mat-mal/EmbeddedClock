#include<Arduino.h>
#include "avr8-stub.h"
#include "app_api.h"
#include<LiquidCrystal.h>
#include<math.h>

LiquidCrystal lcd(2,3,4,5,6,7);

const int POS_OFFSET = 5;
const int BUZZER_PIN = 8;
const int LEFT_BUTTON_PIN = 9;
const int SET_BUTTON_PIN = 10;
const int RIGHT_BUTTON_PIN = 12;
const int BRIGHTNESS_PIN = 11;
const int FOTO_PIN = A0;
const int clockElementsAmount = 6;

struct _Alarm
{
    unsigned long alarm_temp = 0;
    bool setAlarm_flag = true;
    bool alarmOn_flag = false;
    int alarmState = LOW;
};

struct _Clock
{
    int elementsValue[6] = {50, 59, 23, 0, 0, 2023};
    int* seconds = &elementsValue[0];
    int* minutes = &elementsValue[1]; 
    int* hour = &elementsValue[2];
    int* day = &elementsValue[3];
    int* month = &elementsValue[4];
    int* year = &elementsValue[5];
    int elementsPositions[6] = {POS_OFFSET + 6, POS_OFFSET + 3,  POS_OFFSET, POS_OFFSET - 3, POS_OFFSET, POS_OFFSET + 3};
    String sdayofweek_eng[7] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
    String sdayofweek_pl[7] = {" Nd", " Pn", " Wt", " Śr", "Czw", " Pt", " So"};
    int dst = 1;
    char sClockElements[6][5] = {};
    bool digitalclock_flag = true;
    bool dstAdded = false;
};

_Alarm Alarm;
_Clock Clock;
unsigned long currenttime = 0, clock_temp = 0, beep_temp = 0, setclock_temp = 0, setbutton_temp = 0, show_temp = 0, setclock_temp2 = 0, leftbutton_temp = 0, rightbutton_temp = 0;



int beepcounter = 0;
bool beepon = false;
int lastSetButtonState = HIGH;
int lastRightButtonState = HIGH;
int lastLeftButtonState = HIGH;
int countmiliseconds = 0;
bool timer_flag = false;
bool setclock_flag = false;
bool entersetclock_flag = false;
bool show = false;
int setclock_countposition = 0;


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
void TurnOnAlarm();
void setalarm();
void brightnessControl();
void setclock();
void beep();
int timer(unsigned int tocountdown, unsigned long *temp);
void blink(int position, int row, char*);
void displayValue(int position, int row, char* value);
void displaySigns();
int pushsetbutton();
int pushLeftButton();
int pushRightButton();
int Mod(int A, int B);
void ClockToString(int value, int i);


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
    currenttime = millis();

    // int currentRightState = digitalRead(RIGHT_BUTTON_PIN);
    // int currentLeftState = digitalRead(LEFT_BUTTON_PIN);

    if(digitalRead(LEFT_BUTTON_PIN) == LOW && digitalRead(RIGHT_BUTTON_PIN) == LOW)
    {
        if(digitalRead(RIGHT_BUTTON_PIN) == LOW && digitalRead(LEFT_BUTTON_PIN) == LOW && !entersetclock_flag && timer(3000, &setclock_temp))
        {
            beepon = true;
            entersetclock_flag = true;
            setclock_flag = true;
        }   
    }
    else
    {
        entersetclock_flag = false;
    }

    // lastLeftButtonState = currentLeftState;
    // lastRightButtonState = currentRightState; 

    // if(currenttime - clock_temp >= 1000)
    // {
    //     digitalclock();
    //     clock_temp = currenttime;
    // }

    if(timer(1000, &clock_temp) && Clock.digitalclock_flag)
    {
        digitalclock();
    }


    //pushsetbutton();
    setclock();
    beep();
    TurnOnAlarm();
    brightnessControl();


    // Serial.println(timepassed);
    // delay(1000-timepassed);
}

void digitalclock()
{
    if(*Clock.seconds > 59)
    {
        *Clock.seconds = 0;
        (*Clock.minutes)++;

        if(*Clock.minutes > 59)
        {
            *Clock.minutes = 0;
            (*Clock.hour)++;

            if(*Clock.hour > 23)
            {
                *Clock.hour = 0;
                (*Clock.day)++;

                if(*Clock.day > 31 && (*Clock.month == 1 || *Clock.month == 3 || *Clock.month == 5 || *Clock.month == 7 || *Clock.month == 8 || *Clock.month == 10 || *Clock.month == 12))
                {
                    *Clock.day = 1;
                    (*Clock.month)++;

                    if(*Clock.month > 12)
                    {
                        *Clock.month = 1;
                        (*Clock.year)++;
                    }
                }
                else if(*Clock.day > 30 && (*Clock.month == 4 || *Clock.month == 6 || *Clock.month == 9 || *Clock.month == 11))
                {
                    *Clock.day = 1;
                    (*Clock.month)++;
                }                  
                else if(*Clock.day > 28 && *Clock.month == 2 && !IsLeapYear(*Clock.year))
                {
                    *Clock.day = 1;
                    (*Clock.month)++;
                }
                else if(*Clock.day > 29 && *Clock.month == 2)
                {
                    *Clock.day = 1;
                    (*Clock.month)++;
                }
            }
        }
    }

    // snprintf(Clock.sClockElements[0], 3, "%02d", Clock.seconds);
    // snprintf(Clock.sClockElements[1], 3, "%02d", Clock.minutes);
    // snprintf(Clock.sClockElements[2], 3, "%2d", Clock.hour + Clock.dst);
    // snprintf(Clock.sClockElements[3], 3, "%2d", Clock.day);
    // snprintf(Clock.sClockElements[4], 3, "%02d", Clock.month);
    // snprintf(Clock.sClockElements[5], 5, "%4d", Clock.year);    

    for(int i = 0; i < clockElementsAmount; i++)
    {
        // displayValue(Clock.elementsPositions[i], (int)i/3.0,  Clock.sClockElements[i]);
        ClockToString(Clock.elementsValue[i], i);
        displayValue(Clock.elementsPositions[i], (int)i/3.0, Clock.sClockElements[i]);
    }

    lcd.setCursor(13, 0);
    lcd.print(Clock.sdayofweek_pl[dayofweek(*Clock.year, *Clock.month, *Clock.day)]);

    //Serial.print(sseconds);

    displaySigns();
    
    (*Clock.seconds)++;
    //day++;
    if(Alarm.setAlarm_flag && *Clock.hour == 1 && *Clock.minutes == 60)
    {
        Alarm.alarmOn_flag = true;
    }
    else
    {
        Alarm.alarmOn_flag = false;
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

    if(m + 1 < 3)
    {
        y -= 1;
    }
    int dow = (y + y/4 - y/100 + y/400 + t[m] + d + 1)%7; 

    return dow;
}

void TurnOnAlarm()
{   
    if(Alarm.alarmOn_flag && currenttime - Alarm.alarm_temp >= 500)
    {
        digitalWrite(BUZZER_PIN, Alarm.alarmState);
        Alarm.alarmState = !Alarm.alarmState;
        Alarm.alarm_temp = currenttime;
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
    if(setclock_flag)
    {
        Clock.digitalclock_flag = false;
        if(setclock_countposition == 0)
        {
            blink(13, 0, "DST");
            if(Clock.dst == 1)
            {
                blink(13, 1, " ON");
            }
            else
            {
                blink(13, 1, "OFF");
            }
            if(pushLeftButton())
            {
                Clock.dst = !Clock.dst;
            }
            if(pushsetbutton())
            {
                setclock_countposition++;
                displayValue(13, 0, "DST");
                if(Clock.dst == 1)
                {
                    displayValue(13, 1, " ON");
                }
                else
                {
                    displayValue(13, 1, "OFF");
                }
            }

        }
        else if(setclock_countposition < clockElementsAmount)
        {

            blink(Clock.elementsPositions[setclock_countposition], (int)setclock_countposition/3.0, Clock.sClockElements[setclock_countposition]);
            // int value = strtol(Clock.sClockElements[setclock_countposition], NULL, 10);
            // pushLeftButton(Clock.elementsValue[setclock_countposition]);
            
            if(pushLeftButton())
            {
                switch (setclock_countposition)
                {
                case 0:
                case 1:
                    Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] - 1, 60);
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;
                case 2:
                    Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] - 1, 24);
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;
                case 3:
                    if(*Clock.month == 0 || *Clock.month == 2 || *Clock.month == 4 || *Clock.month == 6 || *Clock.month == 7 || *Clock.month == 9 || *Clock.month == 11)
                    {
                        Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] - 1, 31);
                    }
                    else if(*Clock.month == 3 || *Clock.month == 5 || *Clock.month == 8 || *Clock.month == 1)
                    {
                        Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] - 1, 30);
                    }
                    else if(*Clock.month == 1 && !IsLeapYear(*Clock.year))
                    {
                        Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] - 1, 28);
                    }
                    else if(*Clock.month == 1)
                    {
                        Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] - 1, 29);
                    }
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;
                case 4:
                    Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] - 1, 12);
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;
                case 5:
                    Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] - 1, 9999);
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;                                                        
                default:
                    break;
                }
            }

            if(pushRightButton())
            {
                switch (setclock_countposition)
                {
                case 0:
                case 1:
                    Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] + 1, 60);
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;
                case 2:
                    Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] + 1, 24);
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;
                case 3:
                    if(*Clock.month == 0 || *Clock.month == 2 || *Clock.month == 4 || *Clock.month == 6 || *Clock.month == 7 || *Clock.month == 9 || *Clock.month == 11)
                    {
                        Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] + 1, 31);
                    }
                    else if(*Clock.month == 3 || *Clock.month == 5 || *Clock.month == 8 || *Clock.month == 10)
                    {
                        Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] + 1, 30);
                    }
                    else if(*Clock.month == 1 && !IsLeapYear(*Clock.year))
                    {
                        Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] + 1, 28);
                    }
                    else if(*Clock.month == 1)
                    {
                        Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] + 1, 29);
                    }
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;
                case 4:
                    Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] + 1, 12);
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;
                case 5:
                    Clock.elementsValue[setclock_countposition] = Mod(Clock.elementsValue[setclock_countposition] + 1, 9999);
                    ClockToString(Clock.elementsValue[setclock_countposition], setclock_countposition);
                    break;                                                        
                default:
                    break;
                }
            }

            if(pushsetbutton())
            {
                displayValue(Clock.elementsPositions[setclock_countposition], (int)setclock_countposition/3.0, Clock.sClockElements[setclock_countposition]);
                setclock_countposition++;
            }
        }
        else
        {
            setclock_flag = false;
            setclock_countposition = 0;
            Clock.digitalclock_flag = true;
        }
    }
}

void beep()
{   
    if(beepon)
    {   
        digitalWrite(BUZZER_PIN, HIGH);    
        // beepcounter++;
        if(timer(150, &beep_temp))
        {
            //beepcounter = 0;
            // beep_temp = 0;
            digitalWrite(BUZZER_PIN, LOW);
            beepon = false;
        }
    }

}

int timer(unsigned int tocountdown, unsigned long *temp)
{
    if(*temp == 0)
    {
        *temp = currenttime;
    }
    else if(currenttime - *temp >= tocountdown)
    {
        *temp = 0;
        return 1;
    }

    return 0;
}

// void blink(int position, int row, int value = 0, int i = 0)
// {
//     if(timer(500, &show_temp))
//     {
//         show = !show;
//     }

//     // int len = strlen(value);

//     if(show)
//     {
//         displayValue(position, row, value, i);
//     }
//     else
//     {
//         lcd.setCursor(position, row);
//         if(i < 5)
//         {
//             lcd.print("  ");
//         }
//         if(i == 5)
//         {
//            lcd.print("    ");            
//         }
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
    if(Alarm.setAlarm_flag)
    {
        lcd.setCursor(15, 1);
        lcd.write(byte(0));
    }
}

int pushsetbutton()
{
    int setbuttonpressed = 0;
    if(timer(100, &setbutton_temp))
    {
        int currentSetState = digitalRead(SET_BUTTON_PIN);

        if(currentSetState == LOW && lastSetButtonState == HIGH)
        {
            beepon = true;
            setbuttonpressed = 1;
        }
        lastSetButtonState = currentSetState;
    }
    return setbuttonpressed;
}

int pushLeftButton()
{
    int buttonpressed = 0;
    if(timer(100, &leftbutton_temp))
    {
        int currentLeftState = digitalRead(LEFT_BUTTON_PIN);

        if(currentLeftState == LOW && lastLeftButtonState == HIGH)
        {
            beepon = true;
            buttonpressed = 1;
        }
        lastLeftButtonState = currentLeftState;
    }
    return buttonpressed;
}

int pushRightButton()
{
    int buttonpressed = 0;
    if(timer(100, &rightbutton_temp))
    {
        int currentRightState = digitalRead(RIGHT_BUTTON_PIN);

        if(currentRightState == LOW && lastRightButtonState == HIGH)
        {
            beepon = true;
            buttonpressed = 1;
        }
        lastRightButtonState = currentRightState;
    }
    return buttonpressed;
}

int Mod(int A, int B)
{
	int modulo = A - B*floor(((double)A/(double)B));
	return modulo;
}

void AddDst()
{
    if(Clock.dst && !Clock.dstAdded)
    {
        (*Clock.hour)++;
        if(*Clock.hour > 24)
        {

        }
        Clock.dstAdded = true;
    }
    else if(!Clock.dst && Clock.dstAdded)
    {
        Clock.dstAdded = false;
    }
}

void blink(int position, int row, char* value)
{
    if(timer(500, &show_temp))
    {
        show = !show;
    }

    int len = strlen(value);

    if(show)
    {
        displayValue(position, row, value);
    }
    else
    {
        lcd.setCursor(position, row);
        for(int i = 0; i < len; i++)
        {
            lcd.print(" ");
        }
    }

    // if(show)
    // {
    //     lcd.setCursor(position, row);
    //     lcd.print(value);
    // }
    // else
    // {
    //     lcd.setCursor(position, row);
    //     if(i < 5)
    //     {
    //         lcd.print("  ");
    //     }
    //     if(i == 5)
    //     {
    //        lcd.print("    ");            
    //     }
    // }

}

void ClockToString(int value, int i)
{
    // char sValue[5] = {};
    switch(i)
    {
        case 0:
        case 1:
            snprintf(Clock.sClockElements[i], 3, "%02d", value);
            break;        
        case 2:
            snprintf(Clock.sClockElements[i], 3, "%2d", value);
            break;
        case 3:
            snprintf(Clock.sClockElements[i], 3, "%2d", value + 1);
            break;
        case 4:
            snprintf(Clock.sClockElements[i], 3, "%02d", value + 1);
            break;            
        case 5:
            snprintf(Clock.sClockElements[i], 5, "%4d", value);
            break;
        default:
            break;   
    }
}