/*
 * ChipCom.h
 *
 * Created: 10/25/2015 11:46:47 AM
 * Author: Gerald Chapman
 * Developed Using AVR300 & AVR306 application Note
 * Basic Routines for I2c and UART Communication
 */ 

#ifndef _CHIPCOM_H
#define _CHIPCOM_H

#include <avr/io.h>

//Slave Addresses go here
#define SLAVE_MOTORCNTRL1 (0x20)		//SLAVE ADDRESS FOR PWM CONTROLLER 1


//TWI Definitions
#define WRITE 0							//WRITE CODE
#define READ 1							//READ CODE
#define GEN_CALL (0x00)					//ADDRESS FOR GENERAL CALL

/*TWI Master Prototypes*/
extern void i2c_init_Master(void);
extern void i2c_stop(void);
extern unsigned char i2c_start(unsigned char addr);
extern unsigned char i2c_rep_start(unsigned char addr);
extern void i2c_start_wait(unsigned char addr);
extern unsigned char i2c_write_Master(unsigned char data);
extern unsigned char i2c_readAck_Master(void);
extern unsigned char i2c_readNak_Master(void);

/******************************************************************************
 Read one byte from the I2C device
 Implemented as a macro, which calls either i2c_readAck() or i2c_readNak()
 param    ack 1 send ack, request more data from device
              0 send nak, read is followed by a stop condition 
 return   byte read from I2C device
 *****************************************************************************/
extern unsigned char i2c_read_Master(unsigned char ack);
#define i2c_read_Master(ack)  (ack) ? i2c_readAck() : i2c_readNak(); 

//TWI Slave Prototypes
extern void TWI_init_slave(unsigned char);
extern void TWI_match_read_slave(void);
extern unsigned char TWI_read_slave(void);
extern void TWI_match_write_slave(void);
extern void TWI_write_slave(char);

//UART Defines
extern void USART_Init(unsigned int);
extern unsigned char USART_Recieve(void);
extern void USART_Transmit(unsigned char);

#endif