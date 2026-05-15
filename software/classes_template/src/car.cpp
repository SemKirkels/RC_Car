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
    m_prevB(false){
}

void Car::init(){
    m_distanceMutex = osMutexNew(NULL);
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

void Car::update(){
    if(osSemaphoreAcquire(m_gamepad.getSemaphore(), 100) == osOK){
        GamepadState state = m_gamepad.getState();

        /* Update Steering */
        m_steering.setFromJoystick(state.thumbLX);

        /* Update Motor */
        m_motor.setFromTriggers(state.leftTrigger, state.rightTrigger);

        /* Update Lights */
        bool currX = (state.buttons & CTRL_X) != 0;
        bool currB = (state.buttons & CTRL_B) != 0;
        if(currX && !m_prevX) m_ledFront.toggle();
        if(currB && !m_prevB) m_ledRear.toggle();
        m_prevX = currX;
        m_prevB = currB;
    }
    
    if(m_distanceMutex != NULL){
        osMutexAcquire(m_distanceMutex, osWaitForever);
        DistanceMap dist = m_distances;
        osMutexRelease(m_distanceMutex);

        char buf[64];
        snprintf(buf, sizeof(buf), "L:%u F:%u R:%u\r\n",
            dist.left, dist.front, dist.right);
        HAL_UART_Transmit(&huart1, (uint8_t*)buf, strlen(buf), 100);
    }
}

void Car::uartRxCallback(uint16_t size){
    m_gamepad.rxEventCallback(size);
}