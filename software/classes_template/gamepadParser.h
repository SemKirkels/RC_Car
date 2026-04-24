#ifndef GAMEPADPARSER_H
#define GAMEPADPARSER_H

#include "main.h"
#include "cmsis_os.h"
#include <string.h>

struct GamepadState{
    bool  connected;
    unsigned short buttons;
    unsigned char leftTrigger;
    unsigned char rightTrigger;
    short thumbLX;
    short thumbLY;
    short thumbRX;
    short thumbRY;
};

class GamepadParser{
    public:
        GamepadParser(UART_HandleTypeDef* huart);
        void init();
        void rxEventCallback(uint16_t size); /* Call from HAL_UARTEx_Rx_EventCallback */
        GamepadState getState();
        osSemaphoreId_t getSemaphore();
    private:
        UART_HandleTypeDef* m_huart;
        osSemaphoreId_t m_semaphore;
        GamepadState m_state;

        static const int MAX_FIELDS     = 10;
        static const int MAX_LEN        = 16;
        static const int DMA_BUF_LEN    = 64;

        char m_fields[MAX_FIELDS][MAX_LEN];
        int m_fieldIndex;
        int m_charIndex;
        bool m_active;

        uint8_t m_dmaBuffer[DMA_BUF_LEN];

        void startDmaReception();
        void parseChar(char c);
        void buildState();

        static int simpleAtoi(const char* s);
};

#endif