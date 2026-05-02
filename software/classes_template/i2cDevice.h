#ifndef I2CDEVICE_H
#define I2CDEVICE_H

#include "main.h"
#include "cmsis_os.h"

class I2cDevice{
    public:
        I2cDevice(I2C_HandleTypeDef* hi2c, uint8_t address, bool crcEnabled = false);
        virtual void init() = 0;
        virtual void processData() = 0;
        bool isBusy() const{return m_busy;}
        void onDmaComplete();
    protected:
        /* Read and write functions */
        HAL_StatusTypeDef writeReg(uint8_t reg, uint8_t* data, uint16_t len);
        HAL_StatusTypeDef readReg(uint8_t reg, uint8_t* data, uint16_t len);

        /* DMA functions */
        HAL_StatusTypeDef readRegDMA(uint8_t reg, uint8_t* buf, uint16_t len);
        bool waitDMA(uint32_t timeoutMs);

        I2C_HandleTypeDef*  m_hi2c;
        uint8_t             m_address;
        bool                m_crcEnabled;
        volatile bool       m_busy;
        osSemaphoreId_t     m_semaphore;
    private:
        static uint8_t calcCRC(uint8_t* data, uint16_t len);
        bool verifyCRC(uint8_t* data, uint16_t len, uint8_t receivedCRC);
};

#endif