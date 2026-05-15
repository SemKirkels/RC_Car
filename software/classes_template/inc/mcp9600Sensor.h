#ifndef MCP9600_H
#define MCP9600_H

#include "i2cDevice.h"

class Mcp9600 : public I2cDevice{
    public:
        /* Define Probe type */
        enum class ProbeType : uint8_t{
            K = 0x00,
            J = 0x01,
            T = 0x02,
            N = 0x03,
            S = 0x04,
            E = 0x05,
            B = 0x06,
            R = 0x07
        };
        Mcp9600(I2C_HandleTypeDef* hi2c, uint8_t address, ProbeType type = ProbeType::K);
        void init()         override;
        void processData()  override;
        float getHotJunc()  const{return m_hotJunc;}
        float getColdJunc() const{return m_coldJunc;}
        bool isValid()      const{return m_valid;}
    private:
        ProbeType   m_type;
        float       m_hotJunc;
        float       m_coldJunc;
        bool        m_valid;

        static const uint8_t REG_HOT_JUNCTION   = 0x00;
        static const uint8_t REG_COLD_JUNCTION  = 0x02;
        static const uint8_t REG_SENSOR_CONFIG  = 0x05;
        static const uint8_t REG_DEVICE_CONFIG  = 0x06;
        
        float convertTemp(uint8_t* buf);
};

#endif