#ifndef METER_H
#define METER_H

#include <avr/pgmspace.h>
#include <stdint.h>
#include "Singleton.h"

#define WINDOW_SIZE 30

const uint16_t inverseValueTable[] PROGMEM = {
        0,
        2174, 2650, 3125, 3600, 4015, 4108, 4202, 4295, 4389, 4483,
        4510, 4523, 4536, 4548, 4561, 4574, 4587, 4599, 4612, 4625,
        4638, 4650, 4663, 4676, 4688, 4701, 4714, 4727, 4739, 4752,
        4765, 4777, 4790, 4803, 4816, 4828, 4841, 4854, 4866, 4879,
        4892, 4905, 4917, 4930, 4943, 4955, 4968, 4981, 4994, 5006,
        5019, 5032, 5044, 5057, 5070, 5083, 5095, 5108, 5121, 5133,
        5146, 5159, 5171, 5184, 5197, 5209, 5222, 5235, 5248, 5260,
        5273, 5286, 5298, 5311, 5324, 5336, 5349, 5362, 5374, 5387,
        5400, 5413, 5425, 5438, 5451, 5463, 5476, 5489, 5510, 5604,
        5697, 5791, 5884, 5978, 6362, 6838, 7313, 7788, 10000
};

class Meter {
    friend class Singleton<Meter>;
private:
    constexpr Meter() {} ;
    static float window[WINDOW_SIZE];
    static uint16_t insertIndex;
    static const float highThresholdCoefficient;
    static const float lowThresholdCoefficient;
    static const double error;
    static float normalLevel;
    static float windowSum;
    static bool isFirstPair;
    static bool wasMaxima;
    static float partialThickness;

    static float maximumsSum;
    static uint16_t maximumsCount;

    static float minimumsSum;
    static uint16_t minimumsCount;

    void windowShiftLeft();
    float getWindowAverage();
    float getMaximumsAverage();
    float getMinimumsAverage();
    void checkWindow();
    float calcPartialThickness(float signalValue);
    float getInverseByTable(float value);

public:
    void addValue(float value);
    uint16_t getExtremaCount();
    float getThickness();
    const Meter & operator=(const Meter &) = delete ;

    constexpr Meter(const Meter &) = default ;
};

#endif
