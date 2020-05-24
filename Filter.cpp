#include "Filter.h"
#include <cmath>

const float Filter::a = 0.1f;
const float Filter::b = 2.0f;
float Filter::maxValue;
const float Filter::averageLevelCoefficient = 0.8f;
float Filter::inverseMaxPeakValue;
const float maxPeakCoefficient = 0.2f;
float Filter::averageLevel;
float Filter::controlAverageLevel = 0.0f;
uint16_t Filter::averageCount = 0;


bool Filter::isFirstCalculation = true;

Filter* Filter::self;

Filter* Filter::getInstance() {
    if (self == nullptr){
        self = new Filter();
    }
    return self;
}

float Filter::calculateNext(float signalValue) {
    if (isFirstCalculation){
        isFirstCalculation = false;

        maxValue = signalValue;
        inverseMaxPeakValue = (1.0f / maxPeakCoefficient) / maxValue;
        averageLevel = signalValue * averageLevelCoefficient;

        return signalValue;
    }

    float centeredValue = signalValue - averageLevel;

    float normalizedValue = (centeredValue) * inverseMaxPeakValue;

    return centeredValue / (a + b * (float)fabs(normalizedValue)) + averageLevel;
}

float Filter::getMaxValue() {
    if (isFirstCalculation) {
        return 0;
    }

    return maxValue;
}

float Filter::normalize(float signalValue, float controlValue) {
    float calculatedAverageLevel;
    if (averageCount){
        calculatedAverageLevel = controlAverageLevel / averageCount;
    } else {
        calculatedAverageLevel = controlValue;
    }

    controlAverageLevel += controlValue;
    averageCount++;

    return signalValue + controlValue - calculatedAverageLevel;
}