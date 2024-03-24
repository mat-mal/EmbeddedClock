#ifndef _ALARM_H_
#define _ALARM_H_
#include <Arduino.h>
#include "Clock.h"
#include "Timer_Beep_Modulo.h"
#include "Buttons.h"
#include "DisplayClock.h"

const int ALM_POS_OFFSET = 5;

struct _Alarm
{
    unsigned long alarm_timer = 0;
    bool enterSetAlarm_flag = false;
    bool turnOn_flag = false;
    bool armed_flag = false;
    int alarmState = LOW;
    int numberOfElements = 2;
    int alarmTime[2] = {1, 0};
    int* minutes = &alarmTime[0];
    int* hour = &alarmTime[1];
    int count = 0;
    int positions[2] = {ALM_POS_OFFSET + 3,  ALM_POS_OFFSET};
    char sAlarmTime[2][3] = {};
    bool mute_flag = false;

};

extern _Alarm Alarm;

void StartAlarm();
void SetAlarm();
void EnterSetAlarm();
void AlarmTurnOnOff();
void AlarmToString(int value, int i, char* sValue);

#endif