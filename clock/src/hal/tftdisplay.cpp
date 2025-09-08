#include "hal/tftdisplay.h"
#define TFT_WIDTH  172
#define TFT_HEIGHT 320

#define TFT_BL 12  // Chân điều khiển đèn nền (nếu bạn dùng chân khác thì đổi lại)

TftDisplay& TftDisplay::getInstance() {
    static TftDisplay instance;
    return instance;
}

void TftDisplay::setup(uint8_t csPin, uint8_t dcPin, uint8_t rstPin) {
    pinMode(TFT_BL, OUTPUT);
    digitalWrite(TFT_BL, HIGH);  // Bật đèn nền

    _tft = new Adafruit_ST7789(csPin, dcPin, rstPin);
    _tft->init(TFT_WIDTH, TFT_HEIGHT);
    _tft->setRotation(1);
    _tft->fillScreen(ST77XX_BLACK);
    _tft->setTextWrap(false);
}

void TftDisplay::clear() {
    _tft->fillScreen(ST77XX_BLACK);
}

void TftDisplay::turnOff() {
    digitalWrite(TFT_BL, LOW);  // Tắt đèn nền
}

void TftDisplay::setCursor(int16_t x, int16_t y) {
    _tft->setCursor(x, y);
}

void TftDisplay::setTextColor(uint16_t color) {
    _tft->setTextColor(color);
}

void TftDisplay::setTextSize(uint8_t size) {
    _tft->setTextSize(size);
}

void TftDisplay::print(const char* str) {
    _tft->print(str);
}

void TftDisplay::resetLastStatus() {
    _lastStatus = "";
    _lastNextFeed = "";
    _lastLine1 = "";
    _lastCharging = false;
}

void TftDisplay::showTimeSetting(uint8_t hour, uint8_t minute, bool selectingHour) {
    _tft->fillScreen(ST77XX_BLACK);

    _tft->setTextSize(3);
    _tft->setTextColor(ST77XX_WHITE);
    _tft->setCursor(20, 40);
    _tft->print("SET ALARM");

    _tft->setTextSize(4);
    _tft->setCursor(40, 100);

    if (selectingHour) {
        _tft->setTextColor(ST77XX_YELLOW);
    } else {
        _tft->setTextColor(ST77XX_WHITE);
    }
    if (hour < 10) _tft->print("0");
    _tft->print(hour);
    _tft->print(":");

    if (!selectingHour) {
        _tft->setTextColor(ST77XX_YELLOW);
    } else {
        _tft->setTextColor(ST77XX_WHITE);
    }
    if (minute < 10) _tft->print("0");
    _tft->print(minute);
}
