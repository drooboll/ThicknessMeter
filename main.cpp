#include <iostream>
#include "Filter.h"
#include "FileReader.h"
#include "Meter.h"

int main() {
    auto filter = Filter::getInstance();
    auto meter = Meter::getInstance();

    auto fileReader = new FileReader(R"(C:\Users\Valery\CLionProjects\thickness-meter-test\capture.csv)");

    float x, y;

    bool isRead;

    std::fstream fout("out.csv", std::fstream::out);

    int count = 0;
    do {
        isRead = fileReader->getNext(&x, &y);

        float normalizedValue = filter->normalize(x, y);
        float value = filter->calculateNext(normalizedValue);
        meter->addValue(value);
        if (count)
            fout << x << ", " << y << ", " << normalizedValue << ", " << value << ", "<< meter->getExtremaCount() << ", " << meter->getThickness() << std::endl;
        count++;
    } while(count < 65000);

    return 0;
}