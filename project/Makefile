# vim: list!
# Good guide: http://nuclear.mutantstargoat.com/articles/make/
.PHONY: fuse clean size erase debug clear test upload

PROJECT=main
CC=avr-gcc
PROGRAMMER=usbasp
MCU=atmega644p
DMCU=$(MCU)
AVRDUDE=$(ASUDO) avrdude -p $(MCU) -c $(PROGRAMMER)

COMPILER_FLAGS=-mmcu=$(MCU)
COMPILER_FLAGS+=-Wall -Os
COMPILER_FLAGS+=-fno-threadsafe-statics # Remove __cxa_guard errors, when using static objects in functions

COMPILER_FLAGS+=-fdata-sections -ffunction-sections
COMPILER_FLAGS+=-Wl,--gc-sections -Wl,--relax

COMPILER_FLAGS+=-ffreestanding
COMPILER_FLAGS+=-funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums

COMPILER_FLAGS+=-std=c++11

CPPFILES= src/main.cpp lib/Test.cpp 
CPPFILES+=$(LIB_CPP)
OBJ=$(CPPFILES:.cpp=.o)
DEP=$(CPPFILES:.cpp=.d)

default: upload size

$(PROJECT).hex: $(PROJECT).elf
	avr-objcopy -j .text -j .data -O ihex $^ $@

$(PROJECT).elf: $(OBJ)
	$(CC) $(COMPILER_FLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(COMPILER_FLAGS) -c $< -o $@

-include $(DEP)   # include all dep files in the makefile
%.d: %.cpp
	@$(CC) $(COMPILER_FLAGS) $< -MM -MT $(@:.d=.o) >$@

size: $(PROJECT).hex
	avr-size -C --mcu=$(MCU) $(PROJECT).elf

upload: $(PROJECT).hex
	$(AVRDUDE) -U flash:w:$(PROJECT).hex
	
fuse:
	sudo $(AVRDUDE) -U hfuse:w:0xd9:m -U lfuse:w:0xe1:m

clean:
	rm -f $(PROJECT).obj $(PROJECT).hex $(PROJECT).cof $(PROJECT).eep.hex $(PROJECT).elf $(shell fd -I -e o -e d) upload

erase:
	avrdude -p $(MCU) -c $(PROGRAMMER) -e

clear:
	clear


.ccls: makefile
	@ echo -e \
avr-gcc\\n\
-target\\n\
avr\\n\
-nostdinc++\\n\
-mmcu=$(MCU)\\n\
-I/usr/avr/include/ > .ccls
