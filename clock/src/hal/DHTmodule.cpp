#include "hal/DHTmodule.h"

DHTmodule:: DHTmodule(uint8_t pin, uint8_t type) : dht(pin, type){}

void DHTmodule::begin(){
    dht.begin();
}

int DHTmodule::getTemperature(){
    return (int)dht.readTemperature();
}

int DHTmodule::getHumidity(){
    return (int)dht.readHumidity();
}   
