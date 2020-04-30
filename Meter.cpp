#include <cmath>
#include <iostream>
#include "Meter.h"

float Meter::window[WINDOW_SIZE];
uint16_t Meter::insertIndex;
const float Meter::highThresholdCoefficient = 0.85f;
const float Meter::lowThresholdCoefficient = 0.735f;
const float Meter::error = 0.02f;
float Meter::normalLevel;
float Meter::windowSum;
Meter* Meter::self;
bool Meter::isFirstExtremas = true;
bool Meter::wasMaxima = true;

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

            return;
        }
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