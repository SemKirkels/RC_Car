#ifndef STEERING_H
#define STEERING_H

#include "main.h"
#include "rc_config.h"

class Steering{
    public:
        Steering(TIM_HandleTypeDef* htim, uint32_t channel);
        void init();
        void setFromJoystick(short thumbLX);
    private:
        TIM_HandleTypeDef*  m_htim;
        uint32_t            m_channel;

        static const uint32_t SERVO_MIN = 750;
        static const uint32_t SERVO_MID = 1500;
        static const uint32_t SERVO_MAX = 2250;
    
        uint32_t mapToCCR(short thumbLX);
};

#endif