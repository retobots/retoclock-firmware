#include "hal/Clockmodule.h"
#include <Wire.h>
#include <Arduino.h>
#include <WiFi.h>
#include <time.h>

// Thông tin WiFi của bạn
const char* ssid     = "Retobots";
const char* password = "retobot@2025";

// NTP config
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 7 * 3600; // GMT+7
const int daylightOffset_sec = 0;

Clockmodule::Clockmodule() {}

void Clockmodule::begin() {
    Wire.begin();

    if (!rtc.begin()) {
        Serial.println("❌ Không tìm thấy module DS3231!");
        while (1);
    }

    if (rtc.lostPower()) {
        Serial.println("⚠️ DS3231 bị mất nguồn, cần cập nhật lại thời gian.");
    }

    if (syncWithNTP()) {
        Serial.println("✅ Cập nhật thời gian từ NTP thành công.");
    } else {
        Serial.println("❌ Không thể cập nhật thời gian từ NTP.");
    }
}

bool Clockmodule::lostPower() {
    return rtc.lostPower();
}

DateTime Clockmodule::getTime() {
    return rtc.now();
}

bool Clockmodule::syncWithNTP() {
    // Bắt đầu kết nối WiFi
    WiFi.begin(ssid, password);
    Serial.print("🔌 Đang kết nối WiFi: ");
    Serial.println(ssid);

    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 20) {
        delay(500);
        Serial.print(".");
        retry++;
    }
    Serial.println();

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("❌ Không thể kết nối WiFi");
        Serial.print("WiFi Status: ");
        Serial.println(WiFi.status());
        return false;
    }

    Serial.println("✅ Đã kết nối WiFi");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    struct tm timeinfo;
    delay(1500);  // chờ NTP response về

    if (!getLocalTime(&timeinfo)) {
        Serial.println("❌ Không lấy được thời gian từ NTP");
        return false;
    }

    // In thời gian từ NTP ra Serial để kiểm tra
    Serial.printf("🕒 NTP time: %04d-%02d-%02d %02d:%02d:%02d\n",
                  timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
                  timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    rtc.adjust(DateTime(
        timeinfo.tm_year + 1900,
        timeinfo.tm_mon + 1,
        timeinfo.tm_mday,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec
    ));

    // Ngắt WiFi nếu không dùng nữa
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    return true;
}
