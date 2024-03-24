#include "Buttons.h"

_Button LButton;
_Button RButton;
_Button SetButton;
_Button LRButtons;

int PushLRButtons(struct _Button *Button, int PIN)
{
    int buttonpressed = 0;
    int hold;
    if(!Button->hold_flag)
    {
        hold = 500;
    }
    else
    {
        hold = 250;
    }
    if(digitalRead(PIN) == LOW) 
    {
        if(Button->lastState == HIGH && Timer(hold, &Button->button_timer))
        {
            if(Timer(2000, &Button->hold_timer))
            {
                Button->hold_flag = true;
            }
            beepon = true;
            buttonpressed = 1;
        }
    }
    else
    {
        Button->lastState = HIGH;
        Button->hold_timer = 0;
        Button->hold_flag = false;
    }
        
    return buttonpressed;
}

int HoldLRButtons()
{
    int buttonsHold = 0;
    if(digitalRead(LEFT_BUTTON_PIN) == LOW && digitalRead(RIGHT_BUTTON_PIN) == LOW)
    {
        if(!LRButtons.hold_flag && Timer(3000, &LRButtons.hold_timer))
        {
            LRButtons.hold_flag = true;
            beepon = true;
            buttonsHold = 1;
            LRButtons.lastState = LOW;
        }
    }
    else
    {
        LRButtons.hold_timer = 0;
        LRButtons.hold_flag = false;
        LRButtons.lastState = HIGH;
    }
        
    return buttonsHold;
}

int HoldSetButton()
{
    int buttonhold = 0;
    if(digitalRead(SET_BUTTON_PIN) == LOW)
    {
        if(!SetButton.hold_flag && Timer(2000, &SetButton.hold_timer))
        {
            SetButton.hold_flag = true;
            beepon = true;
            buttonhold = 1;
            SetButton.lastState = LOW;
        }
    }
    else
    {
        SetButton.hold_timer = 0;
        SetButton.hold_flag = false;
        SetButton.lastState = HIGH;
    }
        
    return buttonhold;
}

int PushSetButton()
{
    int buttonpressed = 0;
    if(Timer(100, &SetButton.button_timer))
    {
        int currentState = digitalRead(SET_BUTTON_PIN);

        if(currentState == LOW && SetButton.lastState == HIGH && (millis() - SetButton.button_timer) > 20)
        {
            beepon = true;
            buttonpressed = 1;
        }
        SetButton.lastState = currentState;
    }

    return buttonpressed;
}