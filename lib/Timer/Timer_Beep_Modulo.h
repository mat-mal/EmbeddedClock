#ifndef _TIMER_H_
#define _TIMER_H_
#include<Arduino.h>

const int BUZZER_PIN = 8;
extern unsigned long currenttime;
// extern unsigned long temptime;
extern unsigned long beep_timer;
extern bool beepon;

int Timer(unsigned int tocountdown, unsigned long *timer);
void Beep();
int Mod(int A, int B);

#endif