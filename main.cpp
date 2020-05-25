#include <avr/io.h>
#include <avr/interrupt.h>

#include "Singleton.h"
#include "SPI.h"
#include "Filter.h"
#include "Meter.h"
#include "AD7792.h"
#include "UARTComm.h"


ISR(USART_RX_vect){
    auto uart = Singleton<UARTComm>::getInstance();
    uart.onRxComplete();
}

ISR(USART_TX_vect){
    auto uart = Singleton<UARTComm>::getInstance();
    uart.onTxComplete();
}

int main() {
    auto spi = Singleton<SPI>::getInstance();
    auto adc = Singleton<AD7792>::getInstance();
    auto uart = Singleton<UARTComm>::getInstance();

    auto filter = Singleton<Filter>::getInstance();
    auto meter = Singleton<Meter>::getInstance();

    uart.init(115200);
    uart.setMeter(meter);

    sei();

    spi.init(AD7792::CS_PIN);

    adc.setSPI(spi);
    adc.reset();
    adc.calibrateChannels();

    while (true) {
        adc.startConversion(0);
        float mainValue = adc.getValue();
        adc.startConversion(1);
        float refValue = adc.getValue();
        meter.addValue(filter.calculateNext(filter.normalize(mainValue, refValue)));
    };

    return 0;
}