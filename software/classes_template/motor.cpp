#include "motor.h"

Motor::Motor(
        TIM_HandleTypeDef*  htim,
        uint32_t            channel,
        GPIO_TypeDef*       dirPort,
        uint16_t            dirPin,
        GPIO_TypeDef*       brakePort,
        uint16_t            brakePin,
        GPIO_TypeDef*       drvoffPort,
        uint16_t            drvoffPin
    ):
    m_htim(htim),
    m_channel(channel),
    m_dirPort(dirPort),
    m_dirPin(dirPin),
    m_brakePort(brakePort),
    m_brakePin(brakePin),
    m_drvoffPort(drvoffPort),
    m_drvoffPin(drvoffPin){
    }

void Motor::init(){
    HAL_TIM_PWM_Start(m_htim, m_channel);
    enable();
    stop();
}

void Motor::enable(){
    /* drvOff low */
    HAL_GPIO_WritePin(m_drvoffPort, m_drvoffPin, GPIO_PIN_RESET);
    /* brake low */
    HAL_GPIO_WritePin(m_brakePort, m_brakePin, GPIO_PIN_RESET);
}

void Motor::disable(){
    stop();
    /* drvOff high */
    HAL_GPIO_WritePin(m_drvoffPort, m_drvoffPin, GPIO_PIN_SET);
}

void Motor::stop(){
    /* PWM to 0 */
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, 0);
    /* Enable brake */
    HAL_GPIO_WritePin(m_brakePort, m_brakePin, GPIO_PIN_SET);
}

void Motor::setFromTriggers(uint8_t leftTrigger, uint8_t rightTrigger){
    if(rightTrigger > DEADZONE){
        setSpeed(rightTrigger, true);
    }

    if(leftTrigger > DEADZONE){
        setSpeed(leftTrigger, false);
    }
}

void Motor::setSpeed(uint8_t trigger, bool forward){
    /* brake low */
    HAL_GPIO_WritePin(m_brakePort, m_brakePin, GPIO_PIN_RESET);

    if(forward){
        /* dir high */
        HAL_GPIO_WritePin(m_dirPort, m_dirPin, GPIO_PIN_SET);
    }
    else{
        /* dir low */
        HAL_GPIO_WritePin(m_dirPort, m_dirPin, GPIO_PIN_RESET);
    }

    __HAL_TIM_SET_COMPARE(m_htim, m_channel, mapToCCR(trigger));
}

uint32_t Motor::mapToCCR(uint8_t trigger){
    /* scale from 0 to 255 */
    return (uint32_t)trigger * MOTOR_PWM_PERIOD / 255;
}