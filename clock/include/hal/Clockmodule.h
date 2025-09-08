#pragma once
#include <RTClib.h>

class Clockmodule
{
    public:
        Clockmodule();
        void begin();
        DateTime getTime();
        bool lostPower();      // kiểm tra RTC có mất nguồn không
        bool syncWithNTP();    // cập nhật từ NTP nếu cần
    private:
        RTC_DS3231 rtc;

};