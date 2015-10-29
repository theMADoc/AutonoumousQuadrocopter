/*
 * ChipCom.c
 *
 * Created: 10/25/2015 11:12:00 AM
 *  Author: Gerald Chapman
 */ 

//Standard Header Files
#include <inttypes.h>
#include <compat/twi.h>

#include "ChipCom.h"

/*DEFINE CHIP CPU FREQUENCY HERE IN NOT IN MAKEFILE
#ifdef F_CPU
#define F_CPU 1000000UL
#endif*/
#define F_CPU 8000000UL

//Define I2C Bus Clock
#define SCL_CLOCK  100000L

/*TWI Master Mode Functions*/

/*************************************************************************
 Initialization of the I2C bus interface. Need to be called only once
*************************************************************************/
void i2c_init_Master(void)
{
  /* initialize TWI clock: 100 kHz clock, TWPS = 0 => prescaler = 1 */
  
  TWSR = 0;                         /* no prescaler */
  TWBR = ((F_CPU/SCL_CLOCK)-16)/2;  /* must be > 10 for stable operation */

}


/*************************************************************************	
  Issues a start condition and sends address and transfer direction.
  return 0 = device accessible, 1= failed to access device
*************************************************************************/
unsigned char i2c_start_Master(unsigned char address)
{
    uint8_t   twst;

	// send START condition
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_START) && (twst != TW_REP_START)) return 1;

	// send device address
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wail until transmission completed and ACK/NACK has been received
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits.
	twst = TW_STATUS & 0xF8;
	if ( (twst != TW_MT_SLA_ACK) && (twst != TW_MR_SLA_ACK) ) return 1;

	return 0;

}


/*************************************************************************
 Issues a start condition and sends address and transfer direction.
 If device is busy, use ack polling to wait until device is ready
 
 Input:   address and transfer direction of I2C device
*************************************************************************/
void i2c_start_wait(unsigned char address)
{
    uint8_t   twst;


    while ( 1 )
    {
	    // send START condition
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    
    	// wait until transmission completed
    	while(!(TWCR & (1<<TWINT)));
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst != TW_START) && (twst != TW_REP_START)) continue;
    
    	// send device address
    	TWDR = address;
    	TWCR = (1<<TWINT) | (1<<TWEN);
    
    	// wail until transmission completed
    	while(!(TWCR & (1<<TWINT)));
    
    	// check value of TWI Status Register. Mask prescaler bits.
    	twst = TW_STATUS & 0xF8;
    	if ( (twst == TW_MT_SLA_NACK )||(twst ==TW_MR_DATA_NACK) ) 
    	{    	    
    	    /* device busy, send stop condition to terminate write operation */
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	        
	        // wait until stop condition is executed and bus released
	        while(TWCR & (1<<TWSTO));
	        
    	    continue;
    	}
    	//if( twst != TW_MT_SLA_ACK) return 1;
    	break;
     }

}


/*************************************************************************
 Issues a repeated start condition and sends address and transfer direction 

 Input:   address and transfer direction of I2C device
 
 Return:  0 device accessible
          1 failed to access device
*************************************************************************/
unsigned char i2c_rep_start(unsigned char address)
{
    return i2c_start_Master( address );

}


/*************************************************************************
 Terminates the data transfer and releases the I2C bus
*************************************************************************/
void i2c_stop(void)
{
    /* send stop condition */
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	
	// wait until stop condition is executed and bus released
	while(TWCR & (1<<TWSTO));

}


/*************************************************************************
  Send one byte to I2C device
  
  Input:    byte to be transfered
  Return:   0 write successful 
            1 write failed
*************************************************************************/
unsigned char i2c_write_Master( unsigned char data )
{	
    uint8_t   twst;
    
	// send data to the previously addressed device
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);

	// wait until transmission completed
	while(!(TWCR & (1<<TWINT)));

	// check value of TWI Status Register. Mask prescaler bits
	twst = TW_STATUS & 0xF8;
	if( twst != TW_MT_DATA_ACK) return 1;
	return 0;

}/* i2c_write */


/*************************************************************************
 Read one byte from the I2C device, request more data from device 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readAck_Master(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));    

    return TWDR;

}


/*************************************************************************
 Read one byte from the I2C device, read is followed by a stop condition 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char i2c_readNak_Master(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
    return TWDR;

}


/**************************** TWI Slave Mode Function********************/

/*************************************************************************
 Initialize the device for slave mode
 
 Return:  void
*************************************************************************/
void TWI_init_slave(unsigned char address)
{
	TWAR=address; // Fill slave address to TWAR 0x20
}


/*************************************************************************
 Write one byte to the I2C device
 
 Return:  Void
*************************************************************************/
void TWI_write_slave(char data)
{
	TWDR= data;						// Fill TWDR register with the data to be sent
	TWCR= (1<<TWEN)|(1<<TWINT);		// Enable TWI, Clear TWI interrupt flag
	while((TWSR & 0xF8) != 0xC0);	// Wait for the acknowledgment
}


/*************************************************************************
 Function to match the slave address and slave direction bit(write) 
 
 Return:  byte read from I2C device
*************************************************************************/
void TWI_match_write_slave(void)
{
	while((TWSR & 0xF8)!= 0xA8) // Loop till correct acknowledgment have been received
	{
		// Get acknowledgment, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
	}
}


/*************************************************************************
 Function to read byte from I2C Master 
 
 Return:  byte read from I2C device
*************************************************************************/
unsigned char TWI_read_slave(void)
{
	// Clear TWI interrupt flag,Get acknowledgment, Enable TWI
	TWCR=(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));	// Wait for TWINT flag
	while((TWSR & 0xF8)!=0x80);		// Wait for acknowledgment
	return TWDR;					// Get value from TWDR
}


/*************************************************************************
 Function to match the slave address and slave direction bit(read)
 
 Return:  byte read from I2C device
*************************************************************************/
void TWI_match_read_slave(void)
{
	while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknowledgment have been received
	{
		// Get acknowledgment, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
	}
}

//UART PROTOTYPES
/*************************************************************************
 Function Initialize URART
 
 Return:  byte read from I2C device
*************************************************************************/
void USART_Init(unsigned int ubrr)
{
	//Set Baud Rate
	UBRR0H = (unsigned char) (ubrr>>8);
	UBRR0L = (unsigned char) ubrr;
	//Enable Receiver and Transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//Set Frame Format: 8data, 2 stop bit
	UCSR0C = (3<<UCSZ00);
}

unsigned char USART_Recieve(void)
{
	//wait for data to be received
	while(!(UCSR0A&(1<<RXC0)));
	return UDR0;
}

void USART_Transmit(unsigned char data)
{
	//Wait for empty transmit buffer
	while(!(UCSR0A & (1<<UDRE0)));
	//Transmit
	UDR0 = data;
}