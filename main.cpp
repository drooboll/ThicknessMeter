#include <iostream>
#include "Filter.h"
#include "FileReader.h"

int main() {
    auto filter = Filter::getInstance();

    auto fileReader = new FileReader("C:\\Users\\Valery\\CLionProjects\\thickness-meter-test\\capture.csv");

    float x, y;

    bool isRead;
    do {
        isRead = fileReader->getNext(&x, &y);

        std::cout << filter->calculateNext(x) << ' ' << y << std::endl;
    } while(isRead);

    return 0;
}