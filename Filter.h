#pragma once

class Filter {
private:
    static float a;
    static float b;
    static float prevValue;
    static bool isFirstCalculation;

    static Filter* self;

    Filter();

public:
    static Filter* getInstance();

    float calculateNext(float signalValue);

    float getPrevValue();
};