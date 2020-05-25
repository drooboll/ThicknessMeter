#include "UARTComm.h"
#include "string.h"
#include <stdio.h>

void UARTComm::init(uint32_t baudrate) {
    uint16_t value = F_CPU / (16 * baudrate) - 1;
    UBRR0H = value >> 8;
    UBRR0L = value & 0xFF;

    UCSR0B = _BV(TXEN0) | _BV(TXCIE0) | _BV(RXCIE0);
}

void UARTComm::setMeter(Meter &m) {
    meter = &m;
}

void UARTComm::storeChar(uint8_t byte) {
    if (inputBufferPointer == BUFFER_SIZE)
        inputBufferPointer = 0;

    inputBuffer[inputBufferPointer] = byte;

    if (byte == '\n'){
        executeCommand();
    }
}

void UARTComm::executeCommand() {
    inputBuffer[inputBufferPointer] = 0;

    if (strstr((char*)inputBuffer, "THS?")){
        sendThickness();
        inputBufferPointer = 0;
        return;
    }

    if (strstr((char*)inputBuffer, "SPD?")){
        sendSpeed();
        inputBufferPointer = 0;
        return;
    }

    if (strstr((char*)inputBuffer, "ALL?")){
        sendAll();
        inputBufferPointer = 0;
        return;
    }
}

void UARTComm::onRxComplete() {
    uint8_t byte = UDR0;
    storeChar(byte);
}

void UARTComm::onTxComplete() {
    if (isTransferred)
        return;

    outputBufferPointer++;

    if (outputBufferPointer <= outputMessageLength)
        UDR0 = outputBuffer[outputBufferPointer];
    else
        isTransferred = true;
}

bool UARTComm::sendThickness() {
    if (!isTransferred)
        return false;

    double thickness = meter->getThickness();

    outputMessageLength = (uint8_t) sprintf((char*)outputBuffer, "+THS:%f\n", thickness);
    outputBufferPointer = 0;
    isTransferred = false;

    UDR0 = outputBuffer[0];

    return true;
}

bool UARTComm::sendSpeed() {
    if (!isTransferred)
        return false;

    double speed = meter->getSpeed();

    outputMessageLength = (uint8_t) sprintf((char*)outputBuffer, "+SPD:%f\n", speed);
    outputBufferPointer = 0;
    isTransferred = false;

    UDR0 = outputBuffer[0];

    return true;
}

bool UARTComm::sendAll() {
    if (!isTransferred)
        return false;

    double thickness = meter->getThickness();
    double speed = meter->getSpeed();

    outputMessageLength = (uint8_t) sprintf((char*)outputBuffer, "+THS:%f\n+SPD:%f\n", thickness, speed);
    outputBufferPointer = 0;
    isTransferred = false;

    UDR0 = outputBuffer[0];

    return true;
}
