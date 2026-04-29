#include "steering.h"
 
Steering::Steering(TIM_HandleTypeDef* htim, uint32_t channel):m_htim(htim), m_channel(channel){}
 
void Steering::init(){
    HAL_TIM_PWM_Start(m_htim, m_channel);
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, SERVO_MID);
}
 
void Steering::setFromJoystick(short thumbLX){
    /* update PWM */
    __HAL_TIM_SET_COMPARE(m_htim, m_channel, mapToCCR(thumbLX));
}
 
 
uint32_t Steering::mapToCCR(short thumbLX){
    /* Deadzone: -1500 to 1500*/
    if(thumbLX > -1500 && thumbLX < 1500){
        return SERVO_MID;
    }

    /* thumbLX: -32768 (left) to 32767 (right) */
    if(thumbLX >= 1500){
        int32_t val = thumbLX - 1500; 
        return SERVO_MID + (uint32_t)(val * (SERVO_MAX - SERVO_MID) / 32767);
    }
    else{
        int32_t val = thumbLX + 1500; 
        return SERVO_MID - (uint32_t)(-val * (SERVO_MID - SERVO_MIN) / 32768);
    }
}
 