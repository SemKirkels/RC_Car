#ifndef GPIODRIVE_H
#define GPIODRIVE_H

class GpioDrive{
    public:
        GpioDrive(GPIO_TypeDef* port, uint16_t pin):m_port(port), m_pin(pin){}
        void write(bool val){
            if(val){
                set();
            }
            else{
                clear();
            }
        }
    private:
        GPIO_TypeDef* const m_port;
        const uint16_t m_pin;
        void set(){ HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_SET); }
        void clear(){ HAL_GPIO_WritePin(m_port, m_pin, GPIO_PIN_RESET); }
};

#endif