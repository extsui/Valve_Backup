#include "ValveMain.h"
#include "Console.h"
#include "RotaryEncoder.h"
#include "main.h"

#include "stm32l0xx_hal_tim.h"

extern UART_HandleTypeDef huart2;

extern TIM_HandleTypeDef htim2;

namespace {

constexpr int RotaryEncoderCount = 4;

RotaryEncoder g_RotaryEncoder[RotaryEncoderCount];

int g_SamplingCount = 0;

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

extern "C" void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    for (auto& rotaryEncoder : g_RotaryEncoder) {
        rotaryEncoder.Sample();
    }
    g_SamplingCount++;
}

void ValveMain()
{
    Console::SetPort(&huart2);
    Console::Log("Valve Start.\n");

    for (int i = 0; i < RotaryEncoderCount; i++) {
        auto setting = &RotaryEncoderPortPinSettings[i];
        g_RotaryEncoder[i].SetPortPin(&setting->PhaseA, &setting->PhaseB);
        g_RotaryEncoder[i].Sample();
    }

    HAL_TIM_Base_Start_IT(&htim2);

    while (1) {
        if (g_SamplingCount % 50 == 0) {
            int8_t difference[RotaryEncoderCount] = {
                static_cast<int8_t>(g_RotaryEncoder[0].GetDifference()),
                static_cast<int8_t>(g_RotaryEncoder[1].GetDifference()),
                static_cast<int8_t>(g_RotaryEncoder[2].GetDifference()),
                static_cast<int8_t>(g_RotaryEncoder[3].GetDifference()),
            };

            bool hasDifference = (difference[0] != 0 || difference[1] != 0 || difference[2] != 0 || difference[3] != 0);
           if (hasDifference) {
                Console::Log("%d  % 3d  % 3d  % 3d  % 3d\n",
                    g_SamplingCount,
                    g_RotaryEncoder[0].GetDifference(),
                    g_RotaryEncoder[1].GetDifference(),
                    g_RotaryEncoder[2].GetDifference(),
                    g_RotaryEncoder[3].GetDifference()
                );
            }

            // Sample() と Commit() は要排他
            HAL_NVIC_DisableIRQ(TIM2_IRQn);
            for (auto& rotaryEncoder : g_RotaryEncoder) {
                rotaryEncoder.Commit();
            }
            HAL_NVIC_EnableIRQ(TIM2_IRQn);
        }
    }
}
