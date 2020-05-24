#include <cmath>
#include <iostream>
#include "Meter.h"

const uint16_t tableCoefficient = 99;
const uint16_t tableDivider = 10000;
const uint16_t tableShift = 0;
uint16_t inverseValueTable[] = {
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
        5697, 5791, 5884, 5978, 6362, 6838, 7313, 7788, 10000, };

float Meter::window[WINDOW_SIZE];
uint16_t Meter::insertIndex;
const float Meter::highThresholdCoefficient = 0.85f;
const float Meter::lowThresholdCoefficient = 0.72f;
const float Meter::error = 0.02f;
float Meter::normalLevel;
float Meter::windowSum;
Meter* Meter::self;
bool Meter::isFirstExtremas = true;
bool Meter::wasMaxima = true;
float Meter::partialThickness = 0.0f;

float Meter::maximumsSum;
uint16_t Meter::maximumsCount;

float Meter::minimumsSum;
uint16_t Meter::minimumsCount;

Meter* Meter::getInstance() {
    if (self == nullptr){
        self = new Meter();
    }
    return self;
}

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

    if (isFirstExtremas){
        isMaximum = window[WINDOW_SIZE/2] > highThresholdCoefficient * normalLevel;
        isMinimum = window[WINDOW_SIZE/2] < lowThresholdCoefficient * normalLevel;
    } else {
        float maxFraction = getWindowAverage() / getMaximumsAverage();
        float minFraction = getWindowAverage() / getMinimumsAverage();

        isMaximum = std::fabs(1 - maxFraction) < error;
        isMinimum = std::fabs(1 - minFraction) < error;
    }

    if (isMinimum && wasMaxima){
        float minima = window[0];

        for (uint8_t i = 0; i < WINDOW_SIZE; i++){
            if (window[i] - minima < 0.0f){
                minima = window[i];
            }
        }

        if (std::fabs(minima - window[WINDOW_SIZE/2]) < 0.01f){
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

        if (std::fabs(maxima - window[WINDOW_SIZE/2]) < 0.01f){
            maximumsSum += maxima;
            maximumsCount++;

            wasMaxima = true;

            if (isFirstExtremas){
                isFirstExtremas = false;
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
    float floorValue = std::floor(normalizedValue);

    float tableValue;

    if (std::abs(normalizedValue - floorValue) < 0.01f){
        tableValue = inverseValueTable[(uint16_t) (floorValue + tableShift)];
    } else {
        float highIndexPart = normalizedValue - floorValue;
        float lowIndexPart = 1.0f - (highIndexPart);

        tableValue = highIndexPart * inverseValueTable[(uint16_t) (floorValue + tableShift + 1)] +
                      lowIndexPart * inverseValueTable[(uint16_t) (floorValue + tableShift)];
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
        partialThickness = measuredThickness;
    }

    return partialThickness;
}