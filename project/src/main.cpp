#define __AVR_ATmega644P__
#define F_CPU 16000000UL
#define BAUD 115200

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


#include "../lib/Test.hpp"

int main(){
    int a = test();
    
    return 0;
}
