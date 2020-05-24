#include "AD7792.h"

void AD7792::startConversion(uint8_t channel) {
    spi->setCS(AD7792::CS_PIN);
    spi->sendByte(AD779X::WRITE | AD779X::CONF_REG);//comm: write config
    spi->sendWord(AD779X::UNIPOLAR | AD779X::GAIN_1| AD779X::INT_REF | AD779X::BUF | channel);
    spi->sendByte(AD779X::WRITE | AD779X::MODE_REG);//comm: write mode
    spi->sendWord(AD779X::CONTS_CONV | AD779X::UPD_RATE_123HZ);
    spi->unsetCS(AD7792::CS_PIN);
}

float AD7792::getValue(){
    spi->setCS(AD7792::CS_PIN);
    spi->sendByte(AD779X::READ | AD779X::DATA_REG);//comm: read data
    uint16_t value = spi->readWord(0xffff);
    spi->unsetCS(AD7792::CS_PIN);
    return value * vrefValue / 0x8000;
}

void AD7792::calibrateChannel(uint8_t channel) {
    spi->setCS(AD7792::CS_PIN);
    spi->sendByte(AD779X::WRITE | AD779X::CONF_REG);
    spi->sendWord(AD779X::UNIPOLAR | AD779X::GAIN_1 | AD779X::INT_REF | AD779X::BUF | channel);

    spi->sendByte(AD779X::WRITE | AD779X::MODE_REG);
    spi->sendWord(AD779X::INT_ZSC | AD779X::UPD_RATE_123HZ);
    spi->unsetCS(AD7792::CS_PIN);

    // Check !DRDY flag
    while((readStatus() & AD779X::DRDY));

    spi->setCS(AD7792::CS_PIN);
    spi->sendByte(AD779X::WRITE | AD779X::MODE_REG);
    spi->sendWord(AD779X::INT_FSC | AD779X::UPD_RATE_123HZ);
    spi->unsetCS(AD7792::CS_PIN);

    while((readStatus() & AD779X::DRDY));
}

void AD7792::calibrateChannels() {
    calibrateChannel(0);
    calibrateChannel(1);
    calibrateChannel(2);
}

uint8_t AD7792::readStatus() {
    spi->setCS(AD7792::CS_PIN);
    spi->sendByte(AD779X::READ | AD779X::STATUS_REG);
    uint8_t status = spi->readByte(0xff);
    spi->unsetCS(AD7792::CS_PIN);
    return status;
}

void AD7792::reset() {
    if (spi == nullptr)
        return;

    spi->setCS(AD7792::CS_PIN);
    spi->sendWord(0xffff);
    spi->sendWord(0xffff);
    spi->setCS(AD7792::CS_PIN);
}

void AD7792::setSPI(SPI &spi) {
    this->spi = &spi;
}