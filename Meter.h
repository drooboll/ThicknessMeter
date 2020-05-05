#pragma once

#include <cstdint>

#define WINDOW_SIZE 30

class Meter {
private:
    static float window[WINDOW_SIZE];
    static uint16_t insertIndex;
    static const float highThresholdCoefficient;
    static const float lowThresholdCoefficient;
    static const float error;
    static float normalLevel;
    static float windowSum;
    static bool isFirstExtremas;
    static bool wasMaxima;
    static float partialThickness;
    static float lastNormalizedValue;

    static float maximumsSum;
    static uint16_t maximumsCount;

    static float minimumsSum;
    static uint16_t minimumsCount;

    Meter() = default;
    static Meter* self;

    void windowShiftLeft();
    float getWindowAverage();
    float getMaximumsAverage();
    float getMinimumsAverage();
    void checkWindow();
    float calcPartialThickness(float signalValue);
    float getInverseByTable(float value);

public:
    static Meter* getInstance();
    void addValue(float value);
    uint16_t getExtremaCount();
    float getThickness();
};
