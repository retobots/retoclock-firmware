#pragma once
#include <Arduino.h>

const unsigned long DISPLAY_TIMEOUT = 2000;
class displayModule {
    public:
        displayModule(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin);
        void begin();
        void showDigits(byte d3, byte d2, byte d1, byte d0);
        unsigned long lastShowTime;
        bool isShowing;
        void update();

    
    private:
        u_int8_t dataPin, clockPin, latchPin;

};