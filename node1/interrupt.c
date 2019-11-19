#include "interrupt.h"
#include <avr/io.h>

void interrupt_init() {
	GICR |= (1 << INT0); // Turn on INT0 interrupt
	MCUCR |= (1 << ISC01); // Falling edge
	MCUCR &= ~(1 << ISC00); // ...
	DDRD &=  ~(1 << PIND2);
}
