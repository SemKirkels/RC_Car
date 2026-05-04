/* Required library https://github.com/aminejouini/VL53L0X-STM32H7 */

#ifndef VL53L0XSENSOR_H
#define VL53L0XSENSOR_H

#include "i2cDevice.h"
#include "rc_config.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "VL53L0X.h"
#ifdef __cplusplus
}
#endif

class Vl53l0xSensor : public I2cDevice{
    public:
        Vl53l0xSensor(
            I2C_HandleTypeDef*  hi2c,
            uint8_t             address,
            GPIO_TypeDef*       xshutPort,
            uint16_t            xshutPin);
        void init()         override;
        void processData()  override;

        void enable();
        void disable();
        uint16_t    getDistance()   const {return m_distance;}
        bool        isValid()       const {return m_distance != 65535;}
    private:
        VL53L0X         m_dev;
        GPIO_TypeDef*   m_xshutPort;
        uint16_t        m_xshutPin;
        uint8_t         m_newAddress;
        uint16_t        m_distance;
};

#endif