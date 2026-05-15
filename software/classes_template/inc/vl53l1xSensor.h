/* Required library https://gitlab.com/KevinAljas/vl53l1x-stm32-hal-driver */

#ifndef VL53L1X_SENSOR_H
#define VL53L1X_SENSOR_H

#include "i2cDevice.h"
#ifdef __cplusplus
extern "C" {
#endif
#include "VL53L1X.h"
#ifdef __cplusplus
}
#endif

class Vl53l1xSensor : public I2cDevice{
    public:
        Vl53l1xSensor(
            I2C_HandleTypeDef*  hi2c,
            uint8_t             address,
            GPIO_TypeDef*       xshutPort,
            uint16_t            xshutPin);
        
        void init()         override;
        void processData()  override;

        uint16_t    getDistance()   const   {return m_distance;}
        bool        isValid()       const   {return m_distance != 0xFFFF;}
        VL53L1X&    getSensor()             {return m_sensor;}
    private:
        VL53L1X     m_sensor;
        uint16_t    m_distance;
};

#endif