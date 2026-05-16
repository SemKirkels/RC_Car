#include "mcp9600Sensor.h"

Mcp9600::Mcp9600(I2C_HandleTypeDef* hi2c, uint8_t address, ProbeType type):
    I2cDevice(hi2c, address, false),
    m_type(type),
    m_hotJunc(0.0f),
    m_coldJunc(0.0f),
    m_valid(false){
}

void Mcp9600::init(){
    /* probe type: bit 6 to 4 -> bitshift*/
    uint8_t sensorConfig = (uint8_t)m_type << 4;
    writeReg(REG_SENSOR_CONFIG, &sensorConfig, 1);

    /* Use default mode */
    uint8_t deviceConfig = 0x00;
    writeReg(REG_DEVICE_CONFIG, &deviceConfig, 1);
}

void Mcp9600::processData(){
    /* Get hot junction */
    if(readRegDMA(REG_HOT_JUNCTION, m_dmaBuf, 2) != HAL_OK){
        m_valid = false;
        return;
    }
    if(!waitDMA(100)){
        m_valid = false;
        return;
    }
    m_hotJunc = convertTemp(m_dmaBuf);

    /* Get cold junction */
    if(readRegDMA(REG_HOT_JUNCTION, m_dmaBuf, 2) != HAL_OK){
        m_valid = false;
        return;
    }
    if(!waitDMA(100)){
        m_valid = false;
        return;
    }
    m_coldJunc = convertTemp(m_dmaBuf);

    m_valid = true;
}

float Mcp9600::convertTemp(uint8_t* buf){
    /* two complement */
    int16_t raw = (int16_t)((buf[0] << 8) | buf[8]);

    return raw / 16.0f;
}