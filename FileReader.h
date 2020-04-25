#pragma once
#include <string>
#include <fstream>

class FileReader {
    static std::string filename;
    std::fstream fs;

public:
    FileReader(const std::string& filename);

    bool getNext(float* value1, float* value2);
};
