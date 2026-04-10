#ifndef GPIODRIVE_H
#define GPIODRIVE_H

#include "cmsis_os.h"
#include "main.h"

class GpioDrive{
    public:
        GpioDrive(GPIO_TypeDef* port, uint16_t pin);
        void init(const char* taskName);
        void setState(bool state);
    private:
        GPIO_TypeDef* const m_port;
        const uint16_t m_pin;
        osSemaphoreId_t m_semaphore;
        bool m_state;

        static void taskEntry(void *argument);
        void task();

        void write(bool val);
        void set(){ HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET); }
        void clear(){ HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET); }
};

#endif