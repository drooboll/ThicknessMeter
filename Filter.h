#include <stdint.h>
#include "Singleton.h"

class Filter {
    friend class Singleton<Filter>;
private:
    constexpr Filter() {};
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

public:
    const Filter & operator=(const Filter &) = delete ;

    float calculateNext(float signalValue);

    float normalize(float signalValue, float controlValue);

    float getMaxValue();

    constexpr Filter(const Filter &) = default ;
};