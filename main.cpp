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

    do {
        isRead = fileReader->getNext(&x, &y);

        float value = filter->calculateNext(filter->normalize(x, y));
        meter->addValue(value);
        fout << value << ", "<< meter->getExtremaCount() << " " << meter->getThickness() << std::endl;
    } while(isRead);

    return 0;
}