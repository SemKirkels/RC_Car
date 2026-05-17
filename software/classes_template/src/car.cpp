#include "car.h"

Car::Car(
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
    ):
    m_gamepad(huart),
    m_steering(steeringTim, steeringChannel),
    m_ledFront(ledFrontPort, ledFrontPin),
    m_ledRear(ledRearPort, ledRearPin),
    m_motor(motorTim, motorChannel, dirPort, dirPin, brakePort, brakePin, drvoffPort, drvoffPin),
    m_prevX(false),
    m_prevB(false),
    m_distances({0xFFFF, 0xFFFF, 0xFFFF}),
    m_distanceMutex(NULL),
    m_health({}),
    m_healthMutex(NULL){
}

void Car::init(){
    m_distanceMutex = osMutexNew(NULL);
    m_healthMutex   = osMutexNew(NULL);

    m_gamepad.init();
    m_steering.init();
    m_ledFront.init("FrontLEDs");
    m_ledRear.init("RearLEDs");
    m_motor.init();
}

void Car::setDistances(const DistanceMap& distances){
    osMutexAcquire(m_distanceMutex, osWaitForever);
    m_distances = distances;
    osMutexRelease(m_distanceMutex);
}

void Car::setHealth(const HealthMap& health){
    osMutexAcquire(m_healthMutex, osWaitForever);
    m_health = health;
    osMutexRelease(m_healthMutex);
}

void Car::update(){
    /* Check safety limits */
    /* Get sensor data */
    DistanceMap dist = {0xFFFF, 0xFFFF, 0xFFFF};
    if(m_distanceMutex != NULL){
        osMutexAcquire(m_distanceMutex, osWaitForever);
        dist = m_distances;
        osMutexRelease(m_distanceMutex);
    }
    HealthMap health = {};
    if(m_healthMutex != NULL){
        osMutexAcquire(m_healthMutex, osWaitForever);
        health = m_health;
        osMutexRelease(m_healthMutex);
    }
    /* Block if needed */
    bool blockForward  = dist.front < STOP_DISTANCE_MM;
    bool blockLeft     = dist.left  < STEER_BLOCK_LEFT_MM;
    bool blockRight    = dist.right < STEER_BLOCK_RIGHT_MM;
    bool blockMotor    = health.motorTempHot > MOTOR_TEMP_LIM;


    if(osSemaphoreAcquire(m_gamepad.getSemaphore(), 100) == osOK){
        GamepadState state = m_gamepad.getState();

        /* Update Steering */
        short thumbLX = state.thumbLX;
        if(blockLeft && thumbLX < 0){
            thumbLX = 0;
        }
        if(blockRight && thumbLX > 0){
            thumbLX = 0;
        }
        m_steering.setFromJoystick(thumbLX);

        /* Update Motor */
        if(blockForward || blockMotor){
            m_motor.stop();
        }
        else{
            m_motor.setFromTriggers(state.leftTrigger, state.rightTrigger);
        }

        /* Update Lights */
        bool currX = (state.buttons & CTRL_X) != 0;
        bool currB = (state.buttons & CTRL_B) != 0;
        if(currX && !m_prevX) m_ledFront.toggle();
        if(currB && !m_prevB) m_ledRear.toggle();
        m_prevX = currX;
        m_prevB = currB;
    }
/*
    if(m_distanceMutex != NULL){
        osMutexAcquire(m_distanceMutex, osWaitForever);
        DistanceMap dist = m_distances;
        osMutexRelease(m_distanceMutex);

        char buf[64];
        snprintf(buf, sizeof(buf), "L:%u F:%u R:%u\r\n", dist.left, dist.front, dist.right);
        HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
    }
/*
    if(m_healthMutex != NULL){
        osMutexAcquire(m_healthMutex, osWaitForever);
        HealthMap health = m_health;
        osMutexRelease(m_healthMutex);
        
        char buf[128];
        snprintf(buf, sizeof(buf), "MT:%.1f AT:%.1f V0:%.2f V1:%.2f V2:%.2f VT:%.2f I:%.2f\r\n",
        health.motorTempHot,
        health.ambientTemp,
        health.cellVoltage[0],
        health.cellVoltage[1],
        health.cellVoltage[2],
        health.totalVoltage,
        health.current);
        HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
    }
*/
    uint32_t now = HAL_GetTick();
    if(now - m_lastTelemetry >= 500){
        m_lastTelemetry = now;
        sendTelemetry();
    }
}

void Car::uartRxCallback(uint16_t size){
    m_gamepad.rxEventCallback(size);
}

void Car::sendTelemetry(){
    DistanceMap dist = {0xFFFF, 0xFFFF, 0xFFFF};
    if(m_distanceMutex != NULL){
        osMutexAcquire(m_distanceMutex, osWaitForever);
        dist = m_distances;
        osMutexRelease(m_distanceMutex);
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "T:F:%u,L:%u,R:%u\n",dist.front, dist.left, dist.right);
    HAL_UART_Transmit(&huart4, (uint8_t*)buf, strlen(buf), 100);
}