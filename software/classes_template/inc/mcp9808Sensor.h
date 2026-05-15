#ifndef MCP9808SENSOR_H
#define MCP9808SENSOR_H

#include "i2cDevice.h"

class Mcp9808 : public I2cDevice{
    public:
        Mcp9808(I2C_HandleTypeDef* hi2c, uint8_t address);

        void    init()              override;
        void    processData()       override;

        float   getTemperature()    const {return m_temperature;}
        bool    isValid()           const {return m_valid;}
    private:
        float   m_temperature;
        bool    m_valid;
        static const uint8_t REG_TEMP   = 0x05;
        static const uint8_t REG_CONFIG = 0x01;
};

#endif
