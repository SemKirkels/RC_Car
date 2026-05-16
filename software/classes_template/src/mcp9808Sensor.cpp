#include "mcp9808Sensor.h"

Mcp9808::Mcp9808(I2C_HandleTypeDef* hi2c, uint8_t address):
    I2cDevice(hi2c, address, false),
    m_temperature(0.0f),
    m_valid(false){
}

void Mcp9808::init(){
    /* Continuous conversion mode, default after power-on */
    /* Config register 0x0000 = continuous, no shutdown */
    uint8_t config[2] = {0x00, 0x00};
    writeReg(REG_CONFIG, config, 2);
}

void Mcp9808::processData(){
    if(readRegDMA(REG_TEMP, m_dmaBuf, 2) != HAL_OK){
        m_valid = false;
        return;
    }

    if(!waitDMA(100)){
        m_valid = false;
        return;
    }

    /* Mask alert flags */
    uint8_t upperByte = m_dmaBuf[0] & 0x1F;
    uint8_t lowerByte = m_dmaBuf[1];

    /* Check sign bit & convert if needed */
    if(upperByte & 0x10){
        upperByte &= 0x0F;
        m_temperature = 256.0f - (upperByte * 16.0f + lowerByte / 16.0f);
    }
    else{
        m_temperature = upperByte * 16.0f + lowerByte / 16.0f;
    }

    m_valid = true;
}