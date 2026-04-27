#ifndef CAR_H
#define CAR_H

#include "main.h"
#include "gamepadParser.h"
#include "steering.h"
#include "gpioDrive.h"
#include "motor.h"

class Car{
    public:
        Car(
            UART_HandleTypeDef* huart,
            TIM_HandleTypeDef*  steeringTim,
            uint32_t            steeringChannel,
            GPIO_TypeDef*       ledFrontPort, 
            uint16_t            ledFrontPin,
            GPIO_TypeDef*       ledRearPort, 
            uint16_t            ledRearPin,
            TIM_HandleTypeDef*  motorTim,
            uint32_t            motorChannel,
            GPIO_TypeDef*       dirPort,
            uint16_t            dirPin,
            GPIO_TypeDef*       brakePort,
            uint16_t            brakePin,
            GPIO_TypeDef*       drvoffPort,
            uint16_t            drvoffPin
        );
        void init();
        void update();

        void uartRxCallback(uint16_t size);
    private:
        GamepadParser   m_gamepad;
        Steering        m_steering;
        GpioDrive       m_ledFront;
        GpioDrive       m_ledRear;
        Motor           m_motor;

        bool            m_prevX;
        bool            m_prevB;
};

#endif