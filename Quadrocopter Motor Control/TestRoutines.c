/*
 * TestRoutines.c
 *
 * Created: 10/25/2015 10:03:24 AM
 *  Author: Gerald Chapman
 */ 
#include <inttypes.h>

void PWMtest(volatile uint16_t *port,uint8_t temp)
{
	temp = temp & 0x03;
	if(temp == 0x03) //Full speed ~2ms
	{
		*port = 310;
	}
	else if(temp == 0x02) //Half Speed ~1.5ms
	{
		*port = 250;
	}
	else if(temp == 0x01) //Quarter Speed ~1.25ms
	{
		*port = 190;
	}
	else //Min Speed 1ms
	{
		*port = 177;
	}
}