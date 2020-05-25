#include <math.h>
#include <stdint.h>
#include "Meter.h"

constexpr double delta = 0.001;
constexpr float timeConstant = 3.0f / 123.0f;

const uint16_t tableCoefficient = 99;
const uint16_t tableDivider = 10000;
const uint16_t tableShift = 0;

float Meter::window[WINDOW_SIZE];
uint16_t Meter::insertIndex;
const float Meter::highThresholdCoefficient = 0.85f;
const float Meter::lowThresholdCoefficient = 0.72f;
const double Meter::error = 0.02;
float Meter::normalLevel;
float Meter::windowSum;
float Meter::speed;

bool Meter::isFirstPair = true;
bool Meter::wasMaxima = true;
float Meter::partialThickness = 0.0f;

float Meter::maximumsSum;
uint16_t Meter::maximumsCount;

float Meter::minimumsSum;
uint16_t Meter::minimumsCount;

void Meter::windowShiftLeft() {
    for (uint8_t i = 0; i < WINDOW_SIZE - 1; i++){
        window[i] = window[i + 1];
    }
}

float Meter::getWindowAverage(){
    if (insertIndex != WINDOW_SIZE){
        if (insertIndex == 0){
            return 0.0f;
        }

        return windowSum / insertIndex;
    }

    return windowSum / WINDOW_SIZE;
}

void Meter::addValue(float value) {
    if (insertIndex == 0){
        normalLevel = value;
    }

    if (insertIndex <= WINDOW_SIZE - 1){
        window[insertIndex] = value;
        insertIndex++;

        windowSum += value;
    } else {
        windowShiftLeft();
        window[WINDOW_SIZE - 1] = value;

        windowSum += value;
        windowSum -= window[0];
    }
    if (insertIndex == WINDOW_SIZE) {
        checkWindow();
    }
}

void Meter::checkWindow() {
    bool isMaximum, isMinimum;

    if (isFirstPair){
        isMaximum = window[WINDOW_SIZE/2] > highThresholdCoefficient * normalLevel;
        isMinimum = window[WINDOW_SIZE/2] < lowThresholdCoefficient * normalLevel;
    } else {
        float maxFraction = getWindowAverage() / getMaximumsAverage();
        float minFraction = getWindowAverage() / getMinimumsAverage();

        isMaximum = fabs(1 - maxFraction) < error;
        isMinimum = fabs(1 - minFraction) < error;
    }

    if (isMinimum && wasMaxima){
        float minima = window[0];

        for (uint8_t i = 0; i < WINDOW_SIZE; i++){
            if (window[i] - minima < 0.0f){
                minima = window[i];
            }
        }

        if (fabs(minima - window[WINDOW_SIZE/2]) < delta){
            minimumsSum += minima;
            minimumsCount++;

            wasMaxima = false;
            partialThickness = 0.0f;
            return;
        }
    }

    if (isMaximum && !wasMaxima){
        float maxima = window[0];

        for (uint8_t i = 0; i < WINDOW_SIZE; i++){
            if (window[i] - maxima > 0.0f){
                maxima = window[i];
            }
        }

        if (fabs(maxima - window[WINDOW_SIZE/2]) < delta){
            maximumsSum += maxima;
            maximumsCount++;

            wasMaxima = true;

            if (isFirstPair){
                isFirstPair = false;
            }
            partialThickness = 0.0f;
            return;
        }
    }

    if (getExtremaCount() >= 2) {
        calcPartialThickness(window[WINDOW_SIZE/2]);
    }
}

float  Meter::getMaximumsAverage() {
    if (maximumsCount){
        return maximumsSum / maximumsCount;
    }
    return 0.0f;
}

float  Meter::getMinimumsAverage() {
    if (minimumsCount){
        return minimumsSum / minimumsCount;
    }
    return 0.0f;
}

uint16_t Meter::getExtremaCount() {
    return minimumsCount + maximumsCount;
}

float Meter::getThickness() {
    return getExtremaCount() + partialThickness;
}

// Gets inverse value (from 0.0f to 1.0f), using table values
// Table could be made 2x smaller.
float Meter::getInverseByTable(float value) {
    float normalizedValue = value * tableCoefficient;
    uint16_t floorValue = (uint16_t) floor(normalizedValue);

    float tableValue;

    if (fabs(normalizedValue - floorValue) < delta){
        tableValue = inverseValueTable[(uint16_t) (floorValue + tableShift)];
    } else {
        float highIndexPart = normalizedValue - floorValue;
        float lowIndexPart = 1.0f - (highIndexPart);

        tableValue = highIndexPart * pgm_read_word(&inverseValueTable[(uint16_t) (floorValue + tableShift + 1)]) +
                      lowIndexPart * pgm_read_word(&inverseValueTable[(uint16_t) (floorValue + tableShift)]);
    }

    tableValue /= (float) tableDivider;

    return tableValue;
}

float Meter::calcPartialThickness(float signalValue) {
    float averageMax = getMaximumsAverage();
    float averageMin = getMinimumsAverage();

    float extremaAverage = (averageMax + averageMin) / 2;
    float halfHeight = (averageMax - averageMin) / 2;

    float normalizedValue = (signalValue - extremaAverage) / halfHeight;

    normalizedValue = normalizedValue > 1.0f ? 1.0f : normalizedValue;
    normalizedValue = normalizedValue < -1.0f ? -1.0f : normalizedValue;

    if (wasMaxima)
        normalizedValue = 1 - (normalizedValue + 1) / 2;
    else
        normalizedValue = (normalizedValue + 1) / 2;

    float measuredThickness = getInverseByTable(normalizedValue);

    if (measuredThickness > partialThickness){
        speed = (measuredThickness - partialThickness) / timeConstant;
        partialThickness = measuredThickness;
    }

    return partialThickness;
}

float Meter::getSpeed() {
    return speed;
}