#include "vl53l0xSensor.h"

Vl53l0xSensor::Vl53l0xSensor(
    I2C_HandleTypeDef* hi2c, 
    uint8_t address,
    GPIO_TypeDef* xshutPort,
    uint16_t xshutPin
    ):
    I2cDevice(hi2c, address, false),
    m_xshutPort(xshutPort),
    m_xshutPin(xshutPin),
    m_newAddress(address),
    m_distance(65535){
        m_dev.address       = TOF_DEFAULT_ADDR << 1;
        m_dev.io_2v8        = true;
        m_dev.io_timeout    = 500;
        m_dev.did_timeout   = false;
}

void Vl53l0xSensor::init(){
    m_dev.address = TOF_DEFAULT_ADDR << 1;
    VL53L0X_init(&m_dev);
    VL53L0X_setAddress(&m_dev, m_newAddress << 1);
    VL53L0X_startContinuous(&m_dev, TOF_READOUT_DELAY);
}

void Vl53l0xSensor::processData(){
    m_distance = VL53L0X_readRangeContinuousMillimeters(&m_dev);
}

void Vl53l0xSensor::enable(){
    HAL_GPIO_WritePin(m_xshutPort, m_xshutPin, GPIO_PIN_SET);
    osDelay(2);
}

void Vl53l0xSensor::disable(){
    HAL_GPIO_WritePin(m_xshutPort, m_xshutPin, GPIO_PIN_RESET);
}

