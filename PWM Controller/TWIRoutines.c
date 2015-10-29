/*
 * TWIMaster.c
 *
 * Created: 10/25/2015 11:12:00 AM
 *  Author: Gerald Chapman
 */ 

//Standard Header Files
#include<avr/io.h>

/*Master Mode Functions*/

void TWI_init_master(void) // Function to initialize master
{
	TWBR=0x01; // Bit rate
	TWSR=(0<<TWPS1)|(0<<TWPS0); // Setting prescalar bits
}// SCL freq= F_CPU/(16+2(TWBR).4^TWPS)



void TWI_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // Wait till start condition is transmitted
	while((TWSR & 0xF8)!= 0x08); // Check for the acknowledgment
}



void TWI_repeated_start(void)
{
	// Clear TWI interrupt flag, Put start condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT))); // wait till restart condition is transmitted
	while((TWSR & 0xF8)!= 0x10); // Check for the acknowledgment
}



void TWI_write_address(unsigned char data)
{
	TWDR=data; // Address and write instruction
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8)!= 0x18);  // Check for the acknowledgment
}



void TWI_read_address(unsigned char data)
{
	TWDR=data; // Address and read instruction
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte received
	while((TWSR & 0xF8)!= 0x40);  // Check for the acknowledgment
}



void TWI_write_data(unsigned char data)
{
	TWDR=data; // put data in TWDR
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x28); // Check for the acknowledgment
}



void TWI_read_data(unsigned char *data)
{
	TWCR=(1<<TWINT)|(1<<TWEN);    // Clear TWI interrupt flag,Enable TWI
	while (!(TWCR & (1<<TWINT))); // Wait till complete TWDR byte transmitted
	while((TWSR & 0xF8) != 0x58); // Check for the acknowledgment
	*data=TWDR;
	PORTB=*data;
}



void TWI_stop(void)
{
	// Clear TWI interrupt flag, Put stop condition on SDA, Enable TWI
	TWCR= (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(!(TWCR & (1<<TWSTO)));  // Wait till stop condition is transmitted
}


/* Slave Mode Function*/
void TWI_init_slave(uint8_t address) // Function to initialize slave
{
	TWAR=address; // Fill slave address to TWAR 0x20
}



void TWI_write_slave(char data) // Function to write data
{
	TWDR= data;						// Fill TWDR register with the data to be sent
	TWCR= (1<<TWEN)|(1<<TWINT);		// Enable TWI, Clear TWI interrupt flag
	while((TWSR & 0xF8) != 0xC0);	// Wait for the acknowledgment
}



void TWI_match_write_slave(void) //Function to match the slave address and slave direction bit(write)
{
	while((TWSR & 0xF8)!= 0xA8) // Loop till correct acknowledgment have been received
	{
		// Get acknowledgment, Enable TWI, Clear TWI interrupt flag
		TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
		while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
	}
}



void TWI_read_slave(char *data)
{
	// Clear TWI interrupt flag,Get acknowledgment, Enable TWI
	TWCR=(1<<TWINT)|(1<<TWEA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));	// Wait for TWINT flag
	while((TWSR & 0xF8)!=0x80);		// Wait for acknowledgment
	*data=TWDR;						// Get value from TWDR
}



void TWI_match_read_slave(void) //Function to match the slave address and slave direction bit(read)
{
	//while((TWSR & 0xF8)!= 0x60)  // Loop till correct acknowledgment have been received
	// Get acknowledgment, Enable TWI, Clear TWI interrupt flag
	TWCR=(1<<TWEA)|(1<<TWEN)|(1<<TWINT);
	while (!(TWCR & (1<<TWINT)));  // Wait for TWINT flag
}