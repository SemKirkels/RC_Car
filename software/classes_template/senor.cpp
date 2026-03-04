#include "sensor.h"

Sensor::Sensor(sensorType type, uint8_t i2cAddr) : type_(type), addr_(i2cAddr){
}

sensorType Sensor::getType() const{
    return type_;
}

uint8_t Sensor::getAddr() const{
    return addr_;
}

float Sensor::getData(){
    // add i2c communication
    return 0.0f;
}