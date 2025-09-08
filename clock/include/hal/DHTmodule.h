#pragma once
#include <DHT.h>

class DHTmodule
{
    public: 
        DHTmodule(uint8_t pin, uint8_t type);
        void begin();
        int getTemperature();
        int getHumidity();
    private:
        DHT dht;
};