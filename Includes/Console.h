#ifndef CONSOLE_H
#define CONSOLE_H

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_uart.h"

namespace Console {

// UART ポート設定
void SetPort(UART_HandleTypeDef *pUartHandle);

// UART にログを出力する (printf 相当)
void Log(const char *format, ...);

// UART 受信開始
void StartReceive();

}

#endif // CONSOLE_H
