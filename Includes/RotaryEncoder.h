#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "stm32l0xx_hal.h"
#include "stm32l0xx_hal_gpio.h"

// ロータリーエンコーダで使用する用のポートピン
struct PortPin
{
    GPIO_TypeDef *Port;
    uint16_t Pin;

    explicit PortPin(GPIO_TypeDef *port, uint16_t pin)
    {
        Port = port;
        Pin = pin;
    }
};

class RotaryEncoder
{
public:
    RotaryEncoder()
     : m_phaseA(nullptr, 0)
     , m_phaseB(nullptr, 0)
     , m_totalPosition(0)
     , m_difference(0)
     , m_isUpdated(false)
     , m_previousValue(0)
    {
    }

    ~RotaryEncoder()
    {
    }

    void SetPortPin(PortPin *phaseA, PortPin *phaseB);

    // A/B 相のサンプリング
    // ポーリング方式なのでノイズ除去が適切に働く周期で呼び出すこと
    void Sample();

    // 今回の差分をコミットする
    // 呼び出し側で値取得の一区切りのタイミングで呼び出すこと
    void Commit();

    int GetTotalPosition();
    int GetDifference();
    bool IsUpdated();

private:
    // A 相
    PortPin m_phaseA;
    // B 相
    PortPin m_phaseB;

    // 起動時からの積算位置
    int m_totalPosition;

    // 前回からの差分
    int m_difference;
    // 前回から更新があったか
    bool m_isUpdated;

    // 前回の値 (内部計算用)
    uint8_t m_previousValue;
};

#endif // ROTARY_ENCODER_H
