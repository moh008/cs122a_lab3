/*	Partner(s) Name & E-mail: Minwhan Oh & moh008@ucr.edu
 * Lab Section: 021
 * assignment: Lab3 Exercise 1: Servant code
 * Created: 2019-15-2019 4:52 pm
 * Author : Stephanie Cabrera
 *
 * I acknowledge all content contained herein, excluding template
 * or example code, is my own original work.
 */

//#include <lcd.h>
//#include <keypad.h>
#include <timer.h>
//#include <scheduler.h>

unsigned char receivedData = 0x00;

//Servant code
void SPI_ServantInit(void){
	//set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	// make sure global interrupts are enabled on SREG register (pg.9)
	DDRB = 0x40; // 0100 0000
	SPCR = 0xC0; // 1100 0000
}
ISR(SPI_STC_vect){
	// this is enabled in init with the SPCR register's "SPI Interrupt Enable"
	// SPDR contains the received data, e.g. unsigned char receivedData = SPDR;
	receivedData = SPDR;
}

int main(void)
{
    /* Replace with your application code */
	DDRD = 0xFF; PORTD = 0x00; // These are the LEDs.
	//DDRD = 0xFF; PORTD = 0x00;
	//DDRA = 0x0F; PORTA = 0xF0;
	
    while (1) 
    {
		PORTD = receivedData;
    }
	return 0;
}

