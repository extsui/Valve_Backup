#include "RotaryEncoder.h"

#ifdef ROTARY_ENCODER_DEBUG
#include "Console.h"
#endif

void RotaryEncoder::SetPortPin(PortPin *phaseA, PortPin *phaseB)
{
    // TODO: assert(phaseA.Port == nullptr);
    // TODO: assert(phaseB.Port == nullptr);

    m_phaseA.Port = phaseA->Port;
    m_phaseA.Pin  = phaseA->Pin;
    m_phaseB.Port = phaseB->Port;
    m_phaseB.Pin  = phaseB->Pin;
}

void RotaryEncoder::Sample()
{
    // < 変換アルゴリズム >
    //  0      1      3      2
    //  00 --> 01 --> 11 --> 10 : ロータリーエンコーダのグレイコード
    // (AB)
    //  0      1      2      3
    //  00 --> 01 --> 10 --> 11 : 2進数に変換したもの
    //
    // < 変換手順 >
    //  0 ビット目と 1 ビット目を入れ替えた値と元の値を XOR をとる
    //
    // < 前回値との差分 >
    // (正転)
    // current  : 0  1  2  3  0  1  2  3  0 ..
    // previous : *  0  1  2  3  0  1  2  3 ..
    // diff     : *  1  1  1 -3  1  1  1 -3 ..
    // ---------:------------------------------
    // signed   : *  1  1  1  1  1  1  1  1 ..
    //
    // (逆転)
    // current  : *  0  1  2  3  0  1  2  3 ..
    // previous : 0  1  2  3  0  1  2  3  0 ..
    // diff     : * -1 -1 -1  3 -1 -1 -1  3 ..
    // ---------:------------------------------
    // signed   : * -1 -1 -1 -1 -1 -1 -1 -1 ..

    auto pinStateToValue = [](GPIO_PinState state) -> uint8_t
    {
        if (state == GPIO_PIN_RESET) {
            return 0;
        } else {
            return 1;
        }
    };

    auto stateA = HAL_GPIO_ReadPin(m_phaseA.Port, m_phaseA.Pin);
    auto stateB = HAL_GPIO_ReadPin(m_phaseB.Port, m_phaseB.Pin);

    uint8_t pinA = pinStateToValue(stateA);
    uint8_t pinB = pinStateToValue(stateB);

    uint8_t grayCode = (pinA << 1) | pinB;
    uint8_t currentValue = ((grayCode >> 1) ^ grayCode) & 0x3;

    // 差 (0 ~ 3) を -2 ~ +1 の範囲に変換した値
    // 00 : 0
    // 01 : 1
    // 10 : 2 --> -2
    // 11 : 3 --> -1
    uint8_t diff = (currentValue + (~m_previousValue + 1)) & 0x3;   // 2 の補数
    int8_t diffSigned = static_cast<int8_t>(diff);
    if (diff == 2) {
        diffSigned = -2;
    } else if (diff == 3) {
        diffSigned = -1;
    }

#ifdef ROTARY_ENCODER_DEBUG
    if (diff != 0) {
        Console::Log("diff = %d (s: % d)  cur = %d (g: %d)  pre = %d\n",
            diff, diffSigned, currentValue, grayCode, m_previousValue);
    }
#endif

    if ((diffSigned == 1) || (diffSigned == -1)) {
        m_difference += diffSigned;
        m_isUpdated = true;
    } else if (diffSigned == 0) {
        // 静止
    } else {
        // エラー
        // サンプリング周期より早く回した場合やチャタリング除去に失敗した場合に発生
        m_totalErrorCount++;
    }

    m_previousValue = currentValue;
}

void RotaryEncoder::Commit()
{
    m_totalPosition += m_difference;
    m_difference = 0;
    m_isUpdated = false;
}

int RotaryEncoder::GetTotalPosition()
{
    return m_totalPosition;
}

int RotaryEncoder::GetTotalErrorCount()
{
    return m_totalErrorCount;
}

int RotaryEncoder::GetDifference()
{
    return m_difference;
}

bool RotaryEncoder::IsUpdated()
{
    return m_isUpdated;
}
