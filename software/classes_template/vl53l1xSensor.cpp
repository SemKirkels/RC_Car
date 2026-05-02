#include "vl53l1xSensor.h"

Vl53l1xSensor::Vl53l1xSensor(
    I2C_HandleTypeDef*  hi2c,
    uint8_t             address,
    GPIO_TypeDef*       xshutPort,
    uint16_t            xshutPin
    ): 
    I2cDevice(hi2c, address, false),
    m_distance(0xFFFF){
        TOF_InitStruct(&m_sensor, hi2c, address, xshutPort, xshutPin);
}

void Vl53l1xSensor::init(){
    /* Code can be added */
}

void Vl53l1xSensor::processData(){
    m_distance = TOF_GetDistance(&m_sensor);
}
