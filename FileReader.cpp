#include <iostream>
#include "FileReader.h"

std::string FileReader::filename;

FileReader::FileReader(const std::string &filename) {
    this->fs.open(filename, std::fstream::in);
    this->filename = filename;
}

bool FileReader::getNext(float* value1, float* value2) {
    if (fs.eof()){
        return false;
    }
    fs >> *value1;
    fs >> *value2;
    return true;
}
