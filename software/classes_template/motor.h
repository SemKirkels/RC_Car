#ifndef MOTOR_H
#define MOTOR_H

#include "main.h"
#include "rc_config.h"

#define DEADZONE 1

class Motor{
    public:
        Motor(
            TIM_HandleTypeDef*  htim,
            uint32_t            channel,
            GPIO_TypeDef*       dirPort,
            uint16_t            dirPin,
            GPIO_TypeDef*       brakePort,
            uint16_t            brakePin,
            GPIO_TypeDef*       drvoffPort,
            uint16_t            drvoffPin
        );
        void init();
        void setFromTriggers(uint8_t leftTrigger, uint8_t rightTrigger);
        void stop();
        void enable();
        void disable();
    private:
        TIM_HandleTypeDef*  m_htim;
        uint32_t            m_channel;
        GPIO_TypeDef*       m_dirPort;
        uint16_t            m_dirPin;
        GPIO_TypeDef*       m_brakePort;
        uint16_t            m_brakePin;
        GPIO_TypeDef*       m_drvoffPort;
        uint16_t            m_drvoffPin;

        void setSpeed(uint8_t trigger, bool forward);
        uint32_t mapToCCR(uint8_t trigger);
};

#endif