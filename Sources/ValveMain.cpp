#include "ValveMain.h"
#include "Console.h"

extern UART_HandleTypeDef huart2;

void ValveMain()
{
    Console::SetPort(&huart2);

    Console::Log("Valve Start.\n");

    while (1)
    {
        Console::Log(".");
        HAL_Delay(1000);
    }
}
