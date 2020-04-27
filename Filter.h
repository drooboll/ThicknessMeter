#pragma once

#include <cstdint>

class Filter {
private:
    static const float a;
    static const float b;
    static const float averageLevelCoefficient;
    static float averageLevel;
    static float maxValue;
    static float inverseMaxPeakValue;
    static const float peakCoefficient;
    static bool isFirstCalculation;
    static float controlAverageLevel;
    static uint16_t averageCount;

    static Filter* self;

    Filter() = default;

public:
    static Filter* getInstance();

    float calculateNext(float signalValue);

    float normalize(float signalValue, float controlValue);

    float getMaxValue();
};