#include "ValveMain.h"
#include "Console.h"
#include "RotaryEncoder.h"
#include "main.h"

extern UART_HandleTypeDef huart2;

namespace {

constexpr int RotaryEncoderCount = 4;

RotaryEncoder g_RotaryEncoder[RotaryEncoderCount];

}

void ValveMain()
{
    Console::SetPort(&huart2);
    Console::Log("Valve Start.\n");

    RotaryEncoder& rotaryEncoder = g_RotaryEncoder[0];

    PortPin a(RotaryEncoder1A_GPIO_Port, RotaryEncoder1A_Pin);
    PortPin b(RotaryEncoder1B_GPIO_Port, RotaryEncoder1B_Pin);
    rotaryEncoder.SetPortPin(&a, &b);
    rotaryEncoder.Sample();

    int samplingCount = 0;

    while (1)
    {
        rotaryEncoder.Sample();
        if (samplingCount % 16 == 0) {
            Console::Log("%d %d %d %d\n",
                samplingCount,
                rotaryEncoder.IsUpdated(),
                rotaryEncoder.GetDifference(),
                rotaryEncoder.GetTotalPosition());
            rotaryEncoder.Commit();
        }

        samplingCount++;
        HAL_Delay(1);
    }
}
