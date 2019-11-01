/*	Partner(s) Name & E-mail: Minwhan Oh & moh008@ucr.edu
 * Lab Section: 021
 * assignment: Lab3 Exercise 2: Servant code
 * Write a "pattern" synchSM for each pattern described in the 
 * "Overall System Description" part of the lab. Each "pattern" 
 * synchSM writes its pattern to a unique shared variable.
 *
 * Created: 2019-15-2019 4:52 pm
 * Author : Stephanie Cabrera
 *
 * I acknowledge all content contained herein, excluding template
 * or example code, is my own original work.
 */

//#include <scheduler.h>
#include <timer.h>


	typedef struct task {
		signed 	 char state; 		//Task's current state
		unsigned long period; 		//Task period
		unsigned long elapsedTime; 	//Time elapsed since last task tick
		int (*TickFct)(int); 		//Task tick function
	} task;

//Shared Variables//
unsigned char receivedData = 0x00;

// End of shared variables //

//Servant code
void SPI_ServantInit(void){
	//set DDRB to have MISO line as output and MOSI, SCK, and SS as input
	// set SPCR register to enable SPI and enable SPI interrupt (pg. 168)
	// make sure global interrupts are enabled on SREG register (pg.9)
	DDRB = 0x40;	// 0100 0000 output
	PORTB = 0xB0;	// 1011 0000 input
	SPCR = 0xC0;	// 1100 0000
	SREG = 0x80;	// 1000 0000
}
ISR(SPI_STC_vect){
	// this is enabled in init with the SPCR register's "SPI Interrupt Enable"
	// SPDR contains the received data, e.g. unsigned char receivedData = SPDR;
	receivedData = SPDR;
}

// -----------------------------
// Shared Variables
// -----------------------------
unsigned char output1 = 0xF0;
unsigned char output2 = 0xAA;
unsigned char output3 = 0x80;
unsigned char output4 = 0x01;

// ==== Possible Output Patterns ================== // 
// 1) 11110000 -> 00001111
// 2) 10101010 -> 01010101
// 3) 10000000 -> 01000000 -> 00100000 -> ?-> 00000001 -> 00000010 -> 00000100 -> ?
// 4) Create your own.
// ==== End of Possibles Output Patterns ==========//

// -----------------------------
// State Machines
// -----------------------------


enum SM1_States{SM1_Write} state;
int SMTick1(int state){
	// State Transitions
	switch(state){
		case SM1_Write:
			state = SM1_Write;
		break;
		
		default: break;
	};
	
	// State Actions
	switch(state){
		case SM1_Write:
			output1 = ~output1;
		break;
		
		default: break;		
	}
	return state;
};

enum SM2_States{SM2_Write} state2;
int SMTick2(int state2){
	// State Transitions
	switch(state2){
		
		case SM2_Write:
			state2 = SM2_Write;
		break;
		
		default: break;
	};
	
	// State Actions
	switch(state2){
		case SM2_Write:
			output2 = ~output2;
		break;
		
		default: break;
	}
	return state2;
};

enum SM3_States{SM3_Write} state3;
int SMTick3(int state){
	// State Transitions
	switch(state3){
		case SM3_Write:
		 state3 = SM3_Write; 
		break;
		
		default: break;
	};
	
	// State Actions
	switch(state3){
		case SM3_Write:
			if(output3 == 0x01){
				output3 = 0x80;
			}
			else{
				output3 = output3 >> 1 ;
			}
			
		break;
		
		default: break;
	}
	return state3;
};

enum SM4_States{SM4_Write} state4;
int SMTick4(int state4){
	// State Transitions
	switch(state4){		
		case SM4_Write:
			state4 = SM4_Write;
		break;
		
		default: break;
	};
	
	// State Actions
	switch(state4){
		case SM4_Write:
			if(output4 == 0x80){
				output4 = 0x01;
			}
			else{
				output4 = output4 << 1 ;
			}
		break;
		
		default: break;
	}
	return state4;
};

enum SM5_States{SM5_Output} state5;
int SMTick5(int state5){
	// State Transitions
	switch(state){		
		case SM5_Output:
			state5 =  SM5_Output;
		break;
		
		default: break;
	};
	
	// State Actions
	switch(state5){
		case SM5_Output:
		 if((receivedData >> 4 ) == 0x01){PORTD = output1;}
		 else if((receivedData >> 4 ) == 0x02){PORTD = output2;}
		 else if((receivedData >> 4 ) == 0x03){PORTD = output3;}
		 else if((receivedData >> 4 ) == 0x04){PORTD = output4;}
		break;
		
		default: break;
	}
	return state5;
};

int main(void)
{
    /* Replace with your application code */
	DDRD = 0xFF; PORTD = 0x00; // These are the LEDs.
	//DDRD = 0xFF; PORTD = 0x00;
	//DDRA = 0x0F; PORTA = 0xF0;
	
	//Tasks
	
		//Recalculate GCD periods for scheduler
		unsigned long int SMTick1_period = receivedData & 0x0F;
		unsigned long int SMTick2_period = receivedData & 0x0F;
		unsigned long int SMTick3_period = receivedData & 0x0F;
		unsigned long int SMTick4_period = receivedData & 0x0F;
		unsigned long int SMTick5_period = receivedData & 0x0F;
		
	static task task1, task2, task3, task4, task5; 
	task* tasks[] = {&task1, &task2, &task3, &task4, &task5};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);

	// Task 1
	task1.state = SM1_Write;//Task initial state.
	task1.period = SMTick1_period;//Task Period.
	task1.elapsedTime = SMTick1_period;//Task current elapsed time.
	task1.TickFct = &SMTick1;//Function pointer for the tick.
	
	// Task 2
	task2.state = SM2_Write;//Task initial state.
	task2.period = SMTick2_period;//Task Period.
	task2.elapsedTime = SMTick2_period;//Task current elapsed time.
	task2.TickFct = &SMTick2;//Function pointer for the tick.
	
	
	task3.state = SM3_Write;//Task initial state.
	task3.period = SMTick3_period;//Task Period.
	task3.elapsedTime = SMTick3_period;//Task current elapsed time.
	task3.TickFct = &SMTick3;//Function pointer for the tick.

	task4.state = SM4_Write;//Task initial state.
	task4.period = SMTick4_period;//Task Period.
	task4.elapsedTime = SMTick4_period;//Task current elapsed time.
	task4.TickFct = &SMTick4;//Function pointer for the tick.
	
	task5.state = SM5_Output;//Task initial state.
	task5.period = SMTick5_period;//Task Period.
	task5.elapsedTime = SMTick5_period;//Task current elapsed time.
	task5.TickFct = &SMTick5;//Function pointer for the tick.		
	
	SPI_ServantInit();
	
	// End of tasks
	//receivedData = 0x11;
	TimerOn();
	unsigned short i; // Scheduler for-loop iterator
    while (1) {
		// Scheduler code
			receivedData = 0x11;
			if((receivedData & 0x0F) == 1){TimerSet(2000);}
			else if((receivedData & 0x0F) == 2){TimerSet(1000);}
			else if((receivedData & 0x0F) == 3){TimerSet(500);}
			else if((receivedData & 0x0F) == 4){TimerSet(250);}
			else if((receivedData & 0x0F) == 5){TimerSet(100);}
			else if((receivedData & 0x0F) == 6){TimerSet(50);}
		for ( i = 0; i < numTasks; i++ ) {
			// Task is ready to tick
			if ( tasks[i]->elapsedTime == tasks[i]->period ) {
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				// Reset the elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
	}

	return 0;
}



