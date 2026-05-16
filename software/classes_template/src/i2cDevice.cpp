#include "i2cDevice.h"

osMutexId_t I2cDevice::s_busMutex     = NULL;
bool        I2cDevice::s_busMutexInit = false;
I2cDevice*  I2cDevice::s_activeDevice = NULL;

I2cDevice::I2cDevice(I2C_HandleTypeDef* hi2c, uint8_t address, bool crcEnabled):
    m_hi2c(hi2c),
    m_address(address),
    m_crcEnabled(crcEnabled),
    m_busy(false),
    m_semaphore(NULL){
    m_semaphore = osSemaphoreNew(1, 0, NULL);

    if(!s_busMutexInit){
        s_busMutex      = osMutexNew(NULL);
        s_busMutexInit  = true;
    }
}

HAL_StatusTypeDef I2cDevice::writeReg(uint8_t reg, uint8_t* data, uint16_t len){
    uint8_t buf[len + 2];
    buf[0] = reg;
    for(uint16_t i = 0; i < len; i++){
        buf[i + 1] = data[i];
    }

    uint16_t txLen = len + 1;

    if(m_crcEnabled){
        buf[txLen] = calcCRC(buf, txLen);
        txLen++;
    }

    return HAL_I2C_Master_Transmit(m_hi2c, m_address << 1, buf, txLen, 100);
}

HAL_StatusTypeDef I2cDevice::readReg(uint8_t reg, uint8_t* data, uint16_t len){
    HAL_StatusTypeDef status;

    status = HAL_I2C_Master_Transmit(m_hi2c, m_address << 1, &reg, 1, 100);
    if(status != HAL_OK){
        return status;
    }

    if(m_crcEnabled){
        uint8_t buf[len + 1];
        status = HAL_I2C_Master_Receive(m_hi2c, m_address << 1, buf, len + 1, 100);
        if(status != HAL_OK){
            return status;
        }
        
        if(!verifyCRC(buf, len, buf[len])){
            return HAL_ERROR;
        }

        for(uint16_t i = 0; i < len; i++){
            data[i] = buf[i];
        }
    }
    else{
        status = HAL_I2C_Master_Receive(m_hi2c, m_address << 1, data, len, 100);
    }

    return status;
}

HAL_StatusTypeDef I2cDevice::readRegDMA(uint8_t reg, uint8_t* buf, uint16_t len){
    osMutexAcquire(s_busMutex, osWaitForever);

    HAL_StatusTypeDef status;
    status = HAL_I2C_Master_Transmit(m_hi2c, m_address << 1, &reg, 1, 100);
    if(status != HAL_OK){
        return status;
    }

    m_busy          = true;
    s_activeDevice  = this;

    uint16_t rxLen;
    if(m_crcEnabled){
        rxLen = len + 1;
    }
    else{
        rxLen = len;
    }

    status = HAL_I2C_Master_Receive_DMA(m_hi2c, m_address << 1, buf, rxLen);
    if(status != HAL_OK){
        m_busy         = false;
        s_activeDevice = NULL;
        osMutexRelease(s_busMutex);
    }

    return status; 
}

bool I2cDevice::waitDMA(uint32_t timeoutMs){
    bool ok =  osSemaphoreAcquire(m_semaphore, timeoutMs) == osOK;
    osMutexRelease(s_busMutex);
    return ok;
}

void I2cDevice::onDmaComplete(){
    m_busy = false;
    osSemaphoreRelease(m_semaphore);
}

uint8_t I2cDevice::calcCRC(uint8_t* data, uint16_t len){
    uint8_t crc = 0x00;
    for(uint16_t i = 0; i < len; i++){
        crc ^= data[i];
        for(uint8_t bit = 0; bit < 8; bit++){
            if(crc & 0x80){
                crc = (crc << 1) ^ 0x07;
            }
            else{
                crc <<= 1;
            }
        }
    }

    return crc;
}

bool I2cDevice::verifyCRC(uint8_t* data, uint16_t len, uint8_t receivedCRC){
    return calcCRC(data, len) == receivedCRC;
}