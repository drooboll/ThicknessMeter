#include <iostream>
#include "Filter.h"
#include "FileReader.h"

int main() {
    auto filter = Filter::getInstance();

    auto fileReader = new FileReader(R"(C:\Users\Valery\CLionProjects\thickness-meter-test\capture.csv)");

    float x, y;

    bool isRead;

    std::fstream fout("out.csv", std::fstream::out);
    do {
        isRead = fileReader->getNext(&x, &y);

        fout << filter->calculateNext(filter->normalize(x, y)) << std::endl;
    } while(isRead);

    return 0;
}