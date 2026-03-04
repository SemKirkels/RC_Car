#include "car.h"

Car::Car() : 
    distFront(sensorType::Ultrasonic, DIST_FRONT_ADDR),
    distLeft(sensorType::Ultrasonic, DIST_LEFT_ADDR),
    distRight(sensorType::Ultrasonic, DIST_RIGHT_ADDR),
    temp(sensorType::Temperature, TEMP_SENSOR_ADDR){
}