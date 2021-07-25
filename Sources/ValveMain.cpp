#include "ValveMain.h"
#include "Console.h"
#include "RotaryEncoder.h"
#include "main.h"

extern UART_HandleTypeDef huart2;

namespace {

constexpr int RotaryEncoderCount = 4;

RotaryEncoder g_RotaryEncoder[RotaryEncoderCount];

struct RotaryEncoderPortPin
{
    PortPin PhaseA;
    PortPin PhaseB;
};

RotaryEncoderPortPin RotaryEncoderPortPinSettings[] = 
{
    // ch1
    {
        PortPin(RotaryEncoder1A_GPIO_Port, RotaryEncoder1A_Pin),
        PortPin(RotaryEncoder1B_GPIO_Port, RotaryEncoder1B_Pin),
    },
    // ch2
    {
        PortPin(RotaryEncoder2A_GPIO_Port, RotaryEncoder2A_Pin),
        PortPin(RotaryEncoder2B_GPIO_Port, RotaryEncoder2B_Pin),
    },
    // ch3
    {
        PortPin(RotaryEncoder3A_GPIO_Port, RotaryEncoder3A_Pin),
        PortPin(RotaryEncoder3B_GPIO_Port, RotaryEncoder3B_Pin),
    },
    // ch4
    {
        PortPin(RotaryEncoder4A_GPIO_Port, RotaryEncoder4A_Pin),
        PortPin(RotaryEncoder4B_GPIO_Port, RotaryEncoder4B_Pin),
    }
};

}

void ValveMain()
{
    Console::SetPort(&huart2);
    Console::Log("Valve Start.\n");

    for (int i = 0; i < RotaryEncoderCount; i++) {
        auto setting = &RotaryEncoderPortPinSettings[i];
        g_RotaryEncoder[i].SetPortPin(&setting->PhaseA, &setting->PhaseB);
    }

    RotaryEncoder& rotaryEncoder = g_RotaryEncoder[0];
    rotaryEncoder.Sample();

    int samplingCount = 0;

    while (1)
    {
        rotaryEncoder.Sample();
        if (samplingCount % 16 == 0) {
            Console::Log("%d %d %d %d %d\n",
                samplingCount,
                rotaryEncoder.IsUpdated(),
                rotaryEncoder.GetDifference(),
                rotaryEncoder.GetTotalPosition(),
                rotaryEncoder.GetTotalErrorCount());
            rotaryEncoder.Commit();
        }

        samplingCount++;
        HAL_Delay(1);
    }
}
