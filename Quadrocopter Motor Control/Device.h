/*
 * Device.h
 *
 * Created: 10/25/2015 7:04:08 PM
 *  Author: Gerald Chapman
 */

#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>

void Initalize16BitPWMTimer (void);