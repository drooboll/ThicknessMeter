#ifndef UART0_H
#define UART0_H

#define F_CPU 16000000UL

#include "Singleton.h"
#include "Meter.h"
#include "stdint.h"
#include <avr/io.h>

#define BUFFER_SIZE 50

class UARTComm {
    friend class Singleton<UARTComm>;
public:
    const UARTComm & operator=(const UARTComm &) = delete;
    constexpr UARTComm(const UARTComm &) = default;

    void init(uint32_t baudrate);
    void setMeter(Meter& m);
    void onRxComplete();
    void onTxComplete();

private:
    uint8_t inputBuffer[BUFFER_SIZE] = {(0)};
    uint8_t inputBufferPointer = 0;
    uint8_t outputBuffer[BUFFER_SIZE] = {(0)};
    uint8_t outputBufferPointer = 0;
    uint8_t outputMessageLength = 0;
    bool isTransferred = true;
    Meter* meter;
    UARTComm() {};
    void storeChar(uint8_t byte);
    void executeCommand();
    bool sendSpeed();
    bool sendThickness();
    bool sendAll();
};


#endif
