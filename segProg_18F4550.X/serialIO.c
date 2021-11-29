/*
 * File:   serialIO.c
 * Author: curtaeletronica
 *
 * Created on 30 de Setembro de 2019, 19:36
 * 
 * Objetivo: 
 *      Camada de Abstra��o de Hardware:
 *          Interface com o CIs 74595 e 74165.
 * 
 * Pinos    |n�     |Conex�o
 * RA3      |5      | SHLD : Shift / Load
 * RA2      |4      | CLK : Clock
 * RA0      |2      | OUT : Output
 * RA1      |3      | IN : Input
 */



#include <xc.h>
#include "serialIO.h"

char * ptrSerialIn;
char * ptrSerialOut;
unsigned char   lenSerialIO;

// ************************** HAL
#define SERIALIO_ADDRS 0xF80     //Endereco PIC18F4550

typedef union
{
    struct
    {         
        unsigned char OUT   :1;
        unsigned char IN    :1;
        unsigned char CLK   :1;
        unsigned char SHLD  :1;
        unsigned char       :4;
    };
} SERIALIObits_t;
volatile SERIALIObits_t SERIALIO __at(SERIALIO_ADDRS);



void serialIOload( void )
{
    SERIALIO.SHLD = 0;
    SERIALIO.SHLD = 1;
}
unsigned char serialIObyteShift( unsigned char dataIn )
{
    unsigned char dataBit;
    unsigned char dataOut;

    dataBit = 0x80;
    dataOut = 0;

    while( dataBit )
    {
        SERIALIO.CLK = 0;
        if( dataIn & dataBit )
            SERIALIO.OUT = 1;
        else
            SERIALIO.OUT = 0;

        if( SERIALIO.IN )
            dataOut |= dataBit;
        SERIALIO.CLK = 1;
        dataBit >>= 1;
    }
    SERIALIO.CLK = 0;
    return( dataOut );
}

//***************** Interface Serial IO Scan
void serialIOscan( void )
{
    unsigned char i,j;
    j = lenSerialIO - 1;   //j = 0
    serialIOload();
    for( i=0; i<lenSerialIO; i++ )
    {
        ptrSerialIn[i] = serialIObyteShift( ptrSerialOut[j-i]);
    }
    serialIOload();
}

//***************** Inicialia��o de interface com SerialIO
void initSerialIO( char * ptrIn, char * ptrOut, unsigned char length )
{
    ADCON1 = 0xFF;
    
    ptrSerialIn = ptrIn;      //ponteiro aponta para primeira posicao do vetor ptrIn
    ptrSerialOut = ptrOut;    //ponteiro aponta para primeira posicao do vetor ptrOut
    lenSerialIO = length;     //lenSerialIO = 1
// ************************** Serial IO Shift/Load
    PORTAbits.RA3 = 0;
//    ANSELbits.ANS3 = 0;
    TRISAbits.TRISA3 = 0;

// ************************** Serial IO Clock
    PORTAbits.RA2 = 0;
//    ANSELbits.ANS2 = 0;
    TRISAbits.TRISA2 = 0;

// ************************** Serial IO Output
    PORTAbits.RA0 = 0;
//    ANSELbits.ANS0 = 0;
    TRISAbits.TRISA0 = 0;

// ************************** Serial IO Input
    PORTAbits.RA1 = 0;
//    ANSELbits.ANS1 = 0;
    TRISAbits.TRISA1 = 1;
    
//***************** Inicializa��o dos pinos de interface com os CIs
    SERIALIO.CLK = 0;
    SERIALIO.OUT = 0;
    SERIALIO.SHLD = 1;
}