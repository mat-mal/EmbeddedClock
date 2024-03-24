#include "Alarm.h"
#include <Arduino.h>

int j = 0;

void EnterSetAlarm()
{
    if(HoldSetButton() && Clock.digitalClock_flag && !Alarm.enterSetAlarm_flag)
    {
        Alarm.enterSetAlarm_flag = true;
        DispClock.displayClock_flag = false;
        lcd.clear();

        for(int i = 0; i < Alarm.numberOfElements; i++)
        {
            AlarmToString(Alarm.alarmTime[i], i, Alarm.sAlarmTime[i]);
            DisplayValue(Alarm.positions[i], 0, Alarm.sAlarmTime[i]);
        }
    }
}

void SetAlarm()
{
    if(Alarm.enterSetAlarm_flag)
    {
        Alarm.armed_flag = false;

        lcd.setCursor(7, 0);
        lcd.print(":");
        char bellSign[2] = "\x08";              //converting custom sigh to char to use it in Blink function
        Blink(15, 1, bellSign);

        if(Alarm.count < Alarm.numberOfElements)
        {
            Blink(Alarm.positions[Alarm.count], 0, Alarm.sAlarmTime[Alarm.count]);
            if(PushLRButtons(&LButton, LEFT_BUTTON_PIN))
            {
                switch (Alarm.count)
                {
                case 0:
                    Alarm.alarmTime[Alarm.count] = Mod(Alarm.alarmTime[Alarm.count] - 1, 60);
                    AlarmToString(Alarm.alarmTime[Alarm.count], Alarm.count, Alarm.sAlarmTime[Alarm.count]);
                    break;
                case 1:
                    Alarm.alarmTime[Alarm.count] = Mod(Alarm.alarmTime[Alarm.count] - 1, 24);
                    AlarmToString(Alarm.alarmTime[Alarm.count], Alarm.count, Alarm.sAlarmTime[Alarm.count]);
                    break;
                default:
                    break;
                }
            }
            if(PushLRButtons(&RButton, RIGHT_BUTTON_PIN))
            {
                switch (Alarm.count)
                {
                case 0:
                    Alarm.alarmTime[Alarm.count] = Mod(Alarm.alarmTime[Alarm.count] + 1, 60);
                    AlarmToString(Alarm.alarmTime[Alarm.count], Alarm.count, Alarm.sAlarmTime[Alarm.count]);
                    break;
                case 1:
                    Alarm.alarmTime[Alarm.count] = Mod(Alarm.alarmTime[Alarm.count] + 1, 24);
                    AlarmToString(Alarm.alarmTime[Alarm.count], Alarm.count, Alarm.sAlarmTime[Alarm.count]);
                    break;
                default:
                    break;
                }
            }
            if(PushSetButton())
            {
                DisplayValue(Alarm.positions[Alarm.count], 0, Alarm.sAlarmTime[Alarm.count]);
                Alarm.count++;
            }
            
        }
        else
        {
            Alarm.enterSetAlarm_flag = false;
            Alarm.count = 0;
            Alarm.armed_flag = true;
        }
    }
    else
    {
        DispClock.displayClock_flag = true;
    }
}

void StartAlarm()
{   
    if(Alarm.armed_flag && (*Clock.hour + Clock.dst) == *Alarm.hour && *Clock.minutes == *Alarm.minutes)
    {
        if(!Alarm.mute_flag)
        {
            Alarm.turnOn_flag = true;
        }
    }
    else
    {
        Alarm.turnOn_flag = false;
        Alarm.mute_flag = false;
    }
    if(Alarm.turnOn_flag)
    {
        if(j == 8)
        {
            Alarm.alarmState = LOW;
            digitalWrite(BUZZER_PIN, Alarm.alarmState);
            if(Timer(500, &Alarm.alarm_timer))
            {
                j = 0;
            }
        }

        if(j < 8 && Timer(50, &Alarm.alarm_timer))
        {
            Alarm.alarmState = !Alarm.alarmState;
            digitalWrite(BUZZER_PIN, Alarm.alarmState);
            j++;
        }

    }

    if(!Alarm.turnOn_flag && Alarm.alarmState == HIGH)
    {
        Alarm.alarmState = LOW;
        digitalWrite(BUZZER_PIN, Alarm.alarmState);
    }
}

void AlarmTurnOnOff()
{
    if(Clock.digitalClock_flag && !Alarm.enterSetAlarm_flag && PushSetButton())
    {
        if(Alarm.turnOn_flag)
        {
            Alarm.turnOn_flag = false;
            Alarm.mute_flag = true;
        }
        else
        {
            Alarm.armed_flag = !Alarm.armed_flag;
        }
    }
}

void AlarmToString(int value, int i, char* sValue)
{
    switch(i)
    {
        case 0:
            snprintf(sValue, 3, "%02d", value);
            break;        
        case 1:
            snprintf(sValue, 3, "%2d", value);
            break;
        default:
            break;   
    }
}