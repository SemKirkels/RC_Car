#ifndef CAR_H
#define CAR_H

#include "motor.h"
#include "steering.h"
#include "battery.h"
#include "address.h"

class Car{
    public:
        Car();

    private:
        /* Sensors */
        Sensor      distFront;
        Sensor      distLeft;
        Sensor      distRight;
        Sensor      temp;

        /* Actuators */
//        Motor       motor;
//        Steering    steering;

        /* Battery */
//        Battery     battery;
};

#endif