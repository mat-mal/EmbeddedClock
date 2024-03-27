#include "Timer_Beep_Modulo.h"

int Timer(unsigned int tocountdown, unsigned long *temp)
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

void Beep()
{   
    if(beepon)
    {   
        digitalWrite(BUZZER_PIN, HIGH);    
        if(Timer(150, &beep_timer))
        {
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