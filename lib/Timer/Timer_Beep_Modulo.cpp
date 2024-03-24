#include "Timer_Beep_Modulo.h"

int Timer(unsigned int tocountdown, unsigned long *temp)
{
    if(*temp == 0)
    {
        *temp = currenttime;
    }
    else if(currenttime - *temp >= tocountdown)
    {
        //Serial.println(*temp);
        *temp = 0;
        return 1;
    }

    return 0;
}

void Beep()
{   
    if(beepon)
    {   
        digitalWrite(BUZZER_PIN, HIGH);    
        // beepon = false;
        // beepcounter++;
        // Serial.println(beep_timer);
        if(Timer(150, &beep_timer))
        {
            //beepcounter = 0;
            // beep_timer = 0;
            digitalWrite(BUZZER_PIN, LOW);
            beepon = false;
        }
    }
}

int Mod(int A, int B)
{
	int modulo = A - B*floor(((double)A/(double)B));
	return modulo;
}