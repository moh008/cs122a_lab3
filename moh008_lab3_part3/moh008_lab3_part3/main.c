/* Partner : Stephanie scabr006@ucr.edu
 * Lab Section : 021
 * Assignment: Lab3 Exercise 1-3 (SPI Transmission)
 * Created: 2019-10-16 오후 3:52:18
 * Author : Lincoln
 *
 * I acknowledge all content contained herein, excluding template
 * or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Timer.h"
#include "keypad.h"
#include "lcd.h"
//global variable data
unsigned char data = 0x00;

void SPI_MasterInit(void)
{
	//Set DDRB to have MOSI (DDB5), SCK (DDB7), SS (DDB4) as output and 
	//MISO as input
	DDRB = 0xB0;	//as output DDRB 1011 0000
	PORTB = 0x40;	//as input PORTB 0100 0000
	//Set SPCR register to enable SPI, enable master, and use SCK frequency of fosc/16
	SPCR = 0x51; // 0101 0001
	//make sure global interrupts are enabled on SREG register
	SREG = 0x80; // Status register 1000 0000
}

void SPI_MasterTransmit (unsigned char cData)
{
	// data in SPDR will be transmitted, e.g. SPDR = cData;
	SPDR = cData;
	SetBit(PORTB, 4, 0);		// set SS low
	while(!(SPSR & (1<<SPIF))) { //wait for transmission to complete
		;
	}
	SetBit(PORTB, 4, 1);		//set SS high
}

unsigned long int findGCD(unsigned long int a, unsigned long int b)
{
	unsigned long int c;
	while(1){
		c = a%b;
		if (c==0){return b;}
			a = b;
			b = c;
	}
	return 0;
}

typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)( int );
	} task;

unsigned char x = 0;

enum SM1_States {SM1_readkeypad};
int SMTick1 (int state)
{
	switch(state)
	{
		case SM1_readkeypad:
		break;
		
		default:
		state = SM1_readkeypad;
		break;
	}
	switch(state)
	{
		case SM1_readkeypad:
		x = GetKeypadKey();
		
		default:
		break;
	}
	return x;
}

unsigned char tmpdata = 0x00; // hold the current pattern and speed
enum SM2_State{SM2_wait, SM2_writeLCD, SM2_rlsd};
int SMTick2(int state)
{
	switch(state)
	{	
		case SM2_wait:
		if((x == '1') || (x == '2') || (x == '3') || (x == '4')
		|| (x == '5') || (x == '6') || (x == '7') || (x == '8')
		|| (x == '9') || (x == 'A') || (x == 'B') || (x == 'C')
		|| (x == 'D') || (x == '*') || (x == '0') || (x == '#'))
		{
			state = SM2_writeLCD;
		}
		else
		{
			break;
		}
		break;
		
		case SM2_writeLCD:
		state = SM2_rlsd;
		break;
		
		case SM2_rlsd:
		if((x == '1') || (x == '2') || (x == '3') || (x == '4')
		|| (x == '5') || (x == '6') || (x == '7') || (x == '8')
		|| (x == '9') || (x == 'A') || (x == 'B') || (x == 'C')
		|| (x == 'D') || (x == '*') || (x == '0') || (x == '#'))
		{
			state = SM2_rlsd;
		}
		else
		{
			state = SM2_wait;
		}
		
		default:
		state = SM2_wait;
		LCD_DisplayString(1, "Ptrn: 1  Spd: 1 uC: 1 ");LCD_Cursor(32);
		tmpdata = 0x11;
		data = 0x11;
		PORTD = data;
		break;
	}
	
	switch(state)
	{
		case SM2_wait:
		break;
		
		case SM2_writeLCD:
		switch(x)
		{
			//speed
			case '1': 
			LCD_Cursor(15);LCD_WriteData(1+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0xF0);
			tmpdata = (tmpdata | 0x01);
			break;
			
			case '2': 
			LCD_Cursor(15);LCD_WriteData(2+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0xF0);
			tmpdata = (tmpdata | 0x02);
			break;
			
			case '3': 
			LCD_Cursor(15);LCD_WriteData(3+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0xF0);
			tmpdata = (tmpdata | 0x03);
			break;
			
			case '4': 
			LCD_Cursor(15);LCD_WriteData(4+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0xF0);
			tmpdata = (tmpdata | 0x04);
			break;
			
			case '5': 
			LCD_Cursor(15);LCD_WriteData(5+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0xF0);
			tmpdata = (tmpdata | 0x05);
			break;
			
			case '6': 
			LCD_Cursor(15);LCD_WriteData(6+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0xF0);
			tmpdata = (tmpdata | 0x06);
			break;
			
			//servant
			case '7': 
			LCD_Cursor(21); LCD_WriteData(1+'0');LCD_Cursor(32);break;
			
			case '8': 
			LCD_Cursor(21); LCD_WriteData(2+'0');LCD_Cursor(32);break;
			
			case '9': 
			LCD_Cursor(21); LCD_WriteData(3+'0');LCD_Cursor(32);break;
			
			//Pattern
			case 'A': 
			LCD_Cursor(7); LCD_WriteData(1+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0x0F);
			tmpdata = (tmpdata | 0x10);
			break;
			
			case 'B': 
			LCD_Cursor(7); LCD_WriteData(2+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0x0F);
			tmpdata = (tmpdata | 0x20);
			break;
			
			case 'C': 
			LCD_Cursor(7); LCD_WriteData(3+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0x0F);
			tmpdata = (tmpdata | 0x30);
			break;
			
			case 'D': 
			LCD_Cursor(7); LCD_WriteData(4+'0');LCD_Cursor(32);
			tmpdata = (tmpdata & 0x0F);
			tmpdata = (tmpdata | 0x40);
			break;
			
			default : break ; // Should never occur. Middle LED off.
		}
		
		case SM2_rlsd:
		data = tmpdata;
		PORTD = data;
		SPI_MasterTransmit(data);
		break;

		default:
		break;
	}
	return 0;
}

int main(void)
{
    DDRA = 0xF0; PORTA = 0x0F;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
	SPI_MasterInit();
	
	unsigned long int SMTick1_calc = 10;
	unsigned long int SMTick2_calc = 50;
	
	unsigned long int tmpGCD = 1;
	tmpGCD  = findGCD(SMTick1_calc, SMTick2_calc);
	
	unsigned long int GCD = tmpGCD;
	unsigned long int SMTick1_period = SMTick1_calc/GCD;
	unsigned long int SMTick2_period = SMTick2_calc/GCD;
	
	static task task1, task2;
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks = sizeof (tasks)/ sizeof(task*);
		
	//Task 1
	task1.state = -1; //task initial state
	task1.period = SMTick1_period;
	task1.elapsedTime = SMTick1_period;
	task1.TickFct = &SMTick1;
	
	//Task 2
	task2.state = -1; //task initial state
	task2.period = SMTick2_period;
	task2.elapsedTime = SMTick2_period;
	task2.TickFct = &SMTick2;
	
	TimerSet(GCD);
	TimerOn();
	
	unsigned short i;
	
    while (1) 
    {
		for ( i = 0; i < numTasks; i++)
		{
			if(tasks[i]->elapsedTime == tasks[i]->period)
			{
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while (!TimerFlag);
		TimerFlag = 0;
    }
	return 0;
}
