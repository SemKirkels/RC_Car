#include "bms.h"

Bms::Bms(I2C_HandleTypeDef* hi2c):
    I2cDevice(hi2c, 0x08, true),
    m_totalVoltage(0.0f),
    m_current(0.0f),
    m_status(0),
    m_valid(false),
    m_gain(365), /* Example code: default gain 365 uV/LSB */
    m_offset(0){
    m_cellVoltage[0] = 0.0f;
    m_cellVoltage[1] = 0.0f;
    m_cellVoltage[2] = 0.0f;
}

void Bms::init(){
    /* Get calibration data */
    readGainOffset();

    /* Setup ADC */
    /* ADC enable bit */
    uint8_t ctrl1 = 0x10;
    writeReg(REG_SYS_CTRL1, &ctrl1, 1);
    
    /* charge and discharge enable */
    uint8_t ctrl2 = 0x03;
    writeReg(REG_SYS_CTRL2, &ctrl2, 1);

    /* clear current fault flags */
    clearFaults();
}

void Bms::processData(){
    uint8_t buf[2];

    /* stat reg */
    if(readReg(REG_SYS_STAT, buf, 1) != HAL_OK){
        m_valid = false;
        return;
    }
    m_status = buf[0];

    /* read cell voltages */
    for(uint8_t i = 0; i < 3; i++){
        if(readReg(REG_VC1_HI + (i * 2), buf, 2) != HAL_OK){
            m_valid = false;
            return;
        }
        uint16_t raw = (uint8_t)((buf[0] << 8) | buf[1]);
        m_cellVoltage[i] = convertCellVoltage(raw);
    }

    /* Total voltage */
    if(readReg(REG_CC_HI, buf, 2) != HAL_OK){
        m_valid = false;
        return;
    }
    uint16_t ccRaw = (uint16_t)((buf[0] << 8) | buf[1]);
    m_current = convertCurrent(ccRaw);

    m_valid = true;
}

float Bms::getCellVoltage(uint8_t cell) const{
    if(cell > 2){
        return 0.0f;
    }
    return m_cellVoltage[cell];
}

void Bms::enableCharging(bool enable){
    uint8_t buf[1];
    readReg(REG_SYS_CTRL2, buf, 1);
    if(enable){
        buf[0] |= 0x01;
    }
    else{
        buf[0] &= ~0x01;
    }
    writeReg(REG_SYS_CTRL2, buf, 1);
}

void Bms::enableDischarging(bool enable){
    uint8_t buf[1];
    readReg(REG_SYS_CTRL2, buf, 1);
    if(enable){
        buf[0] |= 0x02;
    }
    else{
        buf[0] &= ~0x02;
    }
    writeReg(REG_SYS_CTRL2, buf, 1);
}

float Bms::convertCellVoltage(uint16_t raw){
    /* Datasheet: V = raw * gain + offset */
    /* gain: uV / LSB -> Divide 1000000 */
    /* offset: mV -> Divide 1000 */
    return (raw * (float)m_gain / 1000000.0f) + (m_offset / 1000.0f);
}

float Bms::convertCurrent(uint16_t raw){
    /* coulomb counter LSB = 8.44 uV / Rsense */
    int16_t signed_raw = (int16_t)raw;
    return signed_raw * (8.44e-6f / BMS_SENSE_RESISTOR_OHM);
}

void Bms::readGainOffset(){
    uint8_t buf[1];

    /* Gain is split in to two regs */
    if(readReg(REG_ADCGAIN1, buf, 1) == HAL_OK){
        uint8_t gain1 = buf[0];
        if(readReg(REG_ADCGAIN2, buf, 1) == HAL_OK){
            uint8_t gain2 = buf[0];
            /* datasheet for split (p30) */
            /* WIP */
            m_gain = 365 + (((gain1 & 0x0c) >> 1) | ((gain2 & 0xE0) >> 5));
        }
    }

    if(readReg(REG_ADCOFFSET, buf, 1) == HAL_OK){
        m_offset = (int8_t)buf[0];
    }
}

void Bms::clearFaults(){
    /* clear stat reg */
    uint8_t stat = 0xFF;
    writeReg(REG_SYS_STAT, &stat, 1);
}