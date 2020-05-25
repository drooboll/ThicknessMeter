avr-g++ -c main.cpp Singleton.h SPI.h SPI.cpp AD7792.h AD7792.cpp Filter.h Filter.cpp Meter.h Meter.cpp UARTComm.h UARTComm.cpp -mmcu=atmega168 -ffunction-sections -Os -std=gnu++14 -Wl,-u,vfprintf -lprintf_flt -lm -fno-threadsafe-statics
avr-g++ -mmcu=atmega168 -o main.elf main.o AD7792.o SPI.o Filter.o Meter.o UARTComm.o -Wl,-u,vfprintf -lprintf_flt -lm -fno-threadsafe-statics
avr-objcopy -O ihex -R .eeprom "main.elf" "main.hex"

#avrdude -p atmega168 -b 9600 -c usbasp -U flash:w:main.hex
