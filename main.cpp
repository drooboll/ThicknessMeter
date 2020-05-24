#include "Singleton.h"
#include "SPI.h"
#include "Filter.h"
#include "Meter.h"
#include "AD7792.h"

int main() {
    auto spi = Singleton<SPI>::getInstance();
    auto adc = Singleton<AD7792>::getInstance();
    // Add uart & uart control

    auto filter = Singleton<Filter>::getInstance();
    auto meter = Singleton<Meter>::getInstance();

    spi.init(AD7792::CS_PIN);

    adc.setSPI(spi);
    adc.reset();
    adc.calibrateChannels();

    float filtered = filter.calculateNext(5.7);
    meter.addValue(filtered);

    while (true) {
        adc.startConversion(0);
        float mainValue = adc.getValue();
        adc.startConversion(1);
        float refValue = adc.getValue();
        meter.addValue(filter.calculateNext(filter.normalize(mainValue, refValue)));
    };

    return 0;
}