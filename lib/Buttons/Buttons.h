#ifndef _BUTTONS_H_
#define _BUTTONS_H_
#include "Timer_Beep_Modulo.h"
#include <Arduino.h>

const int LEFT_BUTTON_PIN = 9;
const int SET_BUTTON_PIN = 10;
const int RIGHT_BUTTON_PIN = 12;

struct _Button
{
    unsigned long button_timer = 0;
    bool hold_flag;
    unsigned long hold_timer = 0;
    int lastState;
};

extern _Button LButton;
extern _Button RButton;

int PushSetButton();
int PushLRButtons(struct _Button *Button, int PIN);
int HoldSetButton();
int HoldLRButtons();

#endif