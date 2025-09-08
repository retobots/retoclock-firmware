#pragma once
#include <Arduino.h>

class ButtonModule {
    public:
        ButtonModule(uint8_t pin);
        void begin();
        bool wasPress();
    
    private:
        uint8_t pin;    

};