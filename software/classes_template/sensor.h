#ifndef SENSOR_H
#define SENSOR_H

#include <cstdint>

enum class sensorType: uint8_t{
    // sensor types (more can be added)
    Temperature,
    Ultrasonic
};

class Sensor{
    public:
        Sensor(sensorType, uint8_t i2cAddr);    /* Constructor */
        sensorType  getType()   const;
        uint8_t     getAddr()   const;
        float       getData();

    private:
        sensorType  type_;
        uint8_t     addr_;
};

#endif