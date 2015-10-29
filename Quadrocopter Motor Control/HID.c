/*
 * HID.c
 *
 * Created: 10/25/2015 9:40:40 PM
 *  Author: Gerald Chapman
 */ 

#include <avr/io.h>
#include "HID.h"
#include "Calculations.h"

void InitializeBacklightTimer(void)
{
	//Start Both 8 Bit timers
	TCCR0A |= (1<<COM0A1)|(1<<COM0B1)|(1<<WGM01)|(1<<WGM00);
	TCCR0B |= (1<<CS01);
	TCCR2A |= (1<<COM2B1)|(1<<WGM21)|(1<<WGM20);
	TCCR2B |= (1<<CS21);
}

void SetBacklight(uint8_t red,uint8_t green,uint8_t blue)
{
	//Normalize Red and Green LED, They appear brighter
	red = map(red, 0, 255, 0, 100);
	green = map(green, 0, 255, 0, 150);
	
	Red_Output = map(red, 0, 255, 0, 255);
	Green_Output = map(green, 0, 255, 0, 255);
	Blue_Output = map(blue, 0, 255, 0, 255);
}