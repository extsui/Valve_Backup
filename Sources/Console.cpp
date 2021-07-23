#include "main.h"
#include "Console.h"

#include "stm32l0xx_hal_dma.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

class ConsoleImpl
{
public:
    static const int RECEIVE_BUFFER_SIZE = 64;
    static const int SEND_BUFFER_SIZE = 64;

    ConsoleImpl()
     : m_pUartHandle(nullptr)
     , m_readPointer(0)
    {
    }

    ~ConsoleImpl()
    {
    }

    void SetPort(UART_HandleTypeDef *pUartHandle);
    void Log(const char *fmt, ...);
    void StartReceive();
    bool IsAvailable();
    uint8_t GetAvailableSize();
    uint8_t GetReceivedByte();

private:
    uint8_t m_receiveBuffer[RECEIVE_BUFFER_SIZE];
    uint8_t m_sendBuffer[SEND_BUFFER_SIZE];

    UART_HandleTypeDef *m_pUartHandle;
    uint16_t m_readPointer;
};

void ConsoleImpl::SetPort(UART_HandleTypeDef *pUartHandle)
{
    m_pUartHandle = pUartHandle;
}

void ConsoleImpl::Log(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vsprintf((char*)m_sendBuffer, fmt, ap);
    va_end(ap);

    uint8_t *p = m_sendBuffer;
    while (*p != '\0') {
        HAL_UART_Transmit(m_pUartHandle, (uint8_t*)p, 1, 0xFFFF);
        p++;
    }
}

#define DMA_WRITE_PTR ( (RECEIVE_BUFFER_SIZE - m_pUartHandle->hdmarx->Instance->CNDTR) & (RECEIVE_BUFFER_SIZE - 1) )

void ConsoleImpl::StartReceive()
{
    HAL_UART_Receive_DMA(m_pUartHandle, m_receiveBuffer, RECEIVE_BUFFER_SIZE);
    m_readPointer = 0;
}

bool ConsoleImpl::IsAvailable()
{
    if (m_readPointer == DMA_WRITE_PTR) {
        return false;
    }
    return true;
}

uint8_t ConsoleImpl::GetAvailableSize()
{
    return ((DMA_WRITE_PTR - m_readPointer + RECEIVE_BUFFER_SIZE) & (RECEIVE_BUFFER_SIZE - 1));
}

uint8_t ConsoleImpl::GetReceivedByte()
{
    uint8_t c = 0;
    if (m_readPointer != DMA_WRITE_PTR) {
        c = m_receiveBuffer[m_readPointer++];
        m_readPointer &= (RECEIVE_BUFFER_SIZE - 1);
    }
    return c;
}

/////////////////////////////////////////////////////////////////////////////
//  Public
/////////////////////////////////////////////////////////////////////////////

namespace {
    ConsoleImpl m_Impl;
}

void Console::SetPort(UART_HandleTypeDef *pUartHandle)
{
    m_Impl.SetPort(pUartHandle);
}

void Console::Log(const char *fmt)
{
    m_Impl.Log(fmt);
}

void Console::StartReceive()
{
    m_Impl.StartReceive();
}
