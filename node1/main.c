#include <avr/io.h>
#include "uart.h"
#include "joystick.h"
#include <stdio.h>

#define F_CPU 4915200
#include <util/delay.h>

#include "oled.h"
#include <string.h>
#include "menu.h"
#include "spi.h"
#include "MCP2515.h"
#include <avr/interrupt.h>
#include "sram.h"
#include "draw.h"
#include "MCP2515.h"
#include "mcp.h"
#include "can.h"

#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define UBRR FOSC/16/BAUD-1

int main(void){
	uart_init(UBRR);
	can_init(); // Denne initierer mcp, som initierer spi.
	mcp_set_mode(MODE_NORMAL);

	// Lab 6
	adc_init();
	//sram_init();
	//oled_init();
	//oled_clear();

	//menu_ptr menu = menu_init();
	//menu_start(menu);



	// Interruptgreier
	mcp_bit_modify(MCP_CANINTE, 0b11111111, 0b1); // Skrur på receive0-interrupt. Skrur av alt annet.
	GICR |= (1 << INT0); // Skrur på INT0-interrupt
	MCUCR |= (1 << ISC01); // Setter interrupts til å funke på fallende kant
	MCUCR &= ~(1 << ISC00); // ...
	DDRD &=  ~(1 << PIND2);

	printf("før sei\r\n");
	//GIFR = 0;
	sei(); // Skrur på interrupts globalt
	printf("etter sei\r\n");

/*
	// Sender melding
	message_t message = {
		1, // Id
		5, // Lengde
		"heiiii" // Data. Maks åtte byte
	};
	can_send(&message); // Sender melding
	printf("har sendt melding\r\n");


	while(1){
		printf("w");
		_delay_ms(1000);
	}

	*/



	joy_calibrate();

  while (1) {
    printf("hei");
    joy_send_coordinates();
    _delay_ms(10);
  }

	return 0;
}

ISR(INT0_vect) {
	message_t receive = can_receive(); // Mottar melding
	if (receive.length > 8) {
		printf("Kaos. Meldingslengde: %d\r\n", receive.length);
	} else {
		printf("Heisann sveisann, vi har fått ei melding.\r\n");
		printf("Id: %d \r\n", receive.id);
		printf("Lengde: %d \r\n", receive.length);
		printf("Melding: %s \r\n\r\n", receive.data);
	}
	// Resetter interrupt for motta-buffer0
	mcp_bit_modify(MCP_CANINTF, 0b1, 0);
}

ISR(SPI_STC_vect) {
	//printf("\r\nSPI_STC_vect\r\n");
}

// EKSTRAPOENG!!!

ISR(BADISR_vect) {
	printf("b");
}
