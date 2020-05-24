#include "SPI.h"

void SPI::init(uint8_t csPins) {
    SPI_CS_DDR |= _BV(csPins);
    SPI_CS_PORT |= _BV(csPins);

    SPI_DDR |= _BV(SPI_SCK) | _BV(SPI_MOSI) | _BV(SPI_SS);
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(CPOL) | _BV(CPHA);
}

void SPI::setCS(uint8_t csPins) {
    SPI_CS_PORT &= ~_BV(csPins);
}

void SPI::unsetCS(uint8_t csPins) {
    SPI_CS_PORT |= ~_BV(csPins);
}


void SPI::sendByte(uint8_t byte) {
    SPDR = byte;
    while(!(SPSR & (1<<SPIF)));
}

uint16_t SPI::readWord(uint16_t message) {
    SPDR = message >> 8;
    while(!(SPSR & (1<<SPIF)));
    uint16_t data = SPDR;
    data <<= 8;
    SPDR = message & 0xFF;
    while(!(SPSR & (1<<SPIF)));
    data |= SPDR;
    return data;
}

uint8_t SPI::readByte(uint8_t message) {
    SPDR = message;
    while(!(SPSR & (1<<SPIF)));
    uint8_t data = SPDR;
    return data;
}

void SPI::sendWord(uint16_t word) {
    SPDR = word >> 8;
    while(!(SPSR & (1<<SPIF)));

    SPDR = word & 0xff;
    while(!(SPSR & (1<<SPIF)));
}