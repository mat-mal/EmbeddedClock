#include "DigitalClock.h"

void setup()
{
    // debug_init();            //for debuging, hard to use with inputs
    // Serial.begin(9600);      //for debuging, "easy" to use
    analogWrite(BRIGHTNESS_PIN, brightness);
    lcd.begin(16, 2);
    lcd.clear();

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(LEFT_BUTTON_PIN, INPUT_PULLUP);
    pinMode(SET_BUTTON_PIN, INPUT_PULLUP);
    pinMode(RIGHT_BUTTON_PIN, INPUT_PULLUP);
    lcd.createChar(0, Bell);
}

void loop()
{   
    currenttime = millis();

    DigitalClock();
    EnterClockSetting();
    SettingClock();
    StartAlarm();
    EnterSetAlarm();
    SetAlarm();
    AlarmTurnOnOff();
    DisplayClock();
    Beep();
}