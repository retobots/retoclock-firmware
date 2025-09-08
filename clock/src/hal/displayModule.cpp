#include "hal/displayModule.h"

displayModule::displayModule(uint8_t dataPin, uint8_t clockPin, uint8_t latchPin): dataPin(dataPin), clockPin(clockPin), latchPin(latchPin){}

void displayModule::begin()
{
    pinMode(dataPin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(latchPin, OUTPUT);
}

void displayModule:: showDigits(byte d0, byte d1, byte d2, byte d3)
{
    digitalWrite(latchPin,LOW);
    shiftOut(dataPin, clockPin, MSBFIRST,d3);
    shiftOut(dataPin, clockPin, MSBFIRST,d2);
    shiftOut(dataPin, clockPin, MSBFIRST,d1);
    shiftOut(dataPin, clockPin, MSBFIRST,d0);
    digitalWrite(latchPin,HIGH); 

    // Lưu lại thời điểm hiển thị
    lastShowTime = millis();
    isShowing = true;
}

void displayModule::update()
{
    if (isShowing && (millis() - lastShowTime >= DISPLAY_TIMEOUT )) { 
        // Sau 1 giây thì tắt
        digitalWrite(latchPin, LOW);
        for (int i = 0; i < 4; i++) {
            shiftOut(dataPin, clockPin, MSBFIRST, 0x00);
        }
        digitalWrite(latchPin, HIGH);
        isShowing = false;
    }
}