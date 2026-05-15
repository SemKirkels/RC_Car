#include "gamepadParser.h"

/* External var, no mem alloc */
extern UART_HandleTypeDef huart1;

GamepadParser::GamepadParser(UART_HandleTypeDef* huart): 
    m_huart(huart), 
    m_semaphore(NULL),
    m_fieldIndex(0), 
    m_charIndex(0), 
    m_active(false)
    {
        m_state.connected       = false;
        m_state.buttons         = 0;
        m_state.leftTrigger     = 0;
        m_state.rightTrigger    = 0;
        m_state.thumbLX         = 0;
        m_state.thumbLY         = 0;
        m_state.thumbRX         = 0;
        m_state.thumbRY         = 0;

        memset(m_fields, 0, sizeof(m_fields));
    }

void GamepadParser::init(){
    m_semaphore = osSemaphoreNew(1, 0, NULL);
    if(m_semaphore == NULL){
        Error_Handler();
    }

    startDmaReception();
}

void GamepadParser::startDmaReception(){
    if(HAL_UARTEx_ReceiveToIdle_DMA(m_huart, m_dmaBuffer, DMA_BUF_LEN) != HAL_OK){
        HAL_UART_Abort(m_huart);
        HAL_UARTEx_ReceiveToIdle_DMA(m_huart, m_dmaBuffer, DMA_BUF_LEN);
    }

    __HAL_DMA_DISABLE_IT(m_huart->hdmarx, DMA_IT_HT);
}

osSemaphoreId_t GamepadParser::getSemaphore(){
    return m_semaphore;
}

GamepadState GamepadParser::getState(){
    return m_state;
}

void GamepadParser::rxEventCallback(uint16_t size){
    for(uint16_t i = 0; i < size; i++){
        parseChar((char)m_dmaBuffer[i]);
    }
    startDmaReception(); /* Restart DMA for next data */
}

void GamepadParser::parseChar(char c){
    if(c == '>'){
        if(m_active){
            if(m_fieldIndex < MAX_FIELDS){    
                m_fields[m_fieldIndex][m_charIndex] = '\0';
                m_fieldIndex++;
            }
        }
        m_charIndex = 0;
        m_active = true;
    }
    else if(c == '<'){
        if(m_active){
            if(m_fieldIndex < MAX_FIELDS){
                m_fields[m_fieldIndex][m_charIndex] = '\0';
                m_fieldIndex++;
            }

            if(m_fieldIndex >= 8){
                buildState();
                osSemaphoreRelease(m_semaphore);
            }
        }

        m_fieldIndex = 0;
        m_charIndex = 0;
        m_active = false;
    }
    else if(m_active){
        if(m_fieldIndex < MAX_FIELDS && m_charIndex < (MAX_LEN - 1)){
            m_fields[m_fieldIndex][m_charIndex++] = c;
        }
    }
}

void GamepadParser::buildState(){
    /* Recv order: */
    /* connected, buttons, lt, rt, lx, ly, rx, ry */
    m_state.connected    = simpleAtoi(m_fields[0]) != 0;
    m_state.buttons      = (unsigned short)simpleAtoi(m_fields[1]);
    m_state.leftTrigger  = (unsigned short)simpleAtoi(m_fields[2]);
    m_state.rightTrigger = (unsigned short)simpleAtoi(m_fields[3]);
    m_state.thumbLX      = (short)simpleAtoi(m_fields[4]);
    m_state.thumbLY      = (short)simpleAtoi(m_fields[5]);
    m_state.thumbRX      = (short)simpleAtoi(m_fields[6]);
    m_state.thumbRY      = (short)simpleAtoi(m_fields[7]);
}

int GamepadParser::simpleAtoi(const char* s){
    int rslt = 0;
    bool neg = false;

    if(*s == '-'){
        neg = true;
        s++;
    }

    while(*s >= '0' && *s <= '9'){
        rslt = rslt * 10 + (*s - '0');
        s++;
    }

    if(neg){
        return -rslt;
    }
    else{
        return rslt;
    }
}
