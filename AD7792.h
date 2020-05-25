#ifndef AD_H
#define AD_H

#include <stdint.h>
#include "Singleton.h"
#include "SPI.h"

struct AD779X {
    static constexpr uint8_t READ			= 	0b01000000;
    static constexpr uint8_t WRITE		   	= 	0b00000000;
    static constexpr uint8_t STATUS_REG     = 	0b000 << 3;
    static constexpr uint8_t MODE_REG 		= 	0b001 << 3;
    static constexpr uint8_t CONF_REG		= 	0b010 << 3;
    static constexpr uint8_t DATA_REG	    = 	0b011 << 3;

    // Status register AD7792
    static constexpr uint8_t DRDY		    = 	1 << 7;		// Data Ready
    static constexpr uint8_t CHANNEL0     	= 	0b000 << 0;
    static constexpr uint8_t CHANNEL1     	= 	0b001 << 0;
    static constexpr uint8_t CHANNEL2      	= 	0b010 << 0;

    // Mode register AD7792
    static const unsigned int CONTS_CONV	= 	0b000 << 13;	// Continuous conversion
    static const unsigned int SINGL_CONV	= 	0b001 << 13;	// Single conversion
    static const unsigned int INT_ZSC		= 	0b100 << 13;// Internal Zero Scale Calibration
    static const unsigned int INT_FSC		= 	0b101 << 13;// Internal Full Scale Calibration
    static const unsigned int UPD_RATE_4HZ	= 	0b1 << 0;	// Sampling frequency 4.17 Hz
    static const unsigned int UPD_RATE_123HZ= 	0b11 << 0;	// Sampling frequency 4.17 Hz

    // Configuration register AD7792
    static const unsigned int UNIPOLAR		= 	1 << 12;
    static const unsigned int BIPOLAR		= 	0;
    static const unsigned int GAIN_64		= 	0b110 << 8;
    static const unsigned int GAIN_128		= 	0b111 << 8;
    static const unsigned int GAIN_1		= 	0b000 << 8;
    static const unsigned int INT_REF		= 	1 << 7;
    static const unsigned int BUF			= 	1 << 4;
};

class AD7792 {
    friend class Singleton<AD7792>;
public:
    const static uint8_t CS_PIN = PB0;
    void reset();
    void calibrateChannels();
    void startConversion(uint8_t channel);
    float getValue();
    void setSPI(SPI& spi);

    const AD7792 & operator=(const AD7792 &) = delete ;

    constexpr AD7792(const AD7792 &) = default ;

private:
    constexpr static float vrefValue = 1.17f;
    void calibrateChannel(uint8_t channel);
    uint8_t readStatus();
    SPI* spi;
    AD7792() {};
};

#endif

