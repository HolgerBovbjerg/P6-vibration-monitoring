// MASTER TRANSMITTER PROGRAM

#include <stdio.h>


 void configure_I2C(void){
 	// CONFIGURING CLK FREQUENCY AND SLAVE ADDRESS
 	asm( " MOV #4008h, port(#1A24h) "); // PULLING THE RESET BIT LOW
 	asm( " MOV #110, port(#1A30h) ");   // CLOCK FREQUENCY PRESCALER - 8 BIT VALUE, THE HIGHER THE NUMBER THE LOWER THE FREQUENCY
 	asm( " MOV #65535, port(#1A0Ch) "); // LOW-TIME DIVIDER REGISTER - HIGH NUMBER=LOW FREQUENCY
 	asm( " MOV #65535, port(#1A10h) "); // HIGH-TIME DIVIDER REGISTER - HIGH NUMBER=LOW FREQUENCY
 	asm( " MOV #121, port(#1A1Ch) ");   // THE ADDRESS OF THE RECEIVING SLAVE
 	asm( " MOV #4028h, port(#1A24h) "); // PULLING THE RESET BIT HIGH
 	//while(1);
 
}

void transmitSomething(){
	// THIS FUNCTION IS DOING THE I2C TRANSMISSION
	asm( " MOV #28176, port(#1A24h) "); // STARTS THE TRANSFER
	asm( " MOV 1, port(1A14h) "); // Sets the data count to 1 data word to be transmitted
	asm( " MOV 211, port(1A20h) "); // puts '211' in the data transmit register
	asm( " MOV #28208, port(#1A24h) "); // STARTS THE TRANSFER - TRANSMITS THE CONTENT OF DATA TRANSMIT REGISTER
}

void eatTime(){
	unsigned long i = 0;
	for (i = 0; i < 50000000; i++) // DELAY TO MAKE DEBUGGING EASY
		{
			int t = 1;
		}	
	transmitSomething();
	printf ("%s\n", "Transmitting I2C");
}
int main()
{

printf ("%s\n", "Configuring I2C");

	configure_I2C();
	while(1){
		eatTime();
		} 
return 0;
}



