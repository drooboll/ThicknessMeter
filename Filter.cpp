#include "Filter.h"
#include <cmath>

float Filter::a;
float Filter::b;
float Filter::prevValue;

bool Filter::isFirstCalculation;

Filter* Filter::self;

Filter::Filter(){
    this->a = 0.1f;
    this->b = 2.0f;
    this->isFirstCalculation = true;
}

Filter* Filter::getInstance() {
    if (self == nullptr){
        self = new Filter();
    }
    return self;
}

float Filter::calculateNext(float signalValue) {
    if (isFirstCalculation){
        isFirstCalculation = false;

        prevValue = signalValue;

        return signalValue;
    }

    prevValue = signalValue / (a + b * std::fabs(prevValue));

    return prevValue;
}

float Filter::getPrevValue() {
    if (isFirstCalculation) {
        return 0;
    }

    return prevValue;
}