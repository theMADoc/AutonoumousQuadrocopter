/*
 * TWI.h
 *
 * Created: 10/25/2015 11:46:47 AM
 *  Author: Gerald Chapman
 */ 

//Master Prototypes
void TWI_start(void);
void TWI_repeated_start(void);
void TWI_init_master(void);
void TWI_write_address(unsigned char);
void TWI_read_address(unsigned char);
void TWI_write_data(unsigned char);
void TWI_read_data(unsigned char *);
void TWI_stop(void);

//Slave Prototypes
void TWI_init_slave(uint8_t);
void TWI_match_read_slave(void);
void TWI_read_slave(char *);
void TWI_match_write_slave(void);
void TWI_write_slave(char);