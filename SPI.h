#pragma once

#include <stdint-gcc.h>
#include "Singleton.h"

class SPI {
    friend class Singleton<SPI> ;
public:
    void sendByte(uint8_t byte);
    void sendWord(uint16_t word);
    uint8_t readByte();
    uint16_t readWord();

    const SPI & operator=(const SPI &) = delete ;
private:
    constexpr SPI() {} ;
    constexpr SPI(const SPI &) = default ;
};
