#include "Meter.h"

float Meter::window[WINDOW_SIZE];
uint16_t Meter::insertIndex;
const float Meter::highThresholdCoefficient = 0.9f;
const float Meter::lowThresholdCoefficient = 0.8f;
float Meter::normalLevel;
float Meter::windowSum;
Meter* Meter::self;

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

    if (insertIndex < WINDOW_SIZE - 1){
        window[insertIndex] = value;
        insertIndex++;

        windowSum += value;
    } else {
        windowShiftLeft();
        window[WINDOW_SIZE - 1] = value;

        windowSum += value;
        windowSum -= window[0];
    }
}