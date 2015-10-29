/*
 * Device.c
 *
 * Created: 10/25/2015 7:01:09 PM
 *  Author: Gerald Chapman
 */ 
#include <avr/io.h>

void Initalize16BitPWMTimer (void)
{
	TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<WGM11);        //NON Inverted PWM
	TCCR1B|=(1<<WGM13)|(1<<WGM12)|(1<<CS11)|(1<<CS10); //PRESCALER=64 MODE 14(FAST PWM)
	ICR1 = 3124; //set output to 50Hz
}
