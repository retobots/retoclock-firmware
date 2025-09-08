#pragma once
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

class TftDisplay {
public:
    static TftDisplay& getInstance();

    void setup(uint8_t csPin, uint8_t dcPin, uint8_t rstPin);

    void turnOff();
    void clear();
    void setCursor(int16_t x, int16_t y);
    void setTextColor(uint16_t color);
    void setTextSize(uint8_t size);
    void print(const char* str);

    void showTimeSetting(uint8_t hour, uint8_t minute, bool editingHour);
    void resetLastStatus();


private:
    TftDisplay() = default;
    Adafruit_ST7789* _tft = nullptr;
    private:
    String _lastStatus = "";
    String _lastNextFeed = "";
    String _lastLine1 = "";
    bool _lastCharging = false;

};
