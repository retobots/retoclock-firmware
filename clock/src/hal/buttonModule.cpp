#include "hal/buttonModule.h"

ButtonModule::ButtonModule(uint8_t pin): pin(pin){}

void ButtonModule::begin()
{
    pinMode(pin, INPUT_PULLUP);
}

bool ButtonModule::wasPress()
{
    if(digitalRead(pin)==LOW)
    {
        delay(20);
        if(digitalRead(pin)==LOW)
        {
            while(digitalRead(pin)==LOW);
            delay(20);
            return true;
        }
    }
    return false;
}