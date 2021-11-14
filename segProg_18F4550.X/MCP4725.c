
#include "config.h"
#include <xc.h>
#include "spi_i2c.h"
#include  "delay.h"
#include "MCP4725.h"


#define DAC  0b11000000
#define DAC_READ  0b11000001
#define NORMAL 0b01100000

void dac_init ( void )
{
    I2C_start();
    delay(1);
    I2C_tx( DAC );
    I2C_ack_receive();
    delay(1); 
    I2C_tx( NORMAL );
    I2C_ack_receive();
    delay(1);
}

void dac_write ( unsigned int  code )
{
    I2C_tx( code >> 4 );
    I2C_ack_receive();
    delay(1);
    I2C_tx( code << 4);
    I2C_ack_receive();           
    delay(1);
}

void MCP4725 (unsigned int Vout)
{
    dac_init();
    dac_write(Vout);
    I2C_stop();  
    delay(1); 
}

void MCP4725X (unsigned int Vout1 , unsigned int Vout2 )
{
    dac_init();
    dac_write(Vout1);
    I2C_stop();  
    delay(1);
    
    dac_init();
    dac_write(Vout2);
    I2C_stop();  
    delay(1);
}

//void dac_read ( unsigned char *ptr)
//{
//    dac_init();
//        
//    while( *ptr < 2  )
//    {
//         *ptr++ =   I2C_rx();
//         I2C_ack_transmit(1);
//         delay(1);
//    }
//    
//    I2C_ack_transmit(0);
//    delay(1);
//    I2C_stop();
//    delay(1);
//}




//void I2C_read( unsigned char slave, unsigned char adrs, unsigned int *ptr, unsigned char length )
//{
//   
//    I2C_repeatStart();
//    I2C_tx( slave<<1 | 0x01);
//    I2C_ack_receive();
//    *ptr++ = I2C_rx();
//
//        I2C_ack_transmit( 1 );
//        *ptr++ = I2C_rx();
//    }
//    I2C_ack_transmit( 0 );
//    I2C_stop();
//}





//void dac_fast (void)
//{
//        I2C_start();
//        delay(10);
//        I2C_tx(DAC);
//        I2C_ack_receive();
//        delay(10);
//        
//        I2C_stop();
//    
//}
//
//void dac_writex ( unsigned int d)
//{
//        I2C_tx( d >> 8);
//        I2C_ack_receive() ;
//        delay(10);
//        I2C_tx( d );
//        I2C_ack_receive();
//        delay(10);
//    
//}
//
//void fast (unsigned int data)
//{
//    dac_fast();
//    dac_writex(data);
//    I2C_stop();
//}























