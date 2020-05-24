#ifndef SPI_H
#define SPI_H

#include "Singleton.h"

#include <stdint-gcc.h>
#include <avr/io.h>

#define SPI_PORT PORTB
#define SPI_DDR DDRB
#define SPI_PIN PINB

#define SPI_CS_PORT PORTB
#define SPI_CS_DDR DDRB

class SPI {
    friend class Singleton<SPI>;
public:
    const static uint8_t SPI_SCK = PB5;
    const static uint8_t SPI_MOSI = PB3;
    const static uint8_t SPI_MISO = PB4;
    const static uint8_t SPI_SS = PB2;

    void init(uint8_t csPins);
    void setCS(uint8_t csPins);
    void unsetCS(uint8_t csPins);
    void sendByte(uint8_t byte);
    void sendWord(uint16_t word);
    uint8_t readByte(uint8_t message);
    uint16_t readWord(uint16_t message);

    const SPI & operator=(const SPI &) = delete ;

    constexpr SPI(const SPI &) = default ;

private:
    constexpr SPI() {} ;
};

#endif
