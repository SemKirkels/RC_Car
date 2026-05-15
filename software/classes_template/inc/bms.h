#ifndef BMS_H
#define BMS_H

#include "i2cDevice.h"
#include "rc_config.h"

class Bms : public I2cDevice{
    public:
        Bms(I2C_HandleTypeDef* hi2c);

        void init()         override;
        void processData()  override;
        /* Get functions */
        float   getCellVoltage(uint8_t cell)    const;
        float   getTotalVoltage()               const {return m_totalVoltage;}
        float   getCurrent()                    const {return m_current;}
        uint8_t getStatus()                     const {return m_status;}
        bool    isValid()                       const {return m_valid;}
        bool    isFault()                       const {return m_status & 0x3F;}

        void enableCharging(bool enable);
        void enableDischarging(bool enable);
    private:
        float   m_cellVoltage[3];
        float   m_totalVoltage;
        float   m_current;
        uint8_t m_status;
        bool    m_valid;
        
        /* Calibration values (readout at startup) */
        uint16_t    m_gain;     /* uV/LSB */
        int8_t      m_offset;   /* mV */
        /* Registers */
        static const uint8_t REG_SYS_STAT   = 0x00;
        static const uint8_t REG_SYS_CTRL1  = 0x04;
        static const uint8_t REG_SYS_CTRL2  = 0x05;
        static const uint8_t REG_VC1_HI     = 0x0C;
        static const uint8_t REG_BAT_HI     = 0x2A;
        static const uint8_t REG_CC_HI      = 0x32;
        static const uint8_t REG_ADCGAIN1   = 0x50;
        static const uint8_t REG_ADCOFFSET  = 0x51;
        static const uint8_t REG_ADCGAIN2   = 0x59;

        float convertCellVoltage(uint16_t raw);
        float convertCurrent(uint16_t raw);
        void readGainOffset();
        void clearFaults();
};

#endif