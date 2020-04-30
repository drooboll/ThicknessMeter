#pragma once

#include <cstdint>

#define WINDOW_SIZE 30

class Meter {
private:
    static float window[WINDOW_SIZE];
    static uint16_t insertIndex;
    static const float highThresholdCoefficient;
    static const float lowThresholdCoefficient;
    static float normalLevel;
    static float windowSum;

    Meter() = default;
    static Meter* self;

    void windowShiftLeft();
    float getWindowAverage();

public:
    Meter* getInstance();
    void addValue(float value);
    uint16_t getExtremaCount();
    float getThickness();

};
