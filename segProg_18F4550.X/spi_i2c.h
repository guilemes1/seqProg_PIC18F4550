#ifndef SPI_I2C_H
#define SPI_I2C_H

void I2C_init( unsigned long freq );

void I2C_start( void );

void I2C_repeatStart( void );

void I2C_tx( unsigned char b );

void I2C_stop( void );

unsigned char I2C_rx( void );

void I2C_ack_receive( void );

void I2C_ack_transmit( unsigned char ack );

#endif 