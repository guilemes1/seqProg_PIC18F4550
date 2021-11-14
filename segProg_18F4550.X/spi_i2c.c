// Módulo Master Synchronous Serial Port (MSSP) : i2c ? Inter-Integrated Circuit (I2C)

#include <xc.h>
#include "spi_i2c.h"
#include "delay.h"
#include "config.h"

#define I2C_MASTER      0x8



void I2C_init( unsigned long freq )
{
    TRISBbits.TRISB0 = 1;               // SDA  //Alterado para PIC18F4550
    TRISBbits.TRISB1 = 1;               // SCL  //Alterado para PIC18F4550 
    SSPCON1bits.SSPM = I2C_MASTER;
    SSPADD = ((_XTAL_FREQ>>2) / (freq))-1;
    SSPSTATbits.SMP = 1;
    SSPCON1bits.SSPEN = 1;
}

void I2C_start( void )
{
    SSPCON2bits.SEN = 1;
    while( !PIR1bits.SSPIF );
    PIR1bits.SSPIF = 0;
}
void I2C_repeatStart( void )
{
    while( SSPSTATbits.R_W )        //idle
        ;
    SSPCON2bits.RSEN = 1;
    while( !PIR1bits.SSPIF );
    PIR1bits.SSPIF = 0;
}
void I2C_tx( unsigned char b )
{
    SSPBUF = b;
    while( SSPSTATbits.BF );
}
void I2C_stop( void )
{
    SSPCON2bits.PEN = 1;
    while( !PIR1bits.SSPIF );
    PIR1bits.SSPIF = 0;
}
unsigned char I2C_rx( void )
{
    while( SSPSTATbits.R_W )        //idle
        ;
    SSPCON2bits.RCEN = 1;
    while( SSPCON2bits.RCEN );
    return( SSPBUF );
}

void  I2C_ack_receive( void )
{
    while( SSPCON2bits.ACKSTAT );
  //  return(SSPCON2bits.ACKSTAT);
}

void I2C_ack_transmit( unsigned char ack )
{
    if( ack )
        SSPCON2bits.ACKDT = 0;
    else
        SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
    while( SSPCON2bits.ACKEN );
}
