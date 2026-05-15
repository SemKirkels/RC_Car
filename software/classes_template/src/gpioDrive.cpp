#include "gpioDrive.h"

GpioDrive::GpioDrive(GPIO_TypeDef* port, uint16_t pin):m_port(port), m_pin(pin), m_state(false){}

void GpioDrive::init(const char* taskName){
    /* Create task */
    m_semaphore = osSemaphoreNew(1, 0, NULL);
    if(m_semaphore == NULL){
        Error_Handler();
    }

    osThreadAttr_t attr = {};
    attr.name = taskName;
    attr.stack_size = 512;
    attr.priority = osPriorityNormal;

    osThreadNew(taskEntry, this, &attr);
}

void GpioDrive::setState(bool state){
    /* set state + release semaphore */
    if(state != m_state){
        m_state = state;
        osSemaphoreRelease(m_semaphore);
    }
}

void GpioDrive::toggle(){
    setState(!m_state);
}

void GpioDrive::write(bool val){
    if(val){
        set();
    }
    else{
        clear();
    }
}

void GpioDrive::taskEntry(void *argument){
    GpioDrive* self = static_cast<GpioDrive*>(argument);
    self->task();
}

/* Task loop */
void GpioDrive::task(){
    for(;;){
        osSemaphoreAcquire(m_semaphore, osWaitForever);
        write(m_state);
    }
}