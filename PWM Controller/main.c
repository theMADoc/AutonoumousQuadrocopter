/*
 * PWM Controller.c
 *
 * Created: 10/26/2015 7:51:17 PM
 * Author : Gerald Chapman
 */ 

#include <avr/io.h>
#include "TWI.h"

#define SLAVEADD 0x21

int main(void)
{
	//DDRC |= 0x30;				//
	TWI_init_slave(SLAVEADD);		//Initialize slave mode
		
	//Initialize Start Conditions
	DDRC |= 0x0F;
	char temp = 0x0A;
	
    /* Replace with your application code */
    while (1) 
    {
		if((TWSR & 0xF8) == 0x60) //Check if new data is available
		{
			TWI_match_read_slave();
			TWI_read_slave(&temp);
		}
		PINC = (temp & 0x0F);
    }
}