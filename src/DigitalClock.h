#include<Arduino.h>
// #include "avr8-stub.h"           //for debuging
// #include "app_api.h"             //for debuging
#include "Timer_Beep_Modulo.h"
#include "Alarm.h"
#include "Clock.h"
#include "Buttons.h"
#include "DisplayClock.h"

_Alarm Alarm;
_Clock Clock;
_SetClock SetClock;
_DispClock DispClock;

const int BRIGHTNESS_PIN = 11;

unsigned long currenttime = 0;

int brightness = 100;
bool beepon = false;
unsigned long beep_timer = 0;

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